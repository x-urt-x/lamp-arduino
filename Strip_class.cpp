#include "Strip_class.h"

static const uint8_t center_cutoff_order[] = { 0,90,1,91,2,92,3,93,4,94,5,95,6,96,7,97,8,98,9,99,10,80,11,81,12,82,13,83,14,84,15,85,16,86,17,87,18,88,19,89,20,70,21,71,22,72,23,73,24,74,25,75,26,76,27,77,28,78,29,79,30,60,31,61,32,62,33,63,34,64,35,65,36,66,37,67,38,68,39,69,40,50,49,59,41,51,48,58,42,52,47,57,43,53,46,56};
static const uint8_t center_cutoff_imm[] = { 44,45,54,55 };

static const uint8_t buttom_cutoff_order[] = { 0,9,1,8,2,7,3,6,4,5,10,19,11,18,12,17,13,16,14,15,20,29,21,28,22,27,23,26,24,25,30,39,31,38,32,37,33,36,34,35,40,49,41,48,42,47,43,46,44,45,50,59,51,58,52,57,53,56,54,55,60,69,61,68,62,67,63,66,64,65,70,79,71,78,72,77,73,76,74,75,80,89,81,88,82,87,83,86,84,85,90,99,91,98,92,97,93,96};
static const uint8_t buttom_cutoff_imm[] = { 94,95};

Strip::Strip(uint16_t n, int16_t p)
  : Adafruit_NeoPixel(n, p),
    _main_color(50, 50, 50),
    _second_color(50, 50, 50),
    _leds_arr(reinterpret_cast<Color_str*>(getPixels())),
    _pixels(getPixels()),
    _led_amount(n),
    _br_cutoff_bound(10)
{
  set_effect(EFF_SINGLE);
}


void Strip::set_main_color(Color_str color) {
  _main_color = color;
}

void Strip::frame()
{
  (this->*_effect_cur)();
  apply_br();
}

void Strip::set_effect(byte num)
{
  switch(num)
  {
    case EFF_SINGLE:
      _effect_cur = &Strip::eff_singleColor;
      set_cutoff_order(center_cutoff_order, sizeof(center_cutoff_order), center_cutoff_imm, sizeof(center_cutoff_imm));
      break;
    case EFF_FIRE:
      eff_fire_setup();
      _effect_cur = &Strip::eff_fire;
      set_cutoff_order(buttom_cutoff_order, sizeof(buttom_cutoff_order), buttom_cutoff_imm, sizeof(buttom_cutoff_imm));
      break;
  }
}

void Strip::set_cutoff_order(const uint8_t* cutoff_order, uint8_t cutoff_order_len, const uint8_t* cutoff_imm, uint8_t cutoff_imm_len)
{
  _cur_cutoff_order = cutoff_order;
  _cur_cutoff_imm = cutoff_imm;
  _cur_cutoff_order_len = cutoff_order_len;;
  _cur_cutoff_imm_len = cutoff_imm_len;
  _cur_cutoff_units = _cur_cutoff_order_len + (_cur_cutoff_imm_len==0?0:1);
}

