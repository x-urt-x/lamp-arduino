#include "Strip_class.h"
#define led(x,y) _leds_arr[y*10+(y%2==0?x:9-x)]
#define byte_u(val) ((val) > 255 ? 255 : (val)) 
#define byte_d(val) ((val) < 0 ? 0 : (val)) 

Strip::Strip(uint16_t n, int16_t p)
  : Adafruit_NeoPixel(n, p),
    _main_color(50, 50, 50),
    _second_color(50, 50, 50),
    _leds_arr(reinterpret_cast<Color_str*>(getPixels())),
    _pixels(getPixels()),
    _led_amount(n),
    _br_cutoff_bound(10) {
      eff_fire_setup();
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

void Strip::eff_fire_setup()
{
  _eff_fire_center_dec = 20;
  _eff_fire_center = random(0,10);
  //eff_fire_center=9;
  _fire_temp[_eff_fire_center][9] = random(150,256);

  for(int x = _eff_fire_center+1; x<10; x++)
  {
    byte near = _fire_temp[x-1][9];
    _fire_temp[x][9] = random(
      byte_d(near-sqrt(abs(_eff_fire_center-x))*15),
      byte_u(near+6)
    );
  }
    for(int x = _eff_fire_center-1; x>=0; x--)
  {
      byte near = _fire_temp[x+1][9];
      _fire_temp[x][9] = random(
        byte_d(near-sqrt(abs(_eff_fire_center-x))*15),
        byte_u(near+6)
      );
  }
}

void Strip::eff_fire()
{
  byte old = _fire_temp[_eff_fire_center][9];

  _eff_fire_center += random(0,3);
  LOG_USB_FIRE("center r %d\n", _eff_fire_center);
  _eff_fire_center = constrain(_eff_fire_center, 1, 10);
  _eff_fire_center--;

  LOG_USB_FIRE("center %d\n", _eff_fire_center);

  _fire_temp[_eff_fire_center][9] = random(
      byte_d(old-_eff_fire_center_dec),
      byte_u(old+21)
      );;
  
  if(_fire_temp[_eff_fire_center][9]<100) _eff_fire_center_dec = 0;
  if(_fire_temp[_eff_fire_center][9]>100) _eff_fire_center_dec = 20;
  if(_fire_temp[_eff_fire_center][9]>200) _eff_fire_center_dec = 50;

  for(int x = _eff_fire_center+1; x<10; x++)
  {
    byte near = _fire_temp[x-1][9];
    _fire_temp[x][9] = random(
      byte_d(near-sqrt(abs(_eff_fire_center-x))*15),
      byte_u(near+6)
    );
  }
    for(int x = _eff_fire_center-1; x>=0; x--)
  {
      byte near = _fire_temp[x+1][9];
      _fire_temp[x][9] = random(
        byte_d(near-(abs(_eff_fire_center-x))*15),
        byte_u(near+6)
      );
  }

  for(int y = 8; y>=0;y--)
  {
    for(int x = 0; x<10;x++)
    {
      _fire_temp[x][y] = (_fire_temp[x][y+1] + byte_u(_fire_temp[(x+10-1)%10][y+1] * random(5,13)/10.0) + byte_u(_fire_temp[(x+10+1)%10][y+1] * random(5,13)/10.0) )/3;
      _fire_temp[x][y] = (_fire_temp[x][y+1]  + _fire_temp[(x+10-1)%10][y+1] + _fire_temp[(x+10+1)%10][y+1])/3;
      _fire_temp[x][y] = (_fire_temp[x][y]>30?_fire_temp[x][y]-random(0,30):0);
    }
  } 

  for(int i =0; i<10;i++)
  {
    for(int j = 0; j<10;j++)
    {
      LOG_USB_FIRE("%d ",_fire_temp[j][i]);
      led(i,j).set(fireColor(_fire_temp[i][j]));

    }
    LOG_USB_FIRE("\n");
  }

}

uint32_t Strip::fireColor(byte temp) {
  // Определение цветов от красного до желтого в зависимости от температуры
  int t192 = round((temp / 255.0) * 191);

  uint8_t heatramp = t192 & 0x3F;  // 0..63
  heatramp <<= 2;  // 0..252

  if (t192 > 128) {
    return 0xffff00 | heatramp;
  } else if (t192 > 64) {
    return 0xff0000 | heatramp<<8;
  } else {
    return heatramp<<16;
  }
}