#ifndef STRUCTURES_H
#define STRUCTURES_H
#include "log.h"
struct Color_str{
  byte g, r, b; //внутренний массив библиотеки имеет формат grb 
  
  Color_str(byte red, byte green, byte blue) : r(red), g(green), b(blue) {}

  void set(uint32_t colorValue) {
    r = (colorValue >> 16) & 0xFF;
    g = (colorValue >> 8) & 0xFF;
    b = colorValue & 0xFF;
  }
  uint32_t get()
  {
    return (uint32_t)r << 16 | (uint32_t)g << 8 | b;
  }
  byte avg()
  {
    return (r + g + b)/3;
  }
  int sum()
  {
    return r+g+b;
  }
  void map(uint8_t br_in)
  {
    float br = br_in/255.0;
    //Serial.printf("map1 - br %f r %d g %d b %d\n",br,r,g,b);
    r = r * br;
    if(r>255) r=255;
    g = g * br;
    if(g>255) g=255;
    b = b * br;
    if(b>255) b=255;
    LOG_USB_COLOR_MAP("| map- br%f r%d g%d b%d |\t",br,r,g,b);
  }
};

#endif