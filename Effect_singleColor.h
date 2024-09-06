#ifndef EFFECT_SINGLECOLOR_H
#define EFFECT_SINGLECOLOR_H

#include <Arduino.h>
#include "Structures.h"
#include "IEffect.h"


class Effect_singleColor : public IEffect
{
public:
  Effect_singleColor(Color_str* leds_arr, Color_str* main_color);
  void setup() override;
  void make_frame() override;
  void set_step(int step) override;
  const byte* get_cutoff_order() override;
  const byte* get_cutoff_imm() override;
  byte get_cutoff_order_len() override;
  byte get_cutoff_imm_len() override;


private:
  Color_str *_leds_arr, *_main_color;

  int _step;

  static const byte cutoff_order[96];
  static const byte cutoff_imm[4];
  static const int cutoff_order_len = 96;
  static const int cutoff_imm_len = 4;
};
#endif