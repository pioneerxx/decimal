#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int res = 0;
  int neg = 0;
  if (value.value_type == INF_VALUE || value.value_type == N_INF_VALUE ||
      value.value_type == NAN_VALUE) {
    from_decimal_to_decimal(value, result);
    res = 1;
  } else {
    if (is_neg(value)) {
      set_bit_to_0(&value.bits[3], 31);
      neg = 1;
    }
    if (!neg) {
      s21_truncate(value, result);
    } else {
      s21_truncate(value, result);
      add(result, 1);
      set_bit_to_1(&(result->bits[3]), 31);
    }
  }
  return res;
}