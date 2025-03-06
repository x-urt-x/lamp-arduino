#include "Strip_class.h"

void Strip::apply_br() {
	const Cutoff_str* cutoff_str = effect->get_cutoff_str(); //save pointer of cutoff options to not repeat virtual invocation
	const uint16_t br_cutoff_bound = get_br_cutoff_bound();
	if (_br_vir > br_limit) _br_vir = br_limit; //check hadr limit of br 
#ifdef MATR16x16
	//using >>8 instead /256
	const uint br_base = (_br_vir << 8) & 0xFF'FFFF; //8.16 precise br level for all led
#else
	const uint br_base = ((_br_vir << 16) / MATR_LEN) & 0xFF'FFFF; //8.16 precise br level for all led
#endif // MATR16x16
	uint light_avg_add = 0; //8.16 addition from rounding or disable previous leds
	//first process leds, that can be disable
	for (uint16_t i = 0; i < MATR_LEN; i++)
	{
		Color_str* led = _leds_arr + cutoff_str->_order[i]; //current led to process
		uint r_precise_pre = (led->r/*8.0*/ * br_base/*8.16*/)/*16.16*/ >> 8; //8.16
		uint g_precise_pre = (led->g * br_base) >> 8;
		uint b_precise_pre = (led->b * br_base) >> 8;
		uint br_pre = (r_precise_pre + g_precise_pre + b_precise_pre) / 3; //8.16 base br considering color
		byte br_pre_integer8 = br_pre >> 16; //8.0
		uint light_pre = (LUTGamma[br_pre_integer8]/*8.8*/ << 8)/*8.16*/ + lerpGamma(br_pre_integer8, br_pre % 0xFFFF)/*8.16*/;  //8.16 base light considering base br and color
		uint light_full = light_pre + light_avg_add; //8.16 full light considering base br, color and added light
		byte light_full_integer8 = light_full >> 16; //8.0
		uint br_full = (LUTInvGamma[light_full_integer8]/*8.8*/ << 8)/*8.16*/ + lerpInvGamma(light_full_integer8, light_full & 0xFFFF)/*8.16*/; //8.16 full br considering base br, color and added light
		byte br_full_integer8 = br_full >> 16; //8.0

		uint r_precise_final = (led->r/*8.0*/ * br_full/*8.16*/)/*16.16*/ >> 8; //8.16
		uint g_precise_final = (led->g * br_full) >> 8;
		uint b_precise_final = (led->b * br_full) >> 8;
		uint br_final = (r_precise_final + g_precise_final + b_precise_final) / 3; //8.16
		uint br_final_integer8 = br_final >> 16; //8.0

		LOG_USB_BR("led= %d ", cutoff_str->_order[i]);
		LOG_USB_BR("rem= %d ", (MATR_LEN - 1 - i));
		LOG_USB_BR("br_base= %d.%04d ", (br_base >> 16), (br_base & 0xFFFF) * 10000L >> 16);
		LOG_USB_BR("l_avg_add= %d.%04d ", (light_avg_add >> 16), (light_avg_add & 0xFFFF) * 10000L >> 16);
		LOG_USB_BR("br_pre= %d.%04d ", (br_pre >> 16), (br_pre & 0xFFFF) * 10000L >> 16);
		LOG_USB_BR("l_pre= %d.%04d ", (light_pre >> 16), (light_pre & 0xFFFF) * 10000L >> 16);
		LOG_USB_BR("l_full= %d.%04d ", (light_full >> 16), (light_full & 0xFFFF) * 10000L >> 16);
		LOG_USB_BR("br_full = %d.%04d ", (br_full >> 16), (br_full & 0xFFFF) * 10000L >> 16);

		if (i < cutoff_str->_disableable_count && (br_final_integer8 < br_cutoff_bound || br_full_integer8 == 0)) //false if undisableable led or br too low 
		{
			//disable led to keep good colors
			led->r = 0;
			led->g = 0;
			led->b = 0;
			light_avg_add += light_full / ((MATR_LEN - 1 - i) | 0b1); //add whole br
			LOG_USB_BR("dis ");
		}
		else
		{
			led->r = r_precise_final >> 16; //8.0 
			led->g = g_precise_final >> 16;
			led->b = b_precise_final >> 16;
			uint add = lerpGamma(br_final_integer8, br_final % 0xFFFF); //8.16
			light_avg_add += add / ((MATR_LEN - 1 - i) | 0b1); //add only unshowble fraction part
			LOG_USB_BR("fr %d.%04d ", (add >> 16), (add & 0xFFFF) * 10000L >> 16);
		}
		LOG_USB_BR("\n");
	}
}

void Strip::set_br(int br) {
	LOG_USB_BR("set_br - br %d\n", br);
	_br_vir = constrain(br, 0, 255 * MATR_LEN);
}

int Strip::get_max_br()
{
	return MATR_LEN * 255;
}

int Strip::get_br()
{
	return _br_vir;
}