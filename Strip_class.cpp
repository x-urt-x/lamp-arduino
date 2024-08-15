#include "Strip_class.h"
#define USB_LOG_BR

Strip::Strip(uint16_t n, int16_t p) :
    Adafruit_NeoPixel(n, p, NEO_GRB + NEO_KHZ800),
    _main_color(50, 50, 50),
    _second_color(50, 50, 50),
    _leds_arr(reinterpret_cast<Color_str*>(getPixels())),
    _pixels(getPixels()),
    _led_amount(n),
     _br_cutoff_bound(10)
{
}

void Strip::apply_br()
{
  float br_add = 0;
  float br_gain = _led_amount / _led_active_count;
  //Serial.printf("apply_br - _br_vir-1/97 %f\n",(int(_br_vir)-1) / 97);
  
  uint8_t br_max = int((_br_vir-1) / 97.0 + 1);
  uint8_t br_cutoff_count = 96 - (_br_vir+96) % 97;
  #ifdef USB_LOG_BR
  Serial.printf("apply_br - _main_color %d\n",_main_color);
  Serial.printf("apply_br - _br_vir %d\n",_br_vir);
  Serial.printf("apply_br - br_max %d\n",br_max);
  Serial.printf("apply_br - br_cutoff_count %d\n",br_cutoff_count);
  Serial.printf("apply_br - _br_cutoff_bound %d\n",_br_cutoff_bound);
  #endif
  int j = 97;
  float br_avg_per=0;
  for(uint8_t i : cutoff_order)
  {
#ifdef USB_LOG_BR
Serial.printf("main  cyc i=%d\t", i);
#endif
    if(br_cutoff_count>0)
    {
#ifdef USB_LOG_BR
Serial.printf("cutoff\t");
#endif
      br_avg_per =  float(_leds_arr[i].sum())*((_br_vir-1) / 97.0+br_add)*br_gain / 765;
      _leds_arr[i].map((br_max-1+br_add)*br_gain);
      br_cutoff_count--;
    }
    else
    {
#ifdef USB_LOG_BR
Serial.printf("full\t");
#endif
      br_avg_per =  float(_leds_arr[i].sum())*((_br_vir-1) / 97.0 + 1+br_add)*br_gain / 765;
      _leds_arr[i].map((br_max+br_add)*br_gain);
    }
    j--;
    br_add += (br_add - int(br_add)) / j;
    if(_leds_arr[i].sum()<=_br_cutoff_bound)
    {
#ifdef USB_LOG_BR
Serial.printf("dis\t");
#endif

      _led_active_count--;
      br_add += (br_avg_per+br_add) / _led_active_count;
      _leds_arr[i].r = 0;
      _leds_arr[i].g = 0;
      _leds_arr[i].b = 0;
    }
#ifdef USB_LOG_BR
Serial.printf("|##| br_cutoff_count= %d  br_gain= %f br_add= %f br_avg_per= %f\t", br_cutoff_count, br_gain, br_add, br_avg_per);
Serial.printf("\n");
#endif
  
  }

  for(uint8_t i : cutoff_imm)
  {
    #ifdef USB_LOG_BR
    Serial.printf("centr cyc i=%d\t", i);
    Serial.printf("full\t");
    #endif
    _leds_arr[i].map((br_max+br_add)*br_gain);
    #ifdef USB_LOG_BR
    Serial.printf("\n");
    #endif
  }
}

void Strip::eff_singleColor()
{
  fill(_main_color.get());  
  _led_active_count = _led_amount;
}

void Strip::set_br(int br)
{
  // Serial.printf("set_br - br %d\n",br);
  _br_vir = constrain(br, 0, 24735);
}

void Strip::set_main_color(Color_str color)
{
  _main_color = color;
}