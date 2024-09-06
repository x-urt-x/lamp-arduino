#ifndef EFFECT_FIRE_H
#define EFFECT_FIRE_H

#include <Arduino.h>
#include "Structures.h"
#include "IEffect.h"

class Effect_fire : public IEffect
{
public:
  Effect_fire(Color_str* leds_arr, Color_str* main_color, Color_str* second_color);
  void setup() override;
  void make_frame() override;
  void set_step(int step) override;
  const byte* get_cutoff_order() override;
  const byte* get_cutoff_imm() override;
  byte get_cutoff_order_len() override;
  byte get_cutoff_imm_len() override;

private:
  static uint32_t fireColor(byte temp);


  Color_str *_leds_arr, *_main_color, *_second_color;

  byte _fire_temp[10][10];
  byte _eff_fire_center; 
  byte _eff_fire_center_dec;
  int _step;

  static const byte cutoff_order[98];
  static const byte cutoff_imm[2];
  static const int cutoff_order_len = 98;
  static const int cutoff_imm_len = 2;
};

#endif