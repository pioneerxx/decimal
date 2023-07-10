#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = fetch_err(value_1, value_2, '-');
  if (result && res == 0) {
    nullify(result);
    int neg1, neg2;
    neg1 = neg2 = 0;
    int iszero = int_equal(value_2, 0);
    int norm_err = normalize(&value_1, &value_2);
    if (int_equal(value_2, 0) && !iszero) {
      s21_from_int_to_decimal(1, &value_2);
    }
    if (!s21_is_equal(value_1, value_2) && norm_err == 0) {
      decabs(&value_1, &value_2, &neg1, &neg2);
      if ((!neg1 && neg2) || (neg1 && !neg2)) {
        sub_or_add(value_1, value_2, result, &res, neg1, neg2);
      } else {
        switchValues(&value_1, &value_2, result, neg1, neg2);
        substractCycle(value_1, value_2, result);
        set_exp(result, get_exp(value_1));
      }
    }
  }
  errorInterpreter(result, res);
  return res;
}
