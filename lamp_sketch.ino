#include <Adafruit_NeoPixel.h>
#include "Config.h"
#include "log.h"
#include "StartTimeInfo.h"
#include "Strip_class.h"
#include "TimerHandler.h"

#include <EncButton.h>

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <FS.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "Pass.h" //  define in this file SSID and PASS of ur wifi

const char* geoApiUrl = "http://ip-api.com/json/";
const char* ssid = SSID;
const char* password = PASS;
const unsigned int localPort = 8888;
#ifdef MATR16x16
IPAddress local_ip(192, 168, 1, 42);
#else
#ifdef MATR10x10
IPAddress local_ip(192, 168, 1, 41);
#endif // MATR10x10
#endif

IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

ESP8266WebServer server(80);
WiFiUDP Udp;
inline void handleUDP();

WiFiUDP NtpUdp;
NTPClient timeClient(NtpUdp, "pool.ntp.org", GTMOFFSET);

bool udp_state = false;
int br = 1000;
volatile bool is_enc = false;

EncButtonT<ENC_S1, ENC_S2, ENC_KEY> encoder;
Strip strip(MATR_LEN, STRIP_PIN);
TimerHandler timerHandler;

IRAM_ATTR void enc_click() {
	is_enc = true;
	br = strip.get_br();
	digitalWrite(RED_PIN, HIGH);
}

void setup() {
	randomSeed(analogRead(0));
#ifdef LOG_USB_ENABLE
	Serial.begin(115200);
#endif // LOG_USB_ENABLE
	LOG_USB_STARTUP("\nstart\n");
	IEventTimer::obj = &strip;
	strip.begin();
	strip.fill(strip.Color(0, 0, 0));
	delay(0);
	strip.show();
#ifdef MATR10x10
	pinMode(inbr_cur_PIN, INPUT);
	pinMode(ORNG_PIN, OUTPUT);
	digitalWrite(ORNG_PIN, LOW);
#endif // MATR10x10

	pinMode(RED_PIN, OUTPUT);
	digitalWrite(RED_PIN, LOW);
	pinMode(GREEN_PIN, OUTPUT);
	digitalWrite(GREEN_PIN, LOW);
#ifdef MATR16x16
	pinMode(MOSFET_PIN, OUTPUT);
	digitalWrite(MOSFET_PIN, HIGH);
#endif
	attachInterrupt(ENC_S1, enc_click, CHANGE);
	EEPROM.begin(4096);

	LOG_USB_STARTUP("eeprom[0] = %d\n", EEPROM.read(0));
	if (EEPROM.read(0) & 0b1000'0000)
	{
		LOG_USB_STARTUP("first start\n %d\n", EEPROM.read(0) & 0b0111'1111);
		for (int i = 0; i < OBJ_DATA_CAP * 2 + 1; i++)
		{
			EEPROM.write(i, (byte)0x00);
		}
		EEPROM.put(1, (OBJ_DATA_CAP + 1) * 2 + 1);
		EEPROM.commit();
	}

	WiFi.config(local_ip, gateway, subnet, primaryDNS, secondaryDNS);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		digitalWrite(RED_PIN, HIGH);
		delay(250);
		digitalWrite(RED_PIN, LOW);
		delay(250);
		LOG_USB_STARTUP("Connecting to WiFi...\n");
	}
	LOG_USB_STARTUP("Connected! IP address: %s\n", WiFi.localIP().toString().c_str());

	server.on("/", HTTP_GET, handleRoot);
	server.on("/submit", HTTP_POST, handleCommand);
	server.on("/get-effect-option", HTTP_GET, handleGetEffectOption);
	server.on("/get-timers", HTTP_GET, handleGetTimers);

	Udp.begin(localPort);
	LOG_USB_STARTUP("UDP server on port %d\n", localPort);

	server.begin();

	timeClient.begin();
	timeClient.forceUpdate();
	StartTimeInfo::start_millis_time = millis();
	StartTimeInfo::start_epoch_time = timeClient.getEpochTime();
	StartTimeInfo::start_weekday = timeClient.getDay();
	StartTimeInfo::start_day_time = timeClient.getSeconds() + timeClient.getMinutes() * 60 + timeClient.getHours() * 3600;
	timeClient.end();

	if (!SPIFFS.begin()) {
		LOG_USB_STARTUP("Failed to mount file system\n");
		return;
	}


	digitalWrite(RED_PIN, LOW);
	timerHandler.addActiveTimer(new EffectEventTimer());
	Serial.println("after addActiveTimer");
	timerHandler.addActiveAllFromMem();
	Serial.println("after addActiveAllFromMem");
	strip.set_effect(0);
	strip.set_br(br);
	strip.tick();
}

void loop() {
	server.handleClient();
	if (udp_state)
	{
		handleUDP();
	}
	if (is_enc)
	{
		encoder.tick();
		if (encoder.hold())
			if (timerHandler.getActiveState(0))
			{
				timerHandler.parse("s 0 0");
			}
			else
			{
				timerHandler.parse("s 0 1");
			}
		if (encoder.turn()) {
			if (encoder.pressing())
			{
				br += (encoder.fast() ? 500 : 100) * encoder.dir();
			}
			else
			{
				br += (encoder.fast() ? 25 : 1) * encoder.dir();
			}
			if (br > strip.get_max_br())
				br = strip.get_max_br();
			if (br < 0)
				br = 0;
			LOG_USB_ENC("%d\n", br);
			strip.set_br(br);
			strip.tick();
		}
		if (encoder.timeout(5000))
		{
			is_enc = false;
			digitalWrite(RED_PIN, LOW);
		}
	}

	while (Serial.available() > 0)
	{
		strip.parseSingle(Serial.readStringUntil('\n').c_str());
		strip.tick();
	}
	timerHandler.tickAll();
}

inline void handleUDP() {
	if (Udp.parsePacket()) {
		byte inbytes[3];
		Udp.read(inbytes, 3);
		strip.udp_set_color(Color_str(inbytes[0], inbytes[1], inbytes[2]));
		strip.tick();
		LOG_USB_UPD("UDP Received Data: ");
		LOG_USB_UPD("Color: %02X%02X%02X\n", inbytes[0], inbytes[1], inbytes[2]);
	}
}

void handleRoot() {
	File file = SPIFFS.open("/index.html", "r");
	if (file) {
		server.streamFile(file, "text/html");
		file.close();
	}
	else {
		server.send(500, "text/plain", "Internal Server Error");
	}
}

void handleCommand()
{
	String commands = server.arg("plain");
	while (commands.length() > 0)
	{
		int index = commands.indexOf('\n');
		if (index == -1)
		{
			parseCommand(const_cast<char*>(commands.c_str()));
			strip.tick();
			break;
		}
		parseCommand(const_cast<char*>(commands.substring(0, index).c_str()));
		strip.tick();
		commands = commands.substring(index + 1);
	}
	server.send(200, "text/plain", "Data received successfully");
}

void parseCommand(char* input_str)
{
	LOG_USB_SWITCH("parse: %s\n", input_str);
	char key = input_str[0];
	input_str++;
	switch (key)
	{
	case 'm':
	{
		key = input_str[0];
		input_str++;
		switch (key)
		{
		case 'd':
		{
			strip.apply_default_option();
			break;
		}
		case 'b':
		{
			int br;
			parseIn_int(br);
			strip.set_br(br);
			break;
		}
		case 'e':
		{
			byte pos;
			parseIn_int(pos);
			strip.set_effect(pos);
			break;
		}
		case 'u':
		{
			parseIn_int(udp_state);
			break;
		}
		default:
			break;
		}
		break;
	}
	case 'x':
	{
		key = input_str[0];
		input_str++;
		switch (key)
		{
		case 'r':
		{
			LOG_USB_STARTUP("reset\n");
			ESP.restart();
			break;
		}
		case 'f':
		{
			LOG_USB_STARTUP("full reset\n");
			EEPROM.write(0, 0x80);
			EEPROM.commit();
			ESP.restart();
			break;
		}
		case 'p':
		{
			Serial.println("print mem");
			Serial.printf("init bit: %d\n", EEPROM.read(0) >> 7);
			Serial.printf("obj data count: %d\n", EEPROM.read(0) & 0b0111'1111);
			for (int i = 0; i < OBJ_DATA_CAP; i++)
			{
				uint16_t addr;
				EEPROM.get(1 + i * 2, addr);
				Serial.printf("%d-%d|", addr >> 12, addr & 0x0FFF);
			}
			Serial.print('\n');

			for (int i = (OBJ_DATA_CAP + 1) * 2 + 1; i < 64 * 64; i++)
			{
				if ((i - (OBJ_DATA_CAP + 1) * 2 + 1) % 64 == 0) Serial.print('\n');
				byte value = EEPROM.read(i);
				if (value < 0x10)
					Serial.print("0");
				Serial.print(value, HEX);
				Serial.print(" ");
			}
			Serial.print('\n');
			break;
		}
		case 't':
		{
			unsigned long now_millis_time = millis();
			byte now_weekday = StartTimeInfo::start_weekday + (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) / 86400UL;
			unsigned long now_daytime = (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) % 86400UL;
			Serial.printf("start_day_time: %lu\n ", StartTimeInfo::start_day_time);
			Serial.printf("start_epoch_time: %lu\n ", StartTimeInfo::start_epoch_time);
			Serial.printf("start_millis_time: %lu\n ", StartTimeInfo::start_millis_time);
			Serial.printf("start_weekday: %d\n ", StartTimeInfo::start_weekday);
			Serial.printf("now_millis_time: %lu\n ", now_millis_time);
			Serial.printf("now_weekday: %d\n ", now_weekday);
			Serial.printf("now_daytime: %lu\n ", now_daytime);
			break;
		}
		default:
			break;
		}
		break;
	}
	case 'e':
	{
		strip.parse(input_str);
		break;
	}
	case 't':
	{
		timerHandler.parse(input_str);
		break;
	}
	default:
		break;
	}
}

void handleGetTimers()
{
	String output;
	JsonDocument doc;
	JsonArray memTimers = doc.createNestedArray("memTimers");
	timerHandler.getMemJsonAll(memTimers);
	JsonArray activeTimers = doc.createNestedArray("activeTimers");
	timerHandler.getActiveJsonAll(activeTimers);
	doc["millisNow"] = String(millis());
	serializeJson(doc, output);
	server.send(200, "application/json", output);
}

void handleGetEffectOption()
{
	String output;
	JsonDocument doc;
	JsonObject staticFields = doc.createNestedObject("staticFields");
	staticFields["state"] = String(timerHandler.getActiveState(0));
	staticFields["UDP"] = String(udp_state);
	staticFields["name"] = strip.get_effect_name();
	staticFields["br"] = strip.get_br();
	staticFields["maxBr"] = strip.get_max_br();
	JsonArray blocks = doc.createNestedArray("effectBlocks");
	strip.getEffectJSON(blocks);
	serializeJson(doc, output);
	server.send(200, "application/json", output);
}