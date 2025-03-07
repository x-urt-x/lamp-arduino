#include "BaseEffect.h"

Cutoff_str* BaseEffect::get_cutoff_str()
{
    return &_cutoff_option;
}


BaseEffect::BaseEffect(Color_str* leds_arr) : _leds_arr(leds_arr)
{
	for (int i = 0; i < IEffectBlock::BlockIDEnum::BlocksCount; i++)
	{
		_blocks[i] = nullptr;
	}
}

Cutoff_str BaseEffect::_cutoff_option
{
#ifdef MATR10x10
	{
		0,90,1,91,2,92,3,93,4,94,5,95,6,96,7,97,8,98,9,99,
		10,80,11,81,12,82,13,83,14,84,15,85,16,86,17,87,18,88,19,89,
		20,70,21,71,22,72,23,73,24,74,25,75,26,76,27,77,28,78,29,79,
		30,60,31,61,32,62,33,63,34,64,35,65,36,66,37,67,38,68,39,69,
		40,50,49,59,
		41,51,48,58,
		42,52,47,57,
		43,53,46,56,
		44,45,54,55
	}
#else
#ifdef MATR16x16
	{
		0, 240, 1, 241, 2, 242, 3, 243, 4, 244, 5, 245, 6, 246, 7, 247, 8, 248, 9, 249, 10, 250, 11, 251, 12, 252, 13, 253, 14, 254, 15, 255,
			16, 224, 17, 225, 18, 226, 19, 227, 20, 228, 21, 229, 22, 230, 23, 231, 24, 232, 25, 233, 26, 234, 27, 235, 28, 236, 29, 237, 30, 238, 31, 239,
			32, 208, 33, 209, 34, 210, 35, 211, 36, 212, 37, 213, 38, 214, 39, 215, 40, 216, 41, 217, 42, 218, 43, 219, 44, 220, 45, 221, 46, 222, 47, 223,
			48, 192, 49, 193, 50, 194, 51, 195, 52, 196, 53, 197, 54, 198, 55, 199, 56, 200, 57, 201, 58, 202, 59, 203, 60, 204, 61, 205, 62, 206, 63, 207,
			64, 176, 65, 177, 66, 178, 67, 179, 68, 180, 69, 181, 70, 182, 71, 183, 72, 184, 73, 185, 74, 186, 75, 187, 76, 188, 77, 189, 78, 190, 79, 191,
			80, 160, 81, 161, 82, 162, 83, 163, 84, 164, 85, 165, 86, 166, 87, 167, 88, 168, 89, 169, 90, 170, 91, 171, 92, 172, 93, 173, 94, 174, 95, 175,
			96, 144, 97, 145, 98, 146, 99, 147, 100, 148, 101, 149, 102, 150, 103, 151, 104, 152, 105, 153, 106, 154, 107, 155, 108, 156, 109, 157, 110, 158, 111, 159,
			112, 128, 127, 143,
			113, 129, 126, 142,
			114, 130, 125, 141,
			115, 131, 124, 140,
			116, 132, 123, 139,
			117, 133, 122, 138,
			118, 134, 121, 137,
			119, 120, 135, 136
	}
#endif // MATR16x16
#endif // MATR10x10
	,
	MATR_LEN-4
};