#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  nullify(dst);
  int return_value = 0;
  if (isinf(src) || isnan(src)) {
    return_value = 1;
    if (src == INFINITY) {
      dst->value_type = 1;
    } else if (src == -INFINITY) {
      dst->value_type = 2;
    } else if (isnan(src)) {
      dst->value_type = 3;
    }
  } else {
    if (src != 0) {
      int sign = *(int *)&src >> 31;
      int exp = ((*(int *)&src & ~0x80000000) >> 23) - 127;
      double temp = (double)fabs(src);
      int off = 0;
      for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++) {
      }
      temp = round(temp);
      if (off <= 28 && (exp > -94 && exp < 96)) {
        sharespace mant = {0};
        temp = (float)temp;
        for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
        }
        mant.value = temp;
        exp = ((*(int *)&mant.value & ~0x80000000) >> 23) - 127;
        dst->bits[exp / 32] |= 1 << exp % 32;
        for (int i = exp - 1, j = 22; j >= 0; i--, j--)
          if ((mant.bits & (1 << j)) != 0) dst->bits[i / 32] |= 1 << i % 32;
        dst->bits[3] = (sign << 31) | (off << 16);
      } else {
        return_value = 1;
      }
    }
  }
  return return_value;
}
