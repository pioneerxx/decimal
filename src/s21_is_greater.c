#include "s21_decimal.h"

int s21_is_greater(s21_decimal first, s21_decimal second) {
  int res = 0;
  int neg1 = 0;
  int neg2 = 0;
  int norm_err = normalize(&first, &second);
  if (first.value_type == NAN_VALUE || second.value_type == NAN_VALUE ||
      s21_is_equal(first, second)) {
    res = 0;
  } else if (first.value_type == INF_VALUE || first.value_type == N_INF_VALUE ||
             second.value_type == INF_VALUE ||
             second.value_type == N_INF_VALUE) {
    res = fetch_err(first, second, '>');
  } else if (!norm_err) {
    if (get_bit(first.bits[3], 31)) {
      neg1 = 1;
      set_bit_to_0(&first.bits[3], 31);
    }
    if (get_bit(second.bits[3], 31)) {
      neg2 = 1;
      set_bit_to_0(&second.bits[3], 31);
    }
    for (int i = 95; i > -1; i--) {
      if (get_bit2(first, i) && !get_bit2(second, i)) {
        res = 1;
        break;
      }
      if (!get_bit2(first, i) && get_bit2(second, i)) {
        break;
      }
    }
    if (res && neg1 && !neg2) {
      res = 0;
    } else if (!res && !neg1 && neg2) {
      res = 1;
    } else if (!res && neg1 && neg2) {
      res = 1;
    } else if (res && neg1 && neg2) {
      res = 0;
    }
  }
  return res;
}