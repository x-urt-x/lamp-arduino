#ifndef LOG_CFG_H
#define LOG_CFG_H

#define LOG_USB_STARTUP_ENABLE
//#define LOG_USB_BR_ENABLE
//#define LOG_USB_COLOR_MAP_ENABLE
#define LOG_USB_ENC_ENABLE
//#define LOG_USB_FIRE_ENABLE


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
#else
    #define LOG_USB_FIRE(format, ...) ;
#endif
                                      
#endif