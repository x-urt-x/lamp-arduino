#include "Effect_singleColor.h"

const byte Effect_singleColor::cutoff_order[96] = { 0,90,1,91,2,92,3,93,4,94,5,95,6,96,7,97,8,98,9,99,10,80,11,81,12,82,13,83,14,84,15,85,16,86,17,87,18,88,19,89,20,70,21,71,22,72,23,73,24,74,25,75,26,76,27,77,28,78,29,79,30,60,31,61,32,62,33,63,34,64,35,65,36,66,37,67,38,68,39,69,40,50,49,59,41,51,48,58,42,52,47,57,43,53,46,56}; 
const byte Effect_singleColor::cutoff_imm[4]= { 44,45,54,55 };

Effect_singleColor::Effect_singleColor(Color_str* leds_arr, Color_str* main_color) 
  : _leds_arr(leds_arr), _main_color(main_color) {}

void Effect_singleColor::set_step(int step)
{
  _step = step;
}

const unsigned char* Effect_singleColor::get_cutoff_order()
{
  return cutoff_order;
}

const unsigned char* Effect_singleColor::get_cutoff_imm()
{
  return cutoff_imm;
}

byte Effect_singleColor::get_cutoff_order_len()
{
    return cutoff_order_len;
}

byte Effect_singleColor::get_cutoff_imm_len()
{
    return cutoff_imm_len;
}

void Effect_singleColor::setup(){}

void Effect_singleColor::make_frame()
{
    for (int i = 0; i < 100; i++) {
    _leds_arr[i].r = _main_color->r;
    _leds_arr[i].g = _main_color->g;
    _leds_arr[i].b = _main_color->b;
  }
}