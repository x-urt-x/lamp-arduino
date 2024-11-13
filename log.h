#ifndef LOG_CFG_H
#define LOG_CFG_H

#define LOG_USB_ENABLE

#ifdef LOG_USB_ENABLE

#define LOG_USB_STARTUP_ENABLE
//#define LOG_USB_BR_ENABLE
//#define LOG_USB_COLOR_MAP_ENABLE
#define LOG_USB_ENC_ENABLE
//#define LOG_USB_FIRE_ENABLE
//#define LOG_USB_SWITCH_ENABLE
#define LOG_USB_UPD_ENABLE

#endif // LOG_USB_ENABLE



//use only in this file
#define LOG_USB_MATR(matr)						\
{												\
	Serial.printf("%s\n", #matr);				\
	for (int i = 0; i < MATR_SIZE; i++)				\
	{											\
		for (int j = 0; j < MATR_SIZE; j++)			\
			Serial.printf("%d ", matr[j][i]);	\
		Serial.printf("\n");					\
	}											\
}



#ifdef LOG_USB_STARTUP_ENABLE
#define LOG_USB_STARTUP(format, ...) Serial.printf(format, ##__VA_ARGS__)
#else
#define LOG_USB_STARTUP(format, ...) ;
#endif

#ifdef LOG_USB_BR_ENABLE
#define LOG_USB_BR(format, ...) Serial.printf(format, ##__VA_ARGS__)
#else
#define LOG_USB_BR(format, ...) ;
#endif

#ifdef LOG_USB_COLOR_MAP_ENABLE
#define LOG_USB_COLOR_MAP(format, ...) Serial.printf(format, ##__VA_ARGS__)
#else
#define LOG_USB_COLOR_MAP(format, ...) ;
#endif

#ifdef LOG_USB_ENC_ENABLE
#define LOG_USB_ENC(format, ...) Serial.printf(format, ##__VA_ARGS__)
#else
#define LOG_USB_ENC(format, ...) ;
#endif

#ifdef LOG_USB_FIRE_ENABLE
#define LOG_USB_FIRE(format, ...) Serial.printf(format, ##__VA_ARGS__)
#define LOG_USB_FIRE_SIDE_MAP(_side_coef, _dic_map)		\
{														\
Serial.printf("%s %s\n",#_side_coef, #_dic_map);		\
for (int i = 0; i < MATR_SIZE; i++)							\
{														\
	Serial.printf("%d\t", _side_coef[i]);				\
	for (int j = 0; j < MATR_SIZE; j++)						\
		Serial.printf("%d ", _dic_map[j][i]);			\
	Serial.printf("\n");								\
	}													\
}
#define LOG_USB_FIRE_TEMP() LOG_USB_MATR(_temp_map)
#else
#define LOG_USB_FIRE(format, ...) ;
#define LOG_USB_FIRE_SIDE_MAP(_side_coef, _dic_map) ;
#define LOG_USB_FIRE_TEMP(_temp_map) ;
#endif

#ifdef LOG_USB_SWITCH_ENABLE
#define LOG_USB_SWITCH(format, ...) Serial.printf(format, ##__VA_ARGS__)
#else
#define LOG_USB_SWITCH(format, ...) ;
#endif

#ifdef LOG_USB_UPD_ENABLE
#define LOG_USB_UPD(format, ...) Serial.printf(format, ##__VA_ARGS__)
#else
#define LOG_USB_UPD(format, ...) ;
#endif

#endif