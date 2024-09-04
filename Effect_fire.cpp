#include "Effect_fire.h"

#define led_m(x,y) _leds_arr[y*10+(y%2==0?x:9-x)]

#define FRAMES 30
#define SIDE_CENTER 10
#define SIDE_RANGE 2

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

void Strip::eff_fire_temp()
{
  _fire_temp[_eff_fire_center][9] = _fire_central_cur_rand[_eff_fire_center][9].dic;
  for(int x = _eff_fire_center+1; x<10; x++)
    _fire_temp[x][9] = _fire_central_cur_rand[x][9].dic - random(-1,2);
  for(int x = _eff_fire_center-1; x>=0; x--)
    _fire_temp[x][9] = _fire_central_cur_rand[x][9].dic - random(-1,2);



  ////////сделать обработку сверху вниз


  for(int y = 8; y>=0;y--)
  {
    byte left = SIDE_CENTER - _fire_central_cur_rand[0][y].side;
    LOG_USB_FIRE("row: %d\t", y);
    for(int x = 0; x<10;x++)
    {
      _fire_temp[x][y] = (
        _fire_temp[x][y+1] + 
        float(_fire_temp[x][y>7?9:y+2]) *            ((_fire_central_cur_rand[x][y].down+random(-3,4)/10.0)/10.0) + 
        float(_fire_temp[constrain(x-1,0,9)][y+1]) * ((left+random(-3,4)/10.0)/10.0) + 
        float(_fire_temp[constrain(x+1,0,9)][y+1]) * ((_fire_central_cur_rand[x][y].side+random(-3,4)/10.0)/10.0)
        )/4;
        LOG_USB_FIRE("%d - ", _fire_temp[x][y]);
        int dic = _fire_central_cur_rand[x][y].dic+random(-1,2);
        LOG_USB_FIRE("%d = ", dic);
        _fire_temp[x][y] = _fire_temp[x][y]<dic?0:_fire_temp[x][y]-dic;
        LOG_USB_FIRE("%d\t", _fire_temp[x][y]);
    }
    LOG_USB_FIRE("\n");
  }


      // _fire_temp[x][y] = 20.29
      //   _fire_temp[x][y] = byte_d(0);

      //  _fire_temp[x][y] = (
      //   30 + 
      //   49.0 *        0.67 + 
      //   13.0 * 0.87 + 
      //   26.0 * 0.27
      //   )/4;
      //   _fire_temp[x][y] = byte_d(int(_fire_temp[x][y])-19+random(-1,2));

  // LOG_USB_FIRE("_fire_temp\n");
  // for(int i =0; i<10;i++)
  // {
  //   for(int j = 0; j<10;j++)
  //   {
  //     LOG_USB_FIRE("%d ",_fire_temp[j][i]);
  //   }
  //   LOG_USB_FIRE("\n");
  // }


}


void Strip::make_frame()
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

  LOG_USB_FIRE("old _fire_temp\n");
  for(int i =0; i<10;i++)
  {
    for(int j = 0; j<10;j++)
    {
      LOG_USB_FIRE("%d\t",_fire[j][i]);
    }
    LOG_USB_FIRE("\n");
  }

  eff_fire_temp();


  LOG_USB_FIRE("new _fire_temp\n");
  for(int i =0; i<10;i++)
  {
    for(int j = 0; j<10;j++)
    {
      LOG_USB_FIRE("%d\t",_fire_temp[j][i]);
    }
    LOG_USB_FIRE("\n");
  }

  for(int i =0; i<10;i++)
  {
    for(int j = 0; j<10;j++)
    {
      //LOG_USB_FIRE("%d ",_fire_key_frame[j][i]);
      led_m(i,j).set(fireColor(_fire_temp[i][j]));

    }
    //LOG_USB_FIRE("\n");
  }


  //   for(int x = _eff_fire_center+1; x<10; x++)
  // {
  //   byte near = _fire_temp[x-1][9];
  //   _fire_temp[x][9] = random(
  //     byte_d(near-sqrt(abs(_eff_fire_center-x))*15),
  //     byte_u(near+6)
  //   );
  // }
  //   for(int x = _eff_fire_center-1; x>=0; x--)
  // {
  //     byte near = _fire_temp[x+1][9];
  //     _fire_temp[x][9] = random(
  //       byte_d(near-(abs(_eff_fire_center-x))*15),
  //       byte_u(near+6)
  //     );
  // }

  // for(int x = _eff_fire_center+1; x<10; x++)
  // {
  //   byte near = _fire_temp[x-1][9];
  //   _fire_temp[x][9] = random(
  //     byte_d(near-sqrt(abs(_eff_fire_center-x))*15),
  //     byte_u(near-sqrt(abs(_eff_fire_center-x))*15+5)
  //   );
  // }
  //   for(int x = _eff_fire_center-1; x>=0; x--)
  // {
  //     byte near = _fire_temp[x+1][9];
  //     _fire_temp[x][9] = random(
  //       byte_d(near-(abs(_eff_fire_center-x))*15),
  //       byte_u(near-(abs(_eff_fire_center-x))*15+5)
  //     );
  // }

  // for(int y = 8; y>=0;y--)
  // {
  //   for(int x = 0; x<10;x++)
  //   {

  //     // для боковых общий центр для всего кадра
  //     //_fire_temp[x][y] = (_fire_temp[x][y+1] + byte_u(_fire_temp[(x+10-1)%10][y+1] * random(5,13)/10.0) + byte_u(_fire_temp[(x+10+1)%10][y+1] * random(5,13)/10.0) )/3;
  //     // _fire_temp[x][y] = (_fire_temp[x][y+1] + _fire_temp[x][y>7?9:y+2]* random(8,12)/10.0 + _fire_temp[(x+10-1)%10][y+1]* random(85,115)/100.0 + _fire_temp[(x+10+1)%10][y+1]* random(85,115)/100.0)/4;
  //     // _fire_temp[x][y] = (_fire_temp[x][y]>30?_fire_temp[x][y]-random(11,16):0);
  //     _fire_temp[x][y] = (_fire_temp[x][y+1] + _fire_temp[x][y>7?9:y+2]* random(95,105)/100.0 + _fire_temp[(x+10-1)%10][y+1]* random(90,110)/100.0 + _fire_temp[(x+10+1)%10][y+1]* random(90,110)/100.0)/4;
  //     _fire_temp[x][y] = (_fire_temp[x][y+1] + _fire_temp[x][y>7?9:y+2]* random(центр[][]-д,центр[][]+д)/100.0 + _fire_temp[(x+10-1)%10][y+1]* random(90,110)/100.0 + _fire_temp[(x+10+1)%10][y+1]* random(90,110)/100.0)/4;
  //     _fire_temp[x][y] = (_fire_temp[x][y]>30?_fire_temp[x][y]-random(15,16):0);
  //   }
  // } 
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