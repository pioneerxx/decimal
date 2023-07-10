#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int res = 0;
  nullify(result);
  if (check_decimal(value)) {
    result->value_type = value.value_type;
    res = 1;
  } else {
    long double fl_to_round = 0.0;
    long double difference = 0;
    from_decimal_to_double(value, &fl_to_round);
    fl_to_round = round(fl_to_round);
    if (fl_to_round < 0.0) {
      set_bit_to_1_2(result, 127);
      fl_to_round *= -1;
    }
    for (int i = 0; fl_to_round >= 1 && i < 96; i++) {
      fl_to_round = floor(fl_to_round) / 2;
      difference = fl_to_round - floor(fl_to_round);
      if (difference > 0.0) {
        set_bit_to_1_2(result, i);
      } else {
        set_bit_to_0_2(result, i);
      }
    }
    set_scale(result, 0);
  }
  return res;
}