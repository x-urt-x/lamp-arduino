#include "Strip_class.h"
#define FRAMES 30
#define SIDE_CENTER 10
#define SIDE_RANGE 2

void Strip::eff_fire_setup()
{
  _eff_fire_frame_count = FRAMES;
  //центр пламени
  _eff_fire_center_dec = 20;
  _eff_fire_center = random(0,10);
  _eff_fire_center_temp = random(150,256);

  _eff_fire_center = 4;
  _eff_fire_center_temp = 220;

  eff_fire_key_rand_gen();
  for(int i =0; i<10;i++)
    for(int j = 0; j<10;j++)
    {
      _fire_central_cur_rand[i][j].dic = _fire_central_key_rand[i][j].dic;
      _fire_central_cur_rand[i][j].down = _fire_central_key_rand[i][j].down;
      _fire_central_cur_rand[i][j].side = _fire_central_key_rand[i][j].side;
    }
}

void Strip::eff_fire_key_rand_gen()
{
  LOG_USB_FIRE("center = %d\tcenter_temp = %d\n",_eff_fire_center,_eff_fire_center_temp);

  _fire_central_key_rand[_eff_fire_center][9].dic = _eff_fire_center_temp;
  byte near = _eff_fire_center_temp;
  for(int x = _eff_fire_center+1; x<10; x++)
  {
    _fire_central_key_rand[x][9].dic = random(
        byte_d(near-sqrt(abs(_eff_fire_center-x))*20),
        byte_u(near-sqrt(abs(_eff_fire_center-x))*20+10)
      );
    near = _fire_central_key_rand[x-1][9].dic;
  }
    for(int x = _eff_fire_center-1; x>=0; x--)
  {
    _fire_central_key_rand[x][9].dic = random(
        byte_d(near-sqrt(abs(_eff_fire_center-x))*20),
        byte_u(near-sqrt(abs(_eff_fire_center-x))*20+10)
      );
    near = _fire_central_key_rand[x+1][9].dic;
  }

  for(int y = 8; y>=0;y--)
  {
    byte side = random(SIDE_CENTER-SIDE_RANGE,SIDE_CENTER+SIDE_RANGE+1);
    for(int x = 0; x<10;x++)
    {
      _fire_central_key_rand[x][y].down = random(9,14);
      _fire_central_key_rand[x][y].side = side;
      _fire_central_key_rand[x][y].dic = random(10,20);
    }
  } 
  // LOG_USB_FIRE("eff_fire_key_rand_gen\n");
  // for(int i =0; i<10;i++)
  // {
  //   for(int j = 0; j<10;j++)
  //   {
  //     LOG_USB_FIRE("(%d %d %d %d)",_fire_central_key_rand[j][i].down, _fire_central_key_rand[j][i].left, _fire_central_key_rand[j][i].right, _fire_central_key_rand[j][i].dic);
  //   }
  //   LOG_USB_FIRE("\n");
  // }
}

int Strip::eff_fire_central_frame()
{
  LOG_USB_FIRE("++++eff_fire_central_frame++++\n");
  byte (*fire_central_key_rand)[10][3] = reinterpret_cast<byte (*)[10][3]>(_fire_central_key_rand);
  byte (*fire_central_cur_rand)[10][3] = reinterpret_cast<byte (*)[10][3]>(_fire_central_cur_rand);

  // LOG_USB_FIRE("fire_central_key_rand\n");
  // for(int i =0; i<10;i++)
  // {
  //   for(int j = 0; j<10;j++)
  //   {
  //     LOG_USB_FIRE("(");
  //     for(int k = 0; k<4; k++)
  //     {
  //       LOG_USB_FIRE("%d ",fire_central_key_rand[j][i][k]);
  //     }
  //     LOG_USB_FIRE(")\t");
  //   }
  //   LOG_USB_FIRE("\n");
  // }

  // LOG_USB_FIRE("fire_central_cur_rand\n");
  // for(int i =0; i<10;i++)
  // {
  //   for(int j = 0; j<10;j++)
  //   {
  //     LOG_USB_FIRE("(");
  //     for(int k = 0; k<4; k++)
  //     {
  //       LOG_USB_FIRE("%d ",fire_central_cur_rand[j][i][k]);
  //     }
  //     LOG_USB_FIRE(")\t");
  //   }
  //   LOG_USB_FIRE("\n");
  // }

  LOG_USB_FIRE("fire_central_cur_rand calculation\n");
  int difference=0;
  for(int y = 0; y<10; y++)
  {
    for(int x = 0; x<10; x++)
    {
      for(int i = 0; i<3; i++)
      {
        float dif = 0;
        if(fire_central_cur_rand[x][y][i]>fire_central_key_rand[x][y][i])
        {
          dif = fire_central_cur_rand[x][y][i] - fire_central_key_rand[x][y][i];
          fire_central_cur_rand[x][y][i] -= byte_round_up(dif/(FRAMES-_eff_fire_frame_count));
        }
        else if(fire_central_cur_rand[x][y][i]<fire_central_key_rand[x][y][i])
        {
          dif = fire_central_key_rand[x][y][i] - fire_central_cur_rand[x][y][i];
          fire_central_cur_rand[x][y][i] += byte_round_up(dif/(FRAMES-_eff_fire_frame_count));
        }
        difference += dif;
      }
    }
  }
  // LOG_USB_FIRE("fire_central_cur_rand\n");
  // for(int i =0; i<10;i++)
  // {
  //   for(int j = 0; j<10;j++)
  //   {
  //     LOG_USB_FIRE("(");
  //     for(int k = 0; k<4; k++)
  //     {
  //       LOG_USB_FIRE("%d ",fire_central_cur_rand[j][i][k]);
  //     }
  //     LOG_USB_FIRE(")\t");
  //   }
  //   LOG_USB_FIRE("\n");
  // }
  _eff_fire_frame_count--;
  LOG_USB_FIRE("difference = %d _eff_fire_frame_count = %d\n", difference, _eff_fire_frame_count);
  LOG_USB_FIRE("++++eff_fire_central_frame++++\n");
  
  return difference;
}

