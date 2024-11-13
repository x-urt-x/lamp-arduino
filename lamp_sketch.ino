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
int enc_select = 1;
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

	if (Serial.available() > 0) {
		char key0 = Serial.read();
		LOG_USB_SWITCH("key0 - %c\n", key0);
		switch (key0)
		{
		case 'e':
		{
			char key1 = Serial.read();
			LOG_USB_SWITCH("key1 - %c\n", key1);
			switch (key1)
			{
			case 'd':
				strip.set_effect_strip_update_delay_time(Serial.readStringUntil('\n').toInt());
				break;
			case 'b':
				strip.set_br(Serial.readStringUntil('\n').toInt());
				break;
			case 'c':
				strip.set_effect_br_cutoff_bound(Serial.readStringUntil('\n').toInt());
				break;
			case 's':
				strip.set_effect_step(Serial.readStringUntil('\n').toInt());
				break;
			default:
				break;
			}
			break;
		}
		case 'c':
		{
			byte pos = Serial.readStringUntil(' ').toInt();
			Color_str col;
			col.set(strtoul(Serial.readStringUntil('\n').c_str(), nullptr, 16));
			strip.set_color(col, pos);
		}
		case 'p':
		{
			char key1 = Serial.read();
			LOG_USB_SWITCH("key1 - %c\n", key1);
			switch (key1)
			{
			case 'n':
			{
				int count = strip.get_preset_count();
				if (count > 0)
				{
					const String* names = strip.get_preset_names();
					for (int i = 0; i < count; i++)
					{
						Serial.printf("%d - %s\n", i, names[i]);
					}
				}
				else
				{
					Serial.printf("no preset\n");
				}
				break;
			}
			case 's':
			{
				strip.set_preset(Serial.readStringUntil('\n').toInt());
				break;
			}
			default:
				break;
			}
			break;
		}
		case 'r':
		{
			char key1 = Serial.read();
			LOG_USB_SWITCH("key1 - %c\n", key1);
			switch (key1)
			{
			case 'm':
			{
				byte pos = Serial.readStringUntil(' ').toInt();
				strip.set_rainbow_state(Serial.readStringUntil('\n').toInt(), pos);
				break;
			}
			case 's':
			{
				byte pos = Serial.readStringUntil(' ').toInt();
				strip.set_rainbow_step(Serial.readStringUntil('\n').toInt(), pos);
				break;
			}
			default:
				break;
			}
			break;
		}
		case 's':
		{
			Serial.print(strip.get_status().c_str());
			break;
		}
		case 'm':
		{
			strip.set_effect(Serial.readStringUntil('\n').toInt());
			break;
		}
		default:
			break;
		}
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
		// Обработка POST запроса
		//String data = server.arg("plain");

		//int state_ind = data.indexOf("state=") + 6;
		//int brightness_ind = data.indexOf("brightness=") + 11;
		//int color_ind = data.indexOf("color=") + 6;
		//int effect_ind = data.indexOf("effect=") + 7;
		//int strip_update_delay_time_ind = data.indexOf("strip_update_delay_time=") + 24;
		//int effect_speed_ind = data.indexOf("effect_speed=") + 13;


		//if (state_ind != 5)
		//	changeState(data.substring(state_ind, state_ind + 1) == "t");
		//if (brightness_ind != 10)
		//	stripColor.br = data.substring(brightness_ind, brightness_ind + 3).toInt() / 255.0;
		//if (color_ind != 5)
		//	stripColor.set(0xFF000000 | strtoul(data.substring(color_ind + 1, color_ind + 7).c_str(), nullptr, 16));
		//if (effect_ind != 6)
		//	chooseEffect(data.substring(effect_ind, effect_ind + 2).toInt());
		//if (strip_update_delay_time_ind != 23)
		//	strip_update_delay_time = data.substring(strip_update_delay_time_ind, data.indexOf("&", strip_update_delay_time_ind)).toInt();
		//if (effect_speed_ind != 12)
		//	effect_speed = data.substring(effect_speed_ind, data.indexOf("&", effect_speed_ind)).toInt();
		strip.tick(true);
		server.send(200, "text/plain", "Data received successfully");
	}
}