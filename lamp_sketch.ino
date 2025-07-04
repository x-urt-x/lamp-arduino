#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <FS.h>

#include "Config.h"
#include "log.h"
#include "StartTimeInfo.h"
#include "TimerHandler.h"
#include "InputHandler.h"

#include "Pass.h" //  define in this file SSID and PASS of ur wifi

const char* ssid = SSID;
const char* password = PASS;
#ifdef MATR16x16
const IPAddress local_ip(192, 168, 1, 43);
#else
#ifdef MATR10x10
const IPAddress local_ip(192, 168, 1, 41);
#endif // MATR10x10
#endif

const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);
const IPAddress primaryDNS(8, 8, 8, 8);
const IPAddress secondaryDNS(8, 8, 4, 4);

Strip strip(MATR_LEN, STRIP_PIN);
TimerHandler timerHandler;

void setup() {
	randomSeed(analogRead(0));

#ifdef LOG_USB_ENABLE
	Serial.begin(115200);
#endif

	LOG_USB_STARTUP("\nstart\n");
	IEventTimer::obj = &strip;
	strip.begin();
	strip.fill(strip.Color(0, 0, 0));
	delay(0);
	strip.show();

	pinMode(RED_PIN, OUTPUT);
	digitalWrite(RED_PIN, LOW);
	pinMode(GREEN_PIN, OUTPUT);
	digitalWrite(GREEN_PIN, LOW);
	pinMode(MOSFET_PIN, OUTPUT);
	digitalWrite(MOSFET_PIN, HIGH);
	
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
		EEPROM.put(BRLIMIT_ADRR, (uint16_t)0xFFFF);
		EEPROM.commit();
	}
	EEPROM.get(BRLIMIT_ADRR, strip.br_limit);
	LOG_USB_STARTUP("br_limit = %d\n", strip.br_limit);
	if (!SPIFFS.begin()) {
		LOG_USB_STARTUP("Failed to mount file system\n");
		return;
	}

	WiFi.config(local_ip, gateway, subnet, primaryDNS, secondaryDNS);
	WiFi.mode(WIFI_STA);
	WiFi.setAutoReconnect(true);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		digitalWrite(RED_PIN, HIGH);
		delay(250);
		digitalWrite(RED_PIN, LOW);
		delay(250);
		LOG_USB_STARTUP("Connecting to WiFi...\n");
	}
	LOG_USB_STARTUP("Connected! IP address: %s\n", WiFi.localIP().toString().c_str());

	StartTimeInfo::saveTime();
	InputHandler::setupInputs(&timerHandler, &strip);

	digitalWrite(RED_PIN, LOW);
	timerHandler.addActiveTimer(new EffectEventTimer());
	timerHandler.addActiveAllFromMem();
	strip.set_effect(0);
	strip.set_br(0);
#ifdef MATR10x10
	InputHandler::parseSingleCommand("ec 0 b54200");
	InputHandler::parseSingleCommand("ebc 8");
#endif // MATR10x10
	strip.tick();
#ifdef LOG_USB_BR_ENABLE
	InputHandler::parseSingleCommand("ebd 1000000");
#endif // LOG_USB_BR_ENABLE
}

//uint last = 0;
void loop() {
	InputHandler::tick();
	delay(0);
	timerHandler.tickAll();
	//uint now = micros();
	//Serial.println(now-last);
	//last = now;
}