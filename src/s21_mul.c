#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  nullify(result);
  int neg1 = 0, neg2 = 0;
  s21_decimal tmp;
  nullify(&tmp);
  int res = fetch_err(value_1, value_2, '*');
  if (res != 0) {
    if ((is_neg(value_1) && !is_neg(value_2)) ||
        (!is_neg(value_1) && is_neg(value_2))) {
      set_bit_to_1(&(result->bits[3]), 31);
    }
    errorInterpreter(result, res);
  } else {
    decabs(&value_1, &value_2, &neg1, &neg2);
    multiplyCycle(value_1, value_2, result, &res);
    if (res == 0) {
      if (get_exp(value_1) > get_exp(value_2)) {
        set_exp(result, get_exp(value_1));
      } else {
        set_exp(result, get_exp(value_2));
      }
      if (neg1 != neg2) {
        set_bit_to_1(&(result->bits[3]), 31);
      }
    } else {
      errorInterpreter(result, res);
    }
  }
  return res;
}
