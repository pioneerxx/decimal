#include "s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int neg1 = 0, neg2 = 0;
  int res = fetch_err(value_1, value_2, '%');
  if (!result) {
    res = 0;
  } else if ((int_equal(value_1, 0) && is_norm(value_1) && !res) || res == 1) {
    nullify(result);
    *result = value_1;
    if (res == 1) {
      res = 0;
    }
  } else if (res == 0) {
    nullify(result);
    decabs(&value_1, &value_2, &neg1, &neg2);
    if (s21_is_equal(value_1, value_2)) {
      s21_from_int_to_decimal(0, result);
    } else {
      s21_decimal temp;
      nullify(&temp);
      if (s21_is_equal(value_2, temp)) {
        res = 3;
      }
      normalize(&value_1, &value_2);
      *result = div_dec(value_1, value_2, &temp);
      if (getsign(*result)) {
        res = 1;
        nullify(result);
      }
    }
  }
  if (result != NULL) {
    errorInterpreter(result, res);
    if (res != 3) {
      result->value_type = NORMAL_VALUE;
    } else {
      set_bit_to_0(&(result->bits[3]), 31);
    }
  }
  return res;
}
