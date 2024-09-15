#ifndef IEFFECT_H
#define IEFFECT_H
#include "Structures.h"
//��������� ��� �������� ���������. ��������� �������
class IEffect
{
public:
  virtual void setup()=0;						//����������� ���� ��� ����� �������� �������
  virtual void make_frame()=0;				//��������� ������ �����

  virtual Options* get_options_ptr()=0;

  virtual const byte* get_cutoff_order()=0;	//������� ���������� ������� �����������
  virtual const byte* get_cutoff_imm()=0;		//����� �����������, ��� ������� ������� �� ����������� 
  virtual byte get_cutoff_order_len()=0;		//������ _cutoff_order
  virtual byte get_cutoff_imm_len()=0;		//������ _cutoff_imm_len

  virtual int get_preset_count()=0;
  virtual const String* get_preset_names()=0;
  virtual void set_preset(int num = 0)=0;			//����������� ��������� �� ���������

  virtual String get_effect_name()=0;
};

#endif