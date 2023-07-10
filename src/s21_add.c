#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = fetch_err(value_1, value_2, '+');
  int norm_err = normalize(&value_1, &value_2);
  if (result && !res && !norm_err) {
    nullify(result);
    int shift, tmp, neg1, neg2, sign;
    shift = tmp = neg1 = neg2 = sign = 0;
    decabs(&value_1, &value_2, &neg1, &neg2);
    if ((neg1 && !neg2) || (!neg1 && neg2)) {
      add_or_sub(value_1, value_2, result, &res, neg1, neg2);
    } else {
      for (int i = 0; i < 96; i++) {
        shift = 0;
        shift += get_bit2(value_1, i) ? 1 : 0;
        shift += get_bit2(value_2, i) ? 1 : 0;
        shift += tmp;
        shiftInterpreter(shift, result, i, &tmp);
      }
      if (tmp && neg1 && neg2) {
        res = 2;
        result->value_type = N_INF_VALUE;
      } else if (tmp) {
        res = 1;
        result->value_type = INF_VALUE;
      } else {
        set_exp(result, get_exp(value_1));
        sign = neg1 && neg2 ? 1 : 0;
        set_bit(&(result->bits[3]), 31, sign);
      }
    }
  }
  errorInterpreter(result, res);
  return res;
}
