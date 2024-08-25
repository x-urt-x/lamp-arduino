#include "Strip_class.h"

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
      led_m(i,j).set(fireColor(_fire_temp[i][j]));

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