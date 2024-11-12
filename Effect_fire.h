#ifndef EFFECT_FIRE_H
#define EFFECT_FIRE_H

#include <Arduino.h>
#include "Structures.h"
#include "Effect_bases.h"

class Effect_fire : public Effectable, public Colorable, public Preseteble
{
public:
	Effect_fire(Color_str* leds_arr);

	void setup() override;
	void make_frame() override;
	String get_effect_name() override;
	
	void apply_default_option() override;

	byte* get_cutoff_order() override;
	byte* get_cutoff_imm() override;
	byte get_cutoff_order_len() override;
	byte get_cutoff_imm_len() override;

	//Preseteble
	void set_preset(int num = 0) override;

private:
	Color_str* _leds_arr;

	static IEffect::ParentBaseIDs _parent_base_IDs;
	IEffect::ParentBaseIDs* get_parent_base_ids() override;

	static Option_effect _option_effect;
	inline Option_effect* get_option_effect() override;

	static Option_color _option_color;
	inline Option_color* get_option_color() override;

	static Option_preset _option_preset;
	inline Option_preset* get_option_preset() override;

	uint32_t temp_to_color(byte temp); //������� �� ����������� � �������� ����
	void dic_map_key_gen();		//�������� ��������� �����. ��������� ������� ��������� � ����������� ������ ����
	int dic_map_cur_step();		//���������� �� 1 ������ �������� �� �������� ����� � ���������. ���������� ������� ����� ����
	void temp_map_gen();			//������� ����� ���������� �� ������ �������� �����

	byte _dic_map_key[10][10];	//�������� ���� � �������� ��������� ��� ��������� _temp_map 
	byte _side_coef_key[10];		//���������� ���������� ������� ���� ��� ��������� �����
	byte _dic_map_cur[10][10];	//������� ����
	byte _side_coef_cur[10];		//����� ��� �������� �����
	byte _temp_map[10][10];		//����� ���������� ��� ������� �������

	byte _center_pos;				//������� ����� ����
	int _center_temp_change;		//�������� ��������� ��������� ����������� ������ 
	byte _center_temp;			//����������� ������ ����
	byte _frame_count;			//������ ������ ����� ��������� ��������� �����, ������� ���� �� FRAMES

	static byte _cutoff_order[98];
	static byte _cutoff_imm[2];
	static const int _cutoff_order_len = 98;
	static const int _cutoff_imm_len = 2;
};

#endif