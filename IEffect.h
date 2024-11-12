#ifndef IEFFECT_H
#define IEFFECT_H

class IEffect
{
public:
	virtual void setup() = 0;
	virtual void make_frame() = 0;

	enum BaseIDEnum : byte
	{
		EffectableID,
		ColorableID,
		PresetebleID,
		RainbowbleID,
		BaseCount
	};
	static size_t BaseSize_arr[BaseIDEnum::BaseCount];
	struct ParentBaseIDs
	{
		BaseIDEnum* arr;
		byte len;
	};
	template <typename BaseClass>
	BaseClass* castToBase()
	{
		BaseIDEnum base_class_ID = BaseClass::BaseID;
		ParentBaseIDs* parent_base_IDs = get_parent_base_ids();
		size_t offset = 0;
		for (int i = 0; i < parent_base_IDs->len; i++)
		{
			if (parent_base_IDs->arr[i] == base_class_ID)
			{
				return reinterpret_cast<BaseClass*>(reinterpret_cast<uint8_t*>(this) + offset);
			}
			else
			{
				if (BaseSize_arr[parent_base_IDs->arr[i]] == 1)
				{
					offset += 4; //compiler optimization for virtual classes
				}
				else
				{
					offset += BaseSize_arr[parent_base_IDs->arr[i]];
				}
			}
		}
		return nullptr;
	}

private:
	virtual ParentBaseIDs* get_parent_base_ids() = 0;
};
#endif
