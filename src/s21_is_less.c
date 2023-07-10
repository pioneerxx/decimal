#include "s21_decimal.h"

int s21_is_less(s21_decimal first, s21_decimal second) {
  int res = 1;
  if (first.value_type == NAN_VALUE || second.value_type == NAN_VALUE) {
    res = 0;
  } else {
    if (s21_is_greater_or_equal(first, second)) {
      res = 0;
    }
  }
  return res;
}