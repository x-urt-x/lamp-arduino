#include "Effect_bases.h"

const size_t Effectable::BaseSize = sizeof(Effectable);
byte* Effectable::get_cutoff_order() { return _cutoff_order; }
byte* Effectable::get_cutoff_imm() { return _cutoff_imm; }
byte Effectable::get_cutoff_order_len() { return _cutoff_order_len; }
byte Effectable::get_cutoff_imm_len() { return _cutoff_imm_len; }
void Effectable::set_strip_update_delay_time(int strip_update_delay_time) { get_option_effect()->strip_update_delay_time = strip_update_delay_time; }
void Effectable::set_br_cutoff_bound(int br_cutoff_bound) { get_option_effect()->br_cutoff_bound = br_cutoff_bound; }
void Effectable::set_effect_step(int step) { get_option_effect()->effect_step = step; }
int Effectable::get_strip_update_delay_time() { return get_option_effect()->strip_update_delay_time; }
int Effectable::get_br_cutoff_bound() { return get_option_effect()->br_cutoff_bound; }
int Effectable::get_effect_step() { return get_option_effect()->effect_step; }
byte Effectable::_cutoff_order[Effectable::_cutoff_order_len] = { 0,90,1,91,2,92,3,93,4,94,5,95,6,96,7,97,8,98,9,99,10,80,11,81,12,82,13,83,14,84,15,85,16,86,17,87,18,88,19,89,20,70,21,71,22,72,23,73,24,74,25,75,26,76,27,77,28,78,29,79,30,60,31,61,32,62,33,63,34,64,35,65,36,66,37,67,38,68,39,69,40,50,49,59,41,51,48,58,42,52,47,57,43,53,46,56 };
byte Effectable::_cutoff_imm[Effectable::_cutoff_imm_len] = { 44,45,54,55 };

const size_t Colorable::BaseSize = sizeof(Colorable);
void Colorable::set_color(Color_str color, int num)
{
	if (num >= get_option_color()->color_len) return;
	get_option_color()->colors[num].set(color);
}
void Colorable::set_color(uint32_t color, int num)
{
	if (num >= get_option_color()->color_len) return;
	get_option_color()->colors[num].set(color);
}
Color_str* Colorable::get_colors() { return get_option_color()->colors; }
int Colorable::get_color_len() { return get_option_color()->color_len; }

const size_t Preseteble::BaseSize = sizeof(Preseteble);
int Preseteble::get_preset_len() { return get_option_preset()->names_len; }
const String* Preseteble::get_preset_names() { return get_option_preset()->names; }

const size_t Rainbowble::BaseSize = sizeof(Rainbowble);
void Rainbowble::set_rainbow_step(int step, int num)
{
	if (num >= get_option_rainbow()->states_len) return;
	get_option_rainbow()->steps[num] = step;
}
void Rainbowble::set_rainbow_state(bool state, int num)
{
	if (num >= get_option_rainbow()->states_len) return;
	get_option_rainbow()->states[num] = state;
}
bool* Rainbowble::get_rainbow_states() { return get_option_rainbow()->states; }
int* Rainbowble::get_rainbow_steps() { return get_option_rainbow()->steps; }
int Rainbowble::get_rainbow_len() { return get_option_rainbow()->states_len; }