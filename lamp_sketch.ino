#include "Config.h"
#include "log.h"
#include "Strip_class.h"

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

IPAddress local_ip(192, 168, 1, 42);     // Статический IP, который вы хотите установить
IPAddress gateway(192, 168, 1, 1);        // Шлюз (обычно это IP маршрутизатора)
IPAddress subnet(255, 255, 255, 0);			// Маска подсети

ESP8266WebServer server(80);
WiFiUDP Udp;
inline void handleUDP();

EncButtonT<ENC_S1, ENC_S2, ENC_KEY> encoder;
Strip strip(MATR_LEN, STRIP_PIN);

void setup() {
	randomSeed(analogRead(0));
#ifdef LOG_USB_ENABLE
	Serial.begin(115200);
#endif // LOG_USB_ENABLE
	LOG_USB_STARTUP("\nstart\n");
	strip.begin();
	strip.fill(strip.Color(0, 0, 0));
	delay(0);
	strip.show();
#ifdef MATR10x10
	pinMode(inbr_cur_PIN, INPUT);
	pinMode(GREEN_PIN, OUTPUT);
	digitalWrite(GREEN_PIN, LOW);
	pinMode(ORNG_PIN, OUTPUT);
	digitalWrite(ORNG_PIN, LOW);
#endif // MATR10x10

	pinMode(RED_PIN, OUTPUT);
	digitalWrite(RED_PIN, LOW);

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

	Udp.begin(localPort);
	server.begin();

	LOG_USB_STARTUP("UDP server on port %d\n", localPort);

	digitalWrite(RED_PIN, LOW);

	strip.set_effect(0);
	strip.set_br(100);
	strip.tick(true);
}

int br = 5000;
volatile bool is_enc = false;

void loop() {
	server.handleClient();
	handleUDP();
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
		strip.set_br(br);
		strip.tick(true);
	}

	while (Serial.available() > 0)
	{
		strip.parse(Serial.readStringUntil('\n').c_str());
		strip.tick(true);
	}
	strip.tick();
}

inline void handleUDP() {
	if (Udp.parsePacket()) {
		byte inbytes[3];
		Udp.read(inbytes, 3);
		strip.set_color(Color_str(inbytes[0], inbytes[1], inbytes[2]), 0);
		strip.tick(true);
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
	JsonDocument doc;
	doc["name"] = strip.get_effect_name();
	JsonArray blocks = doc.createNestedArray("blocks");

	JsonObject effectable = blocks.createNestedObject();
	effectable["block_name"] = "effectable";
	effectable["strip_update_delay_time"] = strip.get_strip_update_delay_time();
	effectable["br"] = strip.get_br();
	effectable["br_cutoff_bound"] = strip.get_br_cutoff_bound();

	if (int len = strip.get_color_len())
	{
		JsonObject colorable = blocks.createNestedObject();
		colorable["block_name"] = "colorable";
		//colorable["color_len"] = String(len);
		Color_str* colors = strip.get_colors();
		char hex[8] = {};
		for (int i = 0; i < len; i++)
		{
			JsonObject field = colorable.createNestedObject(String(i));
			snprintf(hex, sizeof(hex), "#%02X%02X%02X", colors[i].r, colors[i].g, colors[i].b);
			field["Color"] = String(hex);
		}
	}
	if (int len = strip.get_preset_count())
	{
		JsonObject preseteble = blocks.createNestedObject();
		preseteble["block_name"] = "preseteble";
		//preseteble["preseteble_len"] = String(len);
		const String* presets = strip.get_preset_names();
		for (int i = 0; i < len; i++)
		{
			JsonObject field = preseteble.createNestedObject(String(i));
			field["Name"] = String(presets[i]);
		}
	}
	if (int len = strip.get_rainbow_len())
	{
		JsonObject rainbowble = blocks.createNestedObject();
		rainbowble["block_name"] = "rainbowble";
		//rainbowble["rainbow_len"] = String(len);
		bool* states = strip.get_rainbow_states();
		int* steps = strip.get_rainbow_steps();
		for (int i = 0; i < len; i++)
		{
			JsonObject field = rainbowble.createNestedObject(String(i));
			field["State"] = String(states[i]);
			field["Step"] = String(steps[i]);
		}
	}

	String output;
	serializeJson(doc, output);
	server.send(200, "application/json", output);
}

void handleCommand()
{
	char* data = const_cast<char*>(server.arg("plain").c_str());
	LOG_USB_HTTP("%s\n", data);
	char* pch;
	pch = strtok(data, "\n");
	while (pch != NULL)
	{
		strip.parse(pch);
		pch = strtok(NULL, "\n");
	}

	strip.tick(true);
	server.send(200, "text/plain", "Data received successfully");
	//LOG_USB_HTTP("server.send\n");
}