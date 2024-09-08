#include "Strip_class.h"
#include "Effect_singleColor.h"
#include "Effect_fire.h"

Strip::Strip(uint16_t n, int16_t p)
  : Adafruit_NeoPixel(n, p),
    _main_color(50, 50, 50),
    _second_color(50, 50, 50),
    _leds_arr(reinterpret_cast<Color_str*>(getPixels())),
    _pixels(getPixels()),
    _led_amount(n),
    _br_cutoff_bound(10)
{
  set_effect(EFF_FIRE);
}


void Strip::set_main_color(Color_str color) {
  _main_color = color;
}

void Strip::set_second_color(Color_str color)
{
    _second_color = color;
}

void Strip::frame()
{
    LOG_USB_STARTUP("%d %d %d", _main_color.r, _main_color.g, _main_color.b);
    effect->make_frame();
    apply_br();
}

void Strip::set_effect(byte num)
{
  if(effect)
  {
    delete effect;
  }

  switch(num)
  {
    case EFF_SINGLE:
      effect = new Effect_singleColor(_leds_arr, &_main_color);
      break;
    case EFF_FIRE:
      effect = new Effect_fire(_leds_arr, &_main_color, &_second_color);
      break;
  }

  _cur_cutoff_order = effect->get_cutoff_order();
  _cur_cutoff_imm = effect->get_cutoff_imm();
  _cur_cutoff_order_len = effect->get_cutoff_order_len();
  _cur_cutoff_imm_len = effect->get_cutoff_order_len();
  _cur_cutoff_units = _cur_cutoff_order_len + (_cur_cutoff_imm_len == 0 ? 0 : 1);

  effect->setup();
}