#include "StartTimeInfo.h"

unsigned long StartTimeInfo::start_epoch_time = 0;
byte StartTimeInfo::start_weekday = 0;
unsigned long StartTimeInfo::start_day_time = 0;
unsigned long StartTimeInfo::start_millis_time = 0;

void StartTimeInfo::saveTime()
{
	timeClient.begin();
	timeClient.forceUpdate();
	StartTimeInfo::start_millis_time = millis();
	StartTimeInfo::start_epoch_time = timeClient.getEpochTime();
	StartTimeInfo::start_weekday = timeClient.getDay();
	StartTimeInfo::start_day_time = timeClient.getSeconds() + timeClient.getMinutes() * 60 + timeClient.getHours() * 3600;
	timeClient.end();
}

WiFiUDP StartTimeInfo::NtpUdp = WiFiUDP();
NTPClient StartTimeInfo::timeClient = NTPClient(NtpUdp, "pool.ntp.org", GTMOFFSET);