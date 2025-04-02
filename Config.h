#ifndef CONFIG_H
#define CONFIG_H

#define EB_NO_FOR
#define EB_NO_CALLBACK
#define EB_DEB_TIME 50
#define EB_CLICK_TIME 500 
#define EB_HOLD_TIME 1500
#define EB_STEP_TIME 200
#define EB_FAST_TIME 30

#define GTMOFFSET 3600+3600

#define OBJ_DATA_CAP 30
#define OBJ_DATA_MAX_ADDR 3000
#define BRLIMIT_ADRR 4093

#define UDP_PORT 8888

#define ENC_KEY D0
#define MOSFET_PIN D8
#define STRIP_PIN D6
#define ENC_S1 D5
#define ENC_S2 D7
#define RED_PIN D4
#define GREEN_PIN D3

//#define MATR10x10
#define MATR16x16


#ifdef MATR10x10
#define MATR_SIZE 10

#else

#ifdef MATR16x16
#define MICRO_PIN A0
#define MATR_SIZE 16

#endif // MATR16x16
#endif

#define MATR_LEN (MATR_SIZE*MATR_SIZE)
#endif