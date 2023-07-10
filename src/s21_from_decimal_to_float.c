#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  double result = 0;
  int res = 0;
  if (!dst) {
    res = 1;
  } else if (src.value_type == INF_VALUE) {
    res = 1;
    *dst = INFINITY;
  } else if (src.value_type == N_INF_VALUE) {
    res = 1;
    *dst = -INFINITY;
  } else if (src.value_type == NAN_VALUE) {
    res = 1;
    *dst = NAN;
  } else {
    *dst = 0;
    int power = 0;
    for (int i = 0; i < 96; i++) {
      if (get_bit2(src, i)) {
        result += pow(2, power);
      }
      power++;
    }
    int scale = get_exp(src);
    while (scale) {
      result /= 10;
      scale--;
    }
    *dst = (float)result;
    if (is_neg(src)) {
      *dst *= -1;
    }
  }
  return res;
}