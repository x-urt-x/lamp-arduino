#include "Effect_fire.h"

const byte Effect_fire::cutoff_order[98] = { 0,9,1,8,2,7,3,6,4,5,10,19,11,18,12,17,13,16,14,15,20,29,21,28,22,27,23,26,24,25,30,39,31,38,32,37,33,36,34,35,40,49,41,48,42,47,43,46,44,45,50,59,51,58,52,57,53,56,54,55,60,69,61,68,62,67,63,66,64,65,70,79,71,78,72,77,73,76,74,75,80,89,81,88,82,87,83,86,84,85,90,99,91,98,92,97,93,96};
const byte Effect_fire::cutoff_imm[2] = { 94,95};

Effect_fire::Effect_fire(Color_str* leds_arr, Color_str* main_color, Color_str* second_color) 
  : _leds_arr(leds_arr), _main_color(main_color), _second_color(second_color) {}

void Effect_fire::set_step(int step)
{
  _step = step;
}

const unsigned char* Effect_fire::get_cutoff_order()
{
  return cutoff_order;
}

const unsigned char* Effect_fire::get_cutoff_imm()
{
  return cutoff_imm;
}

byte Effect_fire::get_cutoff_order_len()
{
    return cutoff_order_len;
}

byte Effect_fire::get_cutoff_imm_len()
{
    return cutoff_imm_len;
}

void Effect_fire::setup() 
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

void Effect_fire::make_frame()
{
  unsigned char old = _fire_temp[_eff_fire_center][9];

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
    unsigned char near = _fire_temp[x-1][9];
    _fire_temp[x][9] = random(
      byte_d(near-sqrt(abs(_eff_fire_center-x))*15),
      byte_u(near+6)
    );
  }
    for(int x = _eff_fire_center-1; x>=0; x--)
  {
      unsigned char near = _fire_temp[x+1][9];
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
      led_m(i,j).set(fireColor(_fire_temp[i][j]));

    }
    LOG_USB_FIRE("\n");
  }

}

uint32_t Effect_fire::fireColor(byte temp) {
  // Определение цветов от красного до желтого в зависимости от температуры
  int t192 = round((temp / 255.0) * 191);

  unsigned char heatramp = t192 & 0x3F;  // 0..63
  heatramp <<= 2;  // 0..252

  if (t192 > 128) {
    return 0xffff00 | heatramp;
  } else if (t192 > 64) {
    return 0xff0000 | heatramp<<8;
  } else {
    return heatramp<<16;
  }
}