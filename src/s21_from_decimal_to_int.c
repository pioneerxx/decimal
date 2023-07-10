#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = 0;
  s21_truncate(src, &src);
  if (findFirstNonZeroBit(src) > 0 || !dst) {
    res = 1;
  } else {
    long int power = 1;
    *dst = 0;
    for (int i = 0; i < 96; i++) {
      if (get_bit2(src, i)) {
        *dst += power;
      }
      power *= 2;
    }
    if (get_bit(src.bits[3], 31)) {
      *dst *= -1;
    }
  }
  return res;
}