void Strip::eff_fire_temp_frame()
{
  _fire_temp_frame[_eff_fire_center][9] = _fire_central_cur_rand[_eff_fire_center][9].dic;
  for(int x = _eff_fire_center+1; x<10; x++)
    _fire_temp_frame[x][9] = _fire_central_cur_rand[x][9].dic - random(-1,2);
  for(int x = _eff_fire_center-1; x>=0; x--)
    _fire_temp_frame[x][9] = _fire_central_cur_rand[x][9].dic - random(-1,2);



  ////////сделать обработку сверху вниз


  for(int y = 8; y>=0;y--)
  {
    byte left = SIDE_CENTER - _fire_central_cur_rand[0][y].side;
    LOG_USB_FIRE("row: %d\t", y);
    for(int x = 0; x<10;x++)
    {
      _fire_temp_frame[x][y] = (
        _fire_temp_frame[x][y+1] + 
        float(_fire_temp_frame[x][y>7?9:y+2]) *            ((_fire_central_cur_rand[x][y].down+random(-3,4)/10.0)/10.0) + 
        float(_fire_temp_frame[constrain(x-1,0,9)][y+1]) * ((left+random(-3,4)/10.0)/10.0) + 
        float(_fire_temp_frame[constrain(x+1,0,9)][y+1]) * ((_fire_central_cur_rand[x][y].side+random(-3,4)/10.0)/10.0)
        )/4;
        LOG_USB_FIRE("%d - ", _fire_temp_frame[x][y]);
        int dic = _fire_central_cur_rand[x][y].dic+random(-1,2);
        LOG_USB_FIRE("%d = ", dic);
        _fire_temp_frame[x][y] = _fire_temp_frame[x][y]<dic?0:_fire_temp_frame[x][y]-dic;
        LOG_USB_FIRE("%d\t", _fire_temp_frame[x][y]);
    }
    LOG_USB_FIRE("\n");
  }


      // _fire_temp_frame[x][y] = 20.29
      //   _fire_temp_frame[x][y] = byte_d(0);

      //  _fire_temp_frame[x][y] = (
      //   30 + 
      //   49.0 *        0.67 + 
      //   13.0 * 0.87 + 
      //   26.0 * 0.27
      //   )/4;
      //   _fire_temp_frame[x][y] = byte_d(int(_fire_temp_frame[x][y])-19+random(-1,2));

  // LOG_USB_FIRE("_fire_temp_frame\n");
  // for(int i =0; i<10;i++)
  // {
  //   for(int j = 0; j<10;j++)
  //   {
  //     LOG_USB_FIRE("%d ",_fire_temp_frame[j][i]);
  //   }
  //   LOG_USB_FIRE("\n");
  // }


}


