#include "s21_decimal.h"

int s21_is_greater_or_equal(s21_decimal first, s21_decimal second) {
  int res = 0;
  if (first.value_type == NAN_VALUE || second.value_type == NAN_VALUE) {
    res = 0;
  } else {
    if (s21_is_greater(first, second) || s21_is_equal(first, second)) {
      res = 1;
    }
  }
  return res;
}