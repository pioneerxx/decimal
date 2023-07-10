#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  nullify(result);
  int res = fetch_err(value_1, value_2, '/');
  int scale = 0, sign = 0, neg1 = 0, neg2 = 0;
  if (!result) {
  } else if (int_equal(value_1, 0) && is_norm(value_1) &&
             !int_equal(value_2, 0) && is_norm(value_2)) {
    *result = value_1;
  } else if (is_norm(value_1) && (is_inf(value_2) || is_ninf(value_2))) {
    if ((is_ninf(value_2) && !is_neg(value_1)) ||
        (is_inf(value_2) && is_neg(value_1))) {
      set_bit_to_1(&(result->bits[3]), 31);
    }
  } else if (res == 0) {
    sign = getsign(value_1) ^ getsign(value_2);
    decabs(&value_1, &value_2, &neg1, &neg2);
    if (s21_is_equal(value_1, value_2)) {
      s21_from_int_to_decimal(1, result);
      setsign(result, sign);
    } else {
      divideCycle(value_1, value_2, result, &res, sign, scale);
    }
  } else {
    errorInterpreter(result, res);
  }
  return res;
}
