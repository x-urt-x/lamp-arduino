#ifndef STRUCTURES_H
#define STRUCTURES_H
#include "log.h"

#define byte_u(val) ((val) > 255 ? 255 : (val)) 
#define byte_d(val) ((val) < 0 ? 0 : (val))
#define byte_round_up(val) ((val)==byte(val)?(val):byte(val+1)) 

struct Color_str {
	byte g, r, b; //внутренний массив библиотеки имеет формат grb 
	Color_str() : r(0), g(0), b(0) {}
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
		return (r + g + b) / 3;
	}
	int sum()
	{
		return r + g + b;
	}
	void map(uint8_t br_in)
	{
		float br = br_in / 255.0;
		//Serial.printf("map1 - br %f r %d g %d b %d\n",br,r,g,b);
		r = r * br;
		if (r > 255) r = 255;
		g = g * br;
		if (g > 255) g = 255;
		b = b * br;
		if (b > 255) b = 255;
		LOG_USB_COLOR_MAP("| map- br%d r%d g%d b%d |\n", br_in, r, g, b);
	}
};

struct Options
{
	int strip_update_delay_time;
	Color_str main_color, second_color;
	int br_cutoff_bound;
	int step;

	Options() : strip_update_delay_time(0), main_color(), second_color(), br_cutoff_bound(0), step(0) {}
};

#endif