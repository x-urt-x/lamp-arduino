#include "Strip_class.h"

void Strip::eff_singleColor() {
  fill(_main_color.get());
}

void Strip::eff_halfSingleColor() {
  for (int i = 0; i < 50; i++) {
    _leds_arr[i].r = _main_color.r;
    _leds_arr[i].g = _main_color.g;
    _leds_arr[i].b = _main_color.b;
  }
}