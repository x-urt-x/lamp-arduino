#include "Strip_class.h"

void Strip::apply_br() {

	float br_max = float(_br_vir) / effect->get_cutoff_str()->_cutoff_units; //базовая яркость, 
	uint8_t br_cutoff_count = effect->get_cutoff_str()->_cutoff_order_len - (_br_vir + effect->get_cutoff_str()->_cutoff_order_len) % effect->get_cutoff_str()->_cutoff_units; //количество с яркостью br_max-1
	int br_cutoff_bound = effect->get_br_cutoff_bound();
	
	LOG_USB_BR("apply_br - _main_color %02X%02X%02X\n", options->main_color.r, options->main_color.g, options->main_color.b);
	LOG_USB_BR("apply_br - _br_vir %d\n", _br_vir);
	LOG_USB_BR("apply_br - br_max %f\n", br_max);
	LOG_USB_BR("apply_br - br_cutoff_count %d\n", br_cutoff_count);
	LOG_USB_BR("apply_br - _br_cutoff_bound %d\n", options->br_cutoff_bound);
	LOG_USB_BR("apply_br - _cur_cutoff_units %d\n", _cur_cutoff_units);
	LOG_USB_BR("apply_br - _cutoff_option->_cutoff_order_len %d\n", _cutoff_option->_cutoff_order_len);
	LOG_USB_BR("apply_br - _cutoff_option->_cutoff_imm_len %d\n", _cutoff_option->_cutoff_imm_len);

	float br_add = 0;
	int to_process = effect->get_cutoff_str()->_cutoff_order_len + effect->get_cutoff_str()->_cutoff_imm_len; //счетчик необработанных светодиодов 
	float br_avg_per = 0;
	for (byte i = 0; i < effect->get_cutoff_str()->_cutoff_order_len; i++)
	{
		Color_str* led = _leds_arr + effect->get_cutoff_str()->_cutoff_order[i]; //текущий светодиод для обработки
		LOG_USB_BR("main  cyc i=%d\t", i);
		if (br_cutoff_count > 0)
			//уменьшаем яркость на 1 
		{
			LOG_USB_BR("cutoff\t");
			br_avg_per = (float(led->sum()) * byte_d(br_max - 1) + br_add) / 765; //точная яркость с учетом цвета
			led->map((byte_round_up(byte_d(br_max - 1)) + br_add)); //дробная часть br_max уже учитена в br_cutoff_count, дробная часть br_add учтется ниже
			br_cutoff_count--;
		}
		else
			//оставляем базовую
		{
			LOG_USB_BR("full\t");
			br_avg_per = (float(led->sum()) * br_max + br_add) / 765; //точная яркость с учетом цвета
			led->map((byte_round_up(br_max) + br_add)); //дробная часть br_max уже учитена в br_cutoff_count, дробная часть br_add учтется ниже
		}

		to_process--;
		br_add += (br_add - int(br_add)) / to_process; //переносим дробную часть на оставшиеся
		if (led->sum() <= br_cutoff_bound)
			//отключаем светодиод для сохранения корректных оттенков
		{
			LOG_USB_BR("dis\t");
			br_add += br_avg_per / to_process; //переносим его яркость на оставшиеся
			//и отключаем
			led->r = 0;
			led->g = 0;
			led->b = 0;
		}
		LOG_USB_BR("|##| br_cutoff_count= %d br_add= %f br_avg_per= %f\n", br_cutoff_count, br_add, br_avg_per);
	}

	for (byte i = 0; i < effect->get_cutoff_str()->_cutoff_imm_len; i++) //i - индекс текущего светодиода, порядок определен константой 
	{
		LOG_USB_BR("centr cyc i=%d\t", i);
		LOG_USB_BR("full\t");
		_leds_arr[effect->get_cutoff_str()->_cutoff_imm[i]].map((byte_round_up(br_max) + br_add));
		to_process--;
		br_add += (br_add - int(br_add)) / to_process; //переносим дробную часть на оставшиеся
		LOG_USB_BR("\n");
	}
}

void Strip::set_br(int br) {
	LOG_USB_BR("set_br - br %d\n", br);
	_br_vir = constrain(br, 0, 255 * effect->get_cutoff_str()->_cutoff_units);
}

int Strip::get_br()
{
	return _br_vir;
}


