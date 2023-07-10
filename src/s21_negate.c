#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  nullify(result);
  if (value.value_type == NAN_VALUE) {
    res = 1;
    result->value_type = NAN_VALUE;
  } else {
    from_decimal_to_decimal(value, result);
    if (value.value_type == INF_VALUE) {
      result->value_type = N_INF_VALUE;
    } else if (value.value_type == N_INF_VALUE) {
      result->value_type = INF_VALUE;
    }
    invert_bit(&(result->bits[3]), 31);
  }
  return res;
}