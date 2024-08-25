#ifndef STRIP_CLASS_H
#define STRIP_CLASS_H

#define led_m(x,y) _leds_arr[y*10+(y%2==0?x:9-x)]
#define byte_u(val) ((val) > 255 ? 255 : (val)) 
#define byte_d(val) ((val) < 0 ? 0 : (val))
#define byte_round_up(val) ((val)==byte(val)?(val):byte(val+1)) 

//effects numbers
#define EFF_SINGLE 0
#define EFF_FIRE 1

#include <Adafruit_NeoPixel.h>
#include "log.h"
#include "Structures.h"


class Strip : public Adafruit_NeoPixel
{
public:
	Strip(uint16_t n, int16_t p = 6); //n=100
	void apply_br();
	void set_br(int br);
	void set_main_color(Color_str color);
	float _br_max_gain;
	int _br_cutoff_bound;
  void set_effect(byte num);
  void frame();
  
private:
  void set_cutoff_order(const uint8_t* cutoff_order, uint8_t cutoff_order_len, const uint8_t* cutoff_imm, uint8_t cutoff_imm_len);
  void(Strip::*_effect_cur)(); //указатель на текущий эффект

  //методы для эффектов
  static uint32_t fireColor(byte temp);
  void eff_fire();
  void eff_fire_setup();

  void eff_singleColor();
  void eff_halfSingleColor();


  byte _fire_temp[10][10];
  byte _eff_fire_center; 
  byte _eff_fire_center_dec;

  const uint8_t* _cur_cutoff_order;
  const uint8_t*  _cur_cutoff_imm;
  byte _cur_cutoff_order_len, _cur_cutoff_imm_len, _cur_cutoff_units;

	uint8* _pixels; 
	byte _led_amount;
	Color_str _main_color, _second_color;
	Color_str* _leds_arr; //array of leds color
	int _br_vir; //virtual brightness up to 255*97=24735
};
#endif


