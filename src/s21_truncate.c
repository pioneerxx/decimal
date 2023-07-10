#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  if (check_decimal(value)) {
    from_decimal_to_decimal(value, result);
    res = 1;
  } else {
    int exp = get_exp(value);
    int neg1 = 0;
    if (is_neg(value)) {
      set_bit_to_0(&value.bits[3], 31);
      neg1 = 1;
    }
    if (exp != 0) {
      truncateCycle(&value, exp);
      from_decimal_to_decimal(value, result);
      set_exp(result, 0);
    } else {
      from_decimal_to_decimal(value, result);
    }
    if (neg1) {
      set_bit_to_1(&result->bits[3], 31);
    }
  }
  return res;
}