void Strip::eff_fire()
{
  // LOG_USB_FIRE("center %d\n", _eff_fire_center);
  LOG_USB_FIRE("\n====Frame start====\n");
  if(eff_fire_central_frame()<5){
    LOG_USB_FIRE("----new frame----\n");
    //температура центра
    _eff_fire_center_temp = random(
      byte_d(_eff_fire_center_temp-_eff_fire_center_dec),
      byte_u(_eff_fire_center_temp+21)
      );;
    if(_eff_fire_center_temp<100) _eff_fire_center_dec = 0;
    if(_eff_fire_center_temp>100) _eff_fire_center_dec = 20;
    if(_eff_fire_center_temp>200) _eff_fire_center_dec = 50;

    //положение центра
    _eff_fire_center += random(0,3);
    
    _eff_fire_center = constrain(_eff_fire_center, 1, 10);
    _eff_fire_center--;

    LOG_USB_FIRE("_eff_fire_center %d\n", _eff_fire_center);
    LOG_USB_FIRE("_eff_fire_center_temp %d\n",_eff_fire_center_temp);
    LOG_USB_FIRE("----new frame----\n");
    eff_fire_key_rand_gen();
    _eff_fire_frame_count = FRAMES;
  }



  LOG_USB_FIRE("eff_fire_key_rand_gen\n");
  for(int i =0; i<10;i++)
  {
    for(int j = 0; j<10;j++)
    {
      LOG_USB_FIRE("(%d %d %d)\t",_fire_central_key_rand[j][i].down, _fire_central_key_rand[j][i].side, _fire_central_key_rand[j][i].dic);
    }
    LOG_USB_FIRE("\n");
  }
  LOG_USB_FIRE("_fire_central_cur_rand\n");
  for(int i =0; i<10;i++)
  {
    for(int j = 0; j<10;j++)
    {
      LOG_USB_FIRE("(%d %d %d)\t",_fire_central_cur_rand[j][i].down, _fire_central_cur_rand[j][i].side, _fire_central_cur_rand[j][i].dic);
    }
    LOG_USB_FIRE("\n");
  }

  LOG_USB_FIRE("old _fire_temp_frame\n");
  for(int i =0; i<10;i++)
  {
    for(int j = 0; j<10;j++)
    {
      LOG_USB_FIRE("%d\t",_fire_temp_frame[j][i]);
    }
    LOG_USB_FIRE("\n");
  }

  eff_fire_temp_frame();


  LOG_USB_FIRE("new _fire_temp_frame\n");
  for(int i =0; i<10;i++)
  {
    for(int j = 0; j<10;j++)
    {
      LOG_USB_FIRE("%d\t",_fire_temp_frame[j][i]);
    }
    LOG_USB_FIRE("\n");
  }

  for(int i =0; i<10;i++)
  {
    for(int j = 0; j<10;j++)
    {
      //LOG_USB_FIRE("%d ",_fire_key_frame[j][i]);
      led_m(i,j).set(fireColor(_fire_temp_frame[i][j]));

    }
    //LOG_USB_FIRE("\n");
  }


  //   for(int x = _eff_fire_center+1; x<10; x++)
  // {
  //   byte near = _fire_temp_frame[x-1][9];
  //   _fire_temp_frame[x][9] = random(
  //     byte_d(near-sqrt(abs(_eff_fire_center-x))*15),
  //     byte_u(near+6)
  //   );
  // }
  //   for(int x = _eff_fire_center-1; x>=0; x--)
  // {
  //     byte near = _fire_temp_frame[x+1][9];
  //     _fire_temp_frame[x][9] = random(
  //       byte_d(near-(abs(_eff_fire_center-x))*15),
  //       byte_u(near+6)
  //     );
  // }

  // for(int x = _eff_fire_center+1; x<10; x++)
  // {
  //   byte near = _fire_temp_frame[x-1][9];
  //   _fire_temp_frame[x][9] = random(
  //     byte_d(near-sqrt(abs(_eff_fire_center-x))*15),
  //     byte_u(near-sqrt(abs(_eff_fire_center-x))*15+5)
  //   );
  // }
  //   for(int x = _eff_fire_center-1; x>=0; x--)
  // {
  //     byte near = _fire_temp_frame[x+1][9];
  //     _fire_temp_frame[x][9] = random(
  //       byte_d(near-(abs(_eff_fire_center-x))*15),
  //       byte_u(near-(abs(_eff_fire_center-x))*15+5)
  //     );
  // }

  // for(int y = 8; y>=0;y--)
  // {
  //   for(int x = 0; x<10;x++)
  //   {

  //     // для боковых общий центр для всего кадра
  //     //_fire_temp_frame[x][y] = (_fire_temp_frame[x][y+1] + byte_u(_fire_temp_frame[(x+10-1)%10][y+1] * random(5,13)/10.0) + byte_u(_fire_temp_frame[(x+10+1)%10][y+1] * random(5,13)/10.0) )/3;
  //     // _fire_temp_frame[x][y] = (_fire_temp_frame[x][y+1] + _fire_temp_frame[x][y>7?9:y+2]* random(8,12)/10.0 + _fire_temp_frame[(x+10-1)%10][y+1]* random(85,115)/100.0 + _fire_temp_frame[(x+10+1)%10][y+1]* random(85,115)/100.0)/4;
  //     // _fire_temp_frame[x][y] = (_fire_temp_frame[x][y]>30?_fire_temp_frame[x][y]-random(11,16):0);
  //     _fire_temp_frame[x][y] = (_fire_temp_frame[x][y+1] + _fire_temp_frame[x][y>7?9:y+2]* random(95,105)/100.0 + _fire_temp_frame[(x+10-1)%10][y+1]* random(90,110)/100.0 + _fire_temp_frame[(x+10+1)%10][y+1]* random(90,110)/100.0)/4;
  //     _fire_temp_frame[x][y] = (_fire_temp_frame[x][y+1] + _fire_temp_frame[x][y>7?9:y+2]* random(центр[][]-д,центр[][]+д)/100.0 + _fire_temp_frame[(x+10-1)%10][y+1]* random(90,110)/100.0 + _fire_temp_frame[(x+10+1)%10][y+1]* random(90,110)/100.0)/4;
  //     _fire_temp_frame[x][y] = (_fire_temp_frame[x][y]>30?_fire_temp_frame[x][y]-random(15,16):0);
  //   }
  // } 
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