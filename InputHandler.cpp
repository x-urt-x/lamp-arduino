#include "InputHandler.h"

void InputHandler::setupInputs(TimerHandler* timerHandler, Strip* strip)
{
	_timerHandler = timerHandler;
	_strip = strip;
	attachInterrupt(ENC_S2, enc_click, CHANGE);
	Udp.begin(UDP_PORT);
	server.begin();
	server.on("/", HTTP_GET, handleRoot);
	server.on("/submit", HTTP_POST, handleCommand);
	server.on("/get-effect-option", HTTP_GET, handleGetEffectOption);
	server.on("/get-timers", HTTP_GET, handleGetTimers);
}

void InputHandler::tick()
{
	server.handleClient();
	handleSerial();
	if (udp_state) handleUDP();
	if (enc_state) handleEncoder();
}

void InputHandler::handleUDP() {
	if (udp_state)
	{
		if (Udp.parsePacket()) {
			byte inbytes[3];
			Udp.read(inbytes, 3);
			_strip->udp_set_color(Color_str(inbytes[0], inbytes[1], inbytes[2]));
			_strip->tick();
			LOG_USB_UPD("UDP Color: %02X%02X%02X\n", inbytes[0], inbytes[1], inbytes[2]);
		}
	}
}

void InputHandler::handleRoot() {
	File file = SPIFFS.open("/index.html", "r");
	if (file) {
		server.streamFile(file, "text/html");
		file.close();
	}
	else {
		server.send(500, "text/plain", "Internal Server Error");
	}
}

void InputHandler::handleCommand()
{
	parseCommand(server.arg("plain"));
	server.send(200, "text/plain", "Data received successfully");
}

void InputHandler::parseCommand(String commands)
{
	while (commands.length() > 0)
	{
		int index = commands.indexOf('\n');
		if (index == -1)
		{
			parseSingleCommand(commands.c_str());
			_timerHandler->tickSingle(0, 0);
			break;
		}
		parseSingleCommand(commands.substring(0, index).c_str());
		_timerHandler->tickSingle(0, 0);
		commands = commands.substring(index + 1);
	}
}

void InputHandler::parseSingleCommand(const char* input_str)
{
#ifdef MATR10x10
	if (
		input_str[0] == 't' &&
		input_str[1] == 's' &&
		input_str[2] == ' ' &&
		input_str[3] == '0' &&
		input_str[4] == ' ' &&
		input_str[5] == '0'
		)
	{
		_strip->fill(_strip->Color(0, 0, 0));
		delay(0);
		_strip->show();
	}
#endif 
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
			_strip->apply_default_option();
			break;
		}
		case 'b':
		{
			int br;
			parseIn_int(br);
			_strip->set_br(br);
			break;
		}
		case 'e':
		{
			byte pos;
			parseIn_int(pos);
			_strip->set_effect(pos);
			break;
		}
		case 'u':
		{
			parseIn_int(udp_state);
			break;
		}
		case 'l':
		{
			bool save;
			parseIn_int(save);
			int br_limit;
			parseIn_int(br_limit);
			if (br_limit > 0xFFFF) br_limit = 0xFFFF;
			_strip->br_limit = br_limit;
			if (save)
			{
				EEPROM.put(BRLIMIT_ADRR, _strip->br_limit);
				EEPROM.commit();
			}
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
			LOG_USB_X("print mem\n");
			LOG_USB_X("init bit: %d\n", EEPROM.read(0) >> 7);
			LOG_USB_X("obj data count: %d\n", EEPROM.read(0) & 0b0111'1111);
			for (int i = 0; i < OBJ_DATA_CAP; i++)
			{
				uint16_t addr;
				EEPROM.get(1 + i * 2, addr);
				LOG_USB_X("%d-%d|", addr >> 12, addr & 0x0FFF);
			}
			LOG_USB_X("\n");

			for (int i = (OBJ_DATA_CAP + 1) * 2 + 1; i < 64 * 64; i++)
			{
				if ((i - (OBJ_DATA_CAP + 1) * 2 + 1) % 64 == 0) LOG_USB_X("\n");
				byte value = EEPROM.read(i);
				if (value < 0x10)
					LOG_USB_X("0");
				LOG_USB_X("0x%X ", value);
			}
			LOG_USB_X("\n");
			break;
		}
		case 't':
		{
			unsigned long now_millis_time = millis();
			byte now_weekday = StartTimeInfo::start_weekday + (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) / 86400UL;
			unsigned long now_daytime = (StartTimeInfo::start_day_time + (now_millis_time - StartTimeInfo::start_millis_time) / 1000UL) % 86400UL;
			LOG_USB_X("start_day_time: %lu\n ", StartTimeInfo::start_day_time);
			LOG_USB_X("start_epoch_time: %lu\n ", StartTimeInfo::start_epoch_time);
			LOG_USB_X("start_millis_time: %lu\n ", StartTimeInfo::start_millis_time);
			LOG_USB_X("start_weekday: %d\n ", StartTimeInfo::start_weekday);
			LOG_USB_X("now_millis_time: %lu\n ", now_millis_time);
			LOG_USB_X("now_weekday: %d\n ", now_weekday);
			LOG_USB_X("now_daytime: %lu\n ", now_daytime);
			break;
		}
		default:
			break;
		}
		break;
	}
	case 'e':
	{
		_strip->parse(input_str);
		break;
	}
	case 't':
	{
		_timerHandler->parse(input_str);
		break;
	}
	default:
		break;
	}
}

void InputHandler::handleGetTimers()
{
	String output;
	JsonDocument doc;
	JsonArray memTimers = doc.createNestedArray("memTimers");
	_timerHandler->getMemJsonAll(memTimers);
	JsonArray activeTimers = doc.createNestedArray("activeTimers");
	_timerHandler->getActiveJsonAll(activeTimers);
	doc["millisNow"] = String(millis());
	serializeJson(doc, output);
	server.send(200, "application/json", output);
}

void InputHandler::handleGetEffectOption()
{
	String output;
	JsonDocument doc;
	JsonObject staticFields = doc.createNestedObject("staticFields");
	staticFields["state"] = String(_timerHandler->getActiveState(0));
	staticFields["UDP"] = String(udp_state);
	staticFields["name"] = _strip->get_effect_name();
	staticFields["br"] = _strip->get_br();
	staticFields["maxBr"] = _strip->get_max_br();
	staticFields["brLimit"] = _strip->br_limit;
	JsonArray blocks = doc.createNestedArray("effectBlocks");
	_strip->getEffectJSON(blocks);
	serializeJson(doc, output);
	server.send(200, "application/json", output);
}

void InputHandler::handleEncoder()
{
	if (enc_state)
	{
		encoder.tick();
		if (encoder.hold())
			if (_timerHandler->getActiveState(0))
			{
				parseCommand("ts 0 0");
			}
			else
			{
				_timerHandler->parse("s 0 1");
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
			if (br > _strip->get_max_br())
				br = _strip->get_max_br();
			if (br < 0)
				br = 0;
			LOG_USB_ENC("%d\n", br);
			_strip->set_br(br);
			_timerHandler->tickSingle(0, 0);
		}
		if (encoder.timeout(5000))
		{
			enc_state = false;
			digitalWrite(RED_PIN, LOW);
		}
	}
}

void InputHandler::handleSerial()
{
#ifdef LOG_USB_ENABLE
	while (Serial.available() > 0)
	{
		_strip->parse(Serial.readStringUntil('\n').c_str());
		_timerHandler->tickSingle(0, 0);
	}
#endif
}

IRAM_ATTR void InputHandler::enc_click() {
	enc_state = true;
	br = _strip->get_br();
	digitalWrite(RED_PIN, HIGH);
}

bool InputHandler::udp_state = false;
bool InputHandler::enc_state = false;
int InputHandler::br = 1000;

EncButtonT<ENC_S1, ENC_S2, ENC_KEY> InputHandler::encoder = EncButtonT<ENC_S1, ENC_S2, ENC_KEY>();

ESP8266WebServer InputHandler::server = ESP8266WebServer(80);
WiFiUDP InputHandler::Udp = WiFiUDP();

TimerHandler* InputHandler::_timerHandler = nullptr;
Strip* InputHandler::_strip = nullptr;