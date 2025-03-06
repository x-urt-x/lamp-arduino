#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Config.h"
#include "log.h"
#include <avr/pgmspace.h>

#define byte_u(val) ((val) > 255 ? 255 : (val)) 
#define byte_d(val) ((val) < 0 ? 0 : (val))
#define byte_round_up(val) ((val)==byte(val)?(val):byte(val+1)) 

#define parseIn_int(val)				\
(val) = atoi(input_str);				\
input_str++;							\
while (*input_str != ' ') input_str++;	\

struct Color_str
{
	byte g, r, b; //внутренний массив библиотеки имеет формат grb 
	Color_str() : r(0), g(0), b(0) {}
	Color_str(byte red, byte green, byte blue) : r(red), g(green), b(blue) {}
	Color_str(uint32_t colorValue) : r((colorValue >> 16) & 0xFF), g((colorValue >> 8) & 0xFF), b(colorValue & 0xFF) {}

	void set(uint32_t colorValue) {
		r = (colorValue >> 16) & 0xFF;
		g = (colorValue >> 8) & 0xFF;
		b = colorValue & 0xFF;
	}
	void set(Color_str color) {
		r = color.r;
		g = color.g;
		b = color.b;
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
		r = r * br;
		if (r > 255) r = 255;
		g = g * br;
		if (g > 255) g = 255;
		b = b * br;
		if (b > 255) b = 255;
		LOG_USB_COLOR_MAP("| map- br%d r%d g%d b%d |\n", br_in, r, g, b);
	}
	static Color_str lerp(Color_str from, Color_str to, byte coef) { return lerp(from, to, float(coef / 255.0)); }
	static Color_str lerp(Color_str from, Color_str to, float coef)
	{
		Color_str res;
		res.r = from.r * coef + to.r * (1 - coef);
		res.g = from.g * coef + to.g * (1 - coef);
		res.b = from.b * coef + to.b * (1 - coef);
		return res;
	}
	static Color_str cyc_lerp(Color_str from, Color_str to, int coef) { return cyc_lerp(from, to, float(coef / 256 * 256 - 1)); }
	static Color_str cyc_lerp(Color_str from, Color_str to, float coef)
	{
		if (coef < 0.5)
			return lerp(from, to, coef * 2);
		else
			return lerp(to, from, float((coef - 0.5) * 2));
	}
	static inline Color_str ColorHSV(int hue)
	{
		return Color_str(Adafruit_NeoPixel::ColorHSV(hue));
	}
};

struct Cutoff_str
{
	byte _order[MATR_LEN];
	byte _disableable_count;
};
#endif