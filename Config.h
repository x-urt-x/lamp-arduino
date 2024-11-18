#ifndef CONFIG_H
#define CONFIG_H

#define EB_NO_FOR
#define EB_NO_CALLBACK
#define EB_DEB_TIME 50     // таймаут гашения дребезга кнопки (кнопка)
#define EB_CLICK_TIME 500  // таймаут ожидания кликов (кнопка)
#define EB_HOLD_TIME 1500  // таймаут удержания (кнопка)
#define EB_STEP_TIME 200   // таймаут импульсного удержания (кнопка)
#define EB_FAST_TIME 30    // таймаут быстрого поворота (энкодер)

//#define MATR10x10
#define MATR16x16

#ifdef MATR10x10
#define ENC_S1 D0
#define ENC_S2 D5
#define ENC_KEY D6
#define inbr_cur_PIN A0
#define GREEN_PIN D1
#define RED_PIN D2
#define ORNG_PIN D3
#define STRIP_PIN D4

#define MATR_SIZE 10
#else
#ifdef MATR16x16
#define ENC_S1 D5
#define ENC_S2 D0
#define ENC_KEY D7
#define inbr_cur_PIN A0
#define RED_PIN D4
#define GREEN_PIN D3
#define STRIP_PIN D6

#define MATR_SIZE 16
#endif // MATR16x16
#endif

#define MATR_LEN (MATR_SIZE*MATR_SIZE)
#endif