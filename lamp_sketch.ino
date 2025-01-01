#include "Config.h"
#include "log.h"
#include "Strip_class.h"
#include "TimerHandler.h"

#include <EncButton.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "Pass.h" //  define in this file SSID and PASS of ur wifi

const char* ssid = SSID;
const char* password = PASS;
const unsigned int localPort = 8888;

IPAddress local_ip(192, 168, 1, 42);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);
WiFiUDP Udp;
inline void handleUDP();

bool udp_enable = false;

EncButtonT<ENC_S1, ENC_S2, ENC_KEY> encoder;
Strip strip(MATR_LEN, STRIP_PIN);
TimerHandler timerHandler;

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
	pinMode(MOSFET_PIN, OUTPUT);
	digitalWrite(MOSFET_PIN, HIGH);

	WiFi.config(local_ip, gateway, subnet);
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

	if (!SPIFFS.begin()) {
		LOG_USB_STARTUP("Failed to mount file system\n");
		return;
	}

	server.on("/", HTTP_GET, handleRoot);
	server.on("/submit", HTTP_POST, handleCommand);
	server.on("/get-effect-option", HTTP_GET, handleGetEffectOption);
	server.on("/udp-state", HTTP_POST, handleSetUDP);

	Udp.begin(localPort);
	server.begin();

	LOG_USB_STARTUP("UDP server on port %d\n", localPort);

	digitalWrite(RED_PIN, LOW);
	timerHandler.addTimer(new EffectEventTimer());
	strip.parseSingle("m 0");
	strip.parseSingle("eb 1000");
	strip.tick();
}

int br = 5000;
volatile bool is_enc = false;

void loop() {
	server.handleClient();
	if (udp_enable)
	{
		handleUDP();
	}
	encoder.tick();
	if (encoder.turn()) {
		if (encoder.pressing())
		{
			br += (encoder.fast() ? 500 : 100) * encoder.dir();
		}
		else
		{
			br += (encoder.fast() ? 25 : 1) * encoder.dir();
		}
		//inbr_mid += (encoder.pressing() ? 5 : 1) * encoder.dir();

		if (br > 247335)
			br = 247335;
		if (br < 0)
			br = 0;
		LOG_USB_ENC("%d\n", br);
		strip.parseSingle(("eb " + String(br)).c_str());
		strip.tick();
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

void handleGetEffectOption()
{
	String output;
	serializeJson(strip.getJSON(udp_enable, timerHandler.getState(0)), output);
	server.send(200, "application/json", output);
}

void handleCommand()
{
	char* data = const_cast<char*>(server.arg("plain").c_str());
	if (data[0] == 't')
	{
		data++;
		char key = data[0];
		data++;
		switch (key)
		{
		case 's':
		{
			byte pos = atoi(data);
			while (data[0] != ' ') data++;
			timerHandler.setState(atoi(data), pos);
			if (atoi(data) == 0)
			{
				digitalWrite(MOSFET_PIN, LOW);
				//WiFi.forceSleepBegin();
			}
			else
			{
				//WiFi.forceSleepWake();
				digitalWrite(MOSFET_PIN, HIGH);
			}
			break;
		}
		case 'd':
		{
			byte pos = atoi(data);
			if (pos == 0) break;
			while (data[0] != ' ') data++;
			timerHandler.deleteTimer(pos);
			break;
		}
		case 'a':
		{
			timerHandler.addTimer(data);
			break;
		}
		default:
			break;
		}
	}
	else
	{
		strip.parse(data);
	}
	strip.tick();
	server.send(200, "text/plain", "Data received successfully");
	//LOG_USB_HTTP("server.send\n");
}

void handleSetUDP()
{
	const char* data = server.arg("plain").c_str();
	if (data[0] == '1')
	{
		udp_enable = true;
	}
	else
	{
		udp_enable = false;
	}
	LOG_USB_UPD("udp state = %d\n", udp_enable ? 1 : 0);
	server.send(200, "text/plain", "Data received successfully");
}