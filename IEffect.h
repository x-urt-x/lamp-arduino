#ifndef IEFFECT_H
#define IEFFECT_H

//��������� ��� �������� ���������. ��������� �������
class IEffect
{
public:
  virtual void setup();						//����������� ���� ��� ����� �������� �������
  virtual void make_frame();				//��������� ������ �����
  virtual void set_step(int step);			//�������� ������� (��������� ������� ����� ��������� �������)
  virtual const byte* get_cutoff_order();	//������� ���������� ������� �����������
  virtual const byte* get_cutoff_imm();		//����� �����������, ��� ������� ������� �� ����������� 
  virtual byte get_cutoff_order_len();		//������ _cutoff_order
  virtual byte get_cutoff_imm_len();		//������ _cutoff_imm_len
  virtual int get_preset_count();
  virtual const String* get_preset_names();
  virtual void set_preset(int num);
};

#endif