#ifndef EFFECT_FIRE_H
#define EFFECT_FIRE_H

#include <Arduino.h>
#include "Structures.h"
#include "IEffect.h"

class Effect_fire : public IEffect
{
public:
  Effect_fire(Color_str* leds_arr);
  void setup() override;
  void make_frame() override;

  Options* get_options_ptr() override;
  
  const byte* get_cutoff_order() override;
  const byte* get_cutoff_imm() override;
  byte get_cutoff_order_len() override;
  byte get_cutoff_imm_len() override;

  int get_preset_count() override;
  const String* get_preset_names() override;
  void set_preset(int num = 0) override;

  String get_effect_name() override;

private:
  Color_str *_leds_arr;

  uint32_t temp_to_color(byte temp); //переход из температуры в реальный цвет
  void dic_map_key_gen();		//создание ключевого кадра. учитывает прошлое положение и температуру центра огня
  int dic_map_cur_step();		//приблежает на 1 каждое значение из текущего кадра к ключевому. возвращает разницу между ними
  void temp_map_gen();			//создает карту температур на основе текущего кадра

  byte _dic_map_key[10][10];	//ключевой кадр с центрами диапазона для генерации _temp_map 
  byte _side_coef_key[10];		//построчный коэфициент наклона огня для ключевого кадра
  byte _dic_map_cur[10][10];	//текущий кадр
  byte _side_coef_cur[10];		//коэфы для текущего кадра
  byte _temp_map[10][10];		//карта температур для каждого пикселя

  byte _center_pos;				//позиция ценра огня
  int _center_temp_change;		//смещение диапазона изменения температуры центра 
  byte _center_temp;			//температура центра огня
  byte _frame_count;			//отсчет кадров после генерации ключевого кадра, считает вниз от FRAMES

  static Options _options;
  static bool _options_ini;

  static const byte _cutoff_order[98];
  static const byte _cutoff_imm[2];
  static const int _cutoff_order_len = 98;
  static const int _cutoff_imm_len = 2;

  static const String _preset_names[3];
  static const int _preset_len = 3;

  static const String _name;
};

#endif