#include "Strip_class.h"

Strip::Strip(uint16_t n, int16_t p)
  : Adafruit_NeoPixel(n, p),
    _main_color(50, 50, 50),
    _second_color(50, 50, 50),
    _leds_arr(reinterpret_cast<Color_str*>(getPixels())),
    _pixels(getPixels()),
    _led_amount(n),
    _br_cutoff_bound(10) {
}

void Strip::apply_br() {
  int _led_active_count1 = 100;
  float br_add = 0;
  float br_gain = _led_amount / _led_active_count;
  //Serial.printf("apply_br - _br_vir-1/97 %f\n",(int(_br_vir)-1) / 97);

  uint8_t br_max = int((_br_vir - 1) / 97.0 + 1);
  uint8_t br_cutoff_count = 96 - (_br_vir + 96) % 97;
  LOG_USB_BR("apply_br - _main_color %d\n", _main_color);
  LOG_USB_BR("apply_br - _br_vir %d\n", _br_vir);
  LOG_USB_BR("apply_br - br_max %d\n", br_max);
  LOG_USB_BR("apply_br - br_cutoff_count %d\n", br_cutoff_count);
  LOG_USB_BR("apply_br - _br_cutoff_bound %d\n", _br_cutoff_bound);
  int j = 97;
  float br_avg_per = 0;
  for (uint8_t i : cutoff_order) {
    LOG_USB_BR("main  cyc i=%d\t", i);
    if (br_cutoff_count > 0) {
      LOG_USB_BR("cutoff\t");
      br_avg_per = float(_leds_arr[i].sum()) * ((_br_vir - 1) / 97.0 + br_add) * br_gain / 765;
      _leds_arr[i].map((br_max - 1 + br_add) * br_gain);
      br_cutoff_count--;
    } else {
      LOG_USB_BR("full\t");
      br_avg_per = float(_leds_arr[i].sum()) * ((_br_vir - 1) / 97.0 + 1 + br_add) * br_gain / 765;
      _leds_arr[i].map((br_max + br_add) * br_gain);
    }
    j--;
    br_add += (br_add - int(br_add)) / j;
    if (_leds_arr[i].sum() <= _br_cutoff_bound) {
      LOG_USB_BR("dis\t");

      _led_active_count1--;
      br_add += (br_avg_per + br_add) / _led_active_count1;
      _leds_arr[i].r = 0;
      _leds_arr[i].g = 0;
      _leds_arr[i].b = 0;
    }
    LOG_USB_BR("|##| br_cutoff_count= %d  br_gain= %f br_add= %f br_avg_per= %f\n", br_cutoff_count, br_gain, br_add, br_avg_per);
  }

  for (uint8_t i : cutoff_imm) {
    LOG_USB_BR("centr cyc i=%d\t", i);
    LOG_USB_BR("full\t");
    _leds_arr[i].map((br_max + br_add) * br_gain);
    LOG_USB_BR("\n");
  }
}

void Strip::eff_singleColor() {
  fill(_main_color.get());
  _led_active_count = _led_amount;
}

void Strip::eff_halfSingleColor() {
  for (int i = 0; i < 50; i++) {
    _leds_arr[i].r = _main_color.r;
    _leds_arr[i].g = _main_color.g;
    _leds_arr[i].b = _main_color.b;
  }
  _led_active_count = 50;
}

void Strip::set_br(int br) {
  // Serial.printf("set_br - br %d\n",br);
  _br_vir = constrain(br, 0, 24735);
}

void Strip::set_main_color(Color_str color) {
  _main_color = color;
}