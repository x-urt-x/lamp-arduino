#ifndef STAR_TIME_INFO_H
#define STAR_TIME_INFO_H
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Config.h"

struct StartTimeInfo
{
	static void saveTime();

	static unsigned long start_epoch_time;
	static byte start_weekday;
	static unsigned long start_day_time;
	static unsigned long start_millis_time;

	static WiFiUDP NtpUdp;
	static NTPClient timeClient;
};
#endif