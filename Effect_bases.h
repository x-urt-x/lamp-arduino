#ifndef EFFECT_BASES_H
#define EFFECT_BASES_H

#include "Config.h"
#include "Structures.h"
#include "IEffect.h"

class Effectable : virtual public IEffect
{
public:
	static const BaseIDEnum BaseID = BaseIDEnum::EffectableID;
	static const size_t BaseSize;
	virtual byte* get_cutoff_order();
	virtual byte* get_cutoff_imm();
	virtual byte get_cutoff_order_len();
	virtual byte get_cutoff_imm_len();

	virtual String get_effect_name() = 0;
	virtual void apply_default_option() = 0;
	
	void set_strip_update_delay_time(int strip_update_delay_time);
	void set_br_cutoff_bound(int br_cutoff_bound);
	void set_effect_step(int step);
	int get_strip_update_delay_time();
	int get_br_cutoff_bound();
	int get_effect_step();
protected:
	struct Option_effect
	{
		int strip_update_delay_time;
		int br_cutoff_bound;
		int effect_step;
	};
	inline virtual Option_effect* get_option_effect() = 0;

	//direct pointer for better perfomance
	static const int _cutoff_order_len = MATR_LEN-4;
	static const int _cutoff_imm_len = 4;
	static byte _cutoff_order[_cutoff_order_len];
	static byte _cutoff_imm[_cutoff_imm_len];
};

class Colorable : virtual public IEffect
{
public:
	static const BaseIDEnum BaseID = BaseIDEnum::ColorableID;
	static const size_t BaseSize;
	void set_color(Color_str color, int num = 0);
	void set_color(uint32_t color, int num = 0);
	Color_str* get_colors();
	int get_color_len();
protected:
	struct Option_color
	{
		Color_str* colors;
		int color_len;
	};
	inline virtual Option_color* get_option_color() = 0;
};

class Preseteble : virtual public IEffect
{
public:
	static const BaseIDEnum BaseID = BaseIDEnum::PresetebleID;
	static const size_t BaseSize;
	int get_preset_len();
	const String* get_preset_names();
	virtual void set_preset(int num = 0) = 0;
protected:
	struct Option_preset
	{
		String* names;
		int names_len;
	};
	inline virtual Option_preset* get_option_preset() = 0;
};

class Rainbowble : virtual public IEffect
{
public:
	static const BaseIDEnum BaseID = BaseIDEnum::RainbowbleID;
	static const size_t BaseSize;
	void set_rainbow_step(int step, int num = 0);
	void set_rainbow_state(bool state, int num = 0);
	bool* get_rainbow_states();
	int* get_rainbow_steps();
	int get_rainbow_len();
protected:
	struct Option_rainbow
	{
		bool* states;
		int states_len;
		int* steps;
	};
	inline virtual Option_rainbow* get_option_rainbow() = 0;
};
#endif