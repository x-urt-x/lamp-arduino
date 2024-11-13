#include "Config.h"
#include "log.h"
#include "Strip_class.h"

#include <EncButton.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <WiFiUdp.h>
#include "Pass.h" //  define in this file SSID and PASS of ur wifi

const char* ssid = SSID;
const char* password = PASS;
const unsigned int localPort = 8888;

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
	server.on("/", HTTP_POST, handleRoot);

	Udp.begin(localPort);
	server.begin();

	LOG_USB_STARTUP("UDP server on port %d\n", localPort);

	digitalWrite(RED_PIN, LOW);

	strip.set_effect(0);
	strip.set_br(5000);
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
	if (server.method() == HTTP_GET) {
		File file = SPIFFS.open("/index.html", "r");
		if (file) {
			server.streamFile(file, "text/html");
			file.close();
		}
		else {
			server.send(500, "text/plain", "Internal Server Error");
		}
	}
	else if (server.method() == HTTP_POST) {
		String data = server.arg("plain");
		strip.parse(data.c_str());
		strip.tick(true);
		server.send(200, "text/plain", "Data received successfully");
		LOG_USB_HTTP("%s",data.c_str());
	}
}