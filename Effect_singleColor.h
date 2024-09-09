#ifndef EFFECT_SINGLECOLOR_H
#define EFFECT_SINGLECOLOR_H

#include <Arduino.h>
#include "Structures.h"
#include "IEffect.h"


class Effect_singleColor : public IEffect
{
public:
  Effect_singleColor(Color_str* leds_arr, Color_str* main_color, uint* strip_update_delay_time);
  void setup() override;
  void make_frame() override;
  void set_step(int step) override;
  const byte* get_cutoff_order() override;
  const byte* get_cutoff_imm() override;
  byte get_cutoff_order_len() override;
  byte get_cutoff_imm_len() override;
  int get_preset_count() override;
  const String* get_preset_names() override;
  void set_preset(int num) override;

private:
  Color_str *_leds_arr, *_main_color;
  int _step;
  uint* _strip_update_delay_time;

  static const byte _cutoff_order[96];
  static const byte _cutoff_imm[4];
  static const int _cutoff_order_len = 96;
  static const int _cutoff_imm_len = 4;
  static const String _preset_names[0];
  static const int _preset_len = 0;
};
#endif