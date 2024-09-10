#ifndef IEFFECT_H
#define IEFFECT_H
#include "Structures.h"
//��������� ��� �������� ���������. ��������� �������
class IEffect
{
public:
  virtual void setup();						//����������� ���� ��� ����� �������� �������
  virtual void make_frame();				//��������� ������ �����

  virtual Options* get_options_ptr();

  virtual const byte* get_cutoff_order();	//������� ���������� ������� �����������
  virtual const byte* get_cutoff_imm();		//����� �����������, ��� ������� ������� �� ����������� 
  virtual byte get_cutoff_order_len();		//������ _cutoff_order
  virtual byte get_cutoff_imm_len();		//������ _cutoff_imm_len

  virtual int get_preset_count();
  virtual const String* get_preset_names();
  virtual void set_preset(int num = 0);			//����������� ��������� �� ���������

  virtual String get_effect_name();
};

#endif