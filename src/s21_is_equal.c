#include "s21_decimal.h"

int s21_is_equal(s21_decimal first, s21_decimal second) {
  int res = 1;
  int norm_err = normalize(&first, &second);
  if (first.value_type == NAN_VALUE || second.value_type == NAN_VALUE ||
      norm_err) {
    res = 0;
  } else if (first.value_type == INF_VALUE || first.value_type == N_INF_VALUE ||
             second.value_type == INF_VALUE ||
             second.value_type == N_INF_VALUE) {
    res = fetch_err(first, second, '=');
  } else {
    for (int i = 0; i < 128; i++) {
      if (get_bit2(first, i) != get_bit2(second, i)) {
        res = 0;
        break;
      }
    }
  }
  return res;
}