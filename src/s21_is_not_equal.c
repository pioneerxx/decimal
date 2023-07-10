#include "s21_decimal.h"

int s21_is_not_equal(s21_decimal first, s21_decimal second) {
  int res = 0;
  if (first.value_type == NAN_VALUE || second.value_type == NAN_VALUE) {
    res = 1;
  } else {
    res = !s21_is_equal(first, second);
  }
  return res;
}