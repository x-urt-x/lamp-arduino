#ifndef IEFFECT_H
#define IEFFECT_H

//интерфейс дл€ паттерна стратеги€. структура эффекта
class IEffect
{
public:
  virtual void setup();						//запускаетс€ один раз после создани€ объекта
  virtual void make_frame();				//генераци€ одного кадра
  virtual void set_step(int step);			//скорость эффекта (изменение эффекта между соседними кадрами)
  virtual const byte* get_cutoff_order();	//пор€док уменьшени€ €ркости светодиодов
  virtual const byte* get_cutoff_imm();		//набор светодиодов, дл€ которых €ркость не уменьшаетс€ 
  virtual byte get_cutoff_order_len();		//длинна _cutoff_order
  virtual byte get_cutoff_imm_len();		//длинна _cutoff_imm_len
  virtual int get_preset_count();
  virtual const String* get_preset_names();
  virtual void set_preset(int num);
};

#endif