#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <EncButton.h>
#include "Config.h"
#include "log.h"
#include "Strip_class.h"
#include "TimerHandler.h"
#include "Strip_class.h"

class InputHandler
{
public:
	static void setupInputs(TimerHandler* timerHandler, Strip* strip);

	static void tick();

	static void parseCommand(String commands);
	static void parseSingleCommand(const char* input_str);

private:
	static void handleUDP();
	static void handleRoot();
	static void handleCommand();
	static void handleGetTimers();
	static void handleGetEffectOption();
	static void handleEncoder();
	static void handleSerial();

	static IRAM_ATTR void enc_click();
	static bool udp_state;
	static bool enc_state;

	static int br;

	static EncButtonT<ENC_S1, ENC_S2, ENC_KEY> encoder;

	static ESP8266WebServer server;
	static WiFiUDP Udp;

	static TimerHandler* _timerHandler;
	static Strip* _strip;
};

#endif