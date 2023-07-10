#include "s21_decimal.h"

void set_bit_to_1(unsigned int *value, int bitNum) { *value |= (1 << bitNum); }

void set_bit_to_1_2(s21_decimal *value, int bitNum) {
  int mask = 1 << bitNum % 32;
  value->bits[bitNum / 32] |= mask;
}

void set_bit_to_0(unsigned int *value, int bitNum) { *value &= ~(1 << bitNum); }

void set_bit_to_0_2(s21_decimal *value, int bitNum) {
  int mask = 1 << bitNum % 32;
  value->bits[bitNum / 32] &= ~mask;
}

int get_bit3(s21_decimal dst, int index) {
  int mask = 1u << (index % 32);
  return (dst.bits[index / 32] & mask) != 0;
}

void set_bit(unsigned int *value, int bitNum, int bit) {
  int mask = 1 << bitNum % 32;
  if (bit) {
    *value |= mask;
  } else {
    *value &= ~mask;
  }
}

int from_decimal_to_double(s21_decimal src, long double *dst) {
  long double temp = (double)*dst;
  for (int i = 0; i < 96; i++) {
    temp += get_bit3(src, i) * pow(2, i);
  }
  temp = temp * pow(10, -get_scale(src));
  if (get_bit3(src, 127)) temp = temp * (-1);
  *dst = temp;
  return 0;
}

void setbit(s21_decimal *value, int bitNum) {
  int place = bitNum / 32;
  bitNum %= 32;
  value->bits[place] |= (1 << bitNum);
}

void invert_bit(unsigned int *value, int bitNum) { *value ^= (1 << bitNum); }

int get_bit(unsigned int value, int bitNum) { return value & (1 << bitNum); }

int get_bit2(s21_decimal value, int bitNum) {
  int mask = 1 << bitNum % 32;
  return value.bits[bitNum / 32] & mask;
}

int is_neg(s21_decimal value) {
  int res = 0;
  if (get_bit(value.bits[3], 31)) {
    res = 1;
  }
  return res;
}

int get_scale(s21_decimal dst) {
  int mask = 127 << 16;
  int scale = (mask & dst.bits[3]) >> 16;
  return scale;
}

void set_scale(s21_decimal *dst, int scale) {
  int sign = 0;
  sign = get_sign(*dst);
  dst->bits[3] = 0;
  scale <<= 16;
  dst->bits[3] = scale | dst->bits[3];
  if (sign) set_sign(dst);
}

int get_sign(s21_decimal dst) { return (dst.bits[3] & 1u << 31) != 0; }

void set_sign(s21_decimal *dst) { dst->bits[3] = dst->bits[3] | 1u << 31; }

int get_exp(s21_decimal src) {
  int exp = 0;
  int power = 1;
  for (int j = 16; j < 24; j++) {
    if (get_bit(src.bits[3], j)) {
      exp += power;
    }
    power *= 2;
  }
  return exp;
}

void set_exp(s21_decimal *dst, int exp) {
  int i = 0;
  for (int j = 16; j < 24; j++) {
    if (get_bit(exp, i)) {
      set_bit_to_1(&(dst->bits[3]), j);
    } else {
      set_bit_to_0(&(dst->bits[3]), j);
    }
    i++;
  }
}

int multiply(s21_decimal *value1, int value2) {
  s21_decimal tmp;
  int res = 0;
  s21_from_int_to_decimal(value2, &tmp);
  res = s21_mul(*value1, tmp, value1);
  return res;
}

int divide(s21_decimal *value1, int value2) {
  s21_decimal tmp;
  int res = 0;
  s21_from_int_to_decimal(value2, &tmp);
  res = s21_div(*value1, tmp, value1);
  return res;
}

void add(s21_decimal *value1, int value2) {
  s21_decimal tmp;
  s21_from_int_to_decimal(value2, &tmp);
  s21_add(*value1, tmp, value1);
}

int int_equal(s21_decimal value, int v) {
  s21_decimal tmp;
  s21_from_int_to_decimal(v, &tmp);
  return s21_is_equal(value, tmp);
}

void from_decimal_to_decimal(s21_decimal src, s21_decimal *dst) {
  nullify(dst);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 32; j++) {
      if (get_bit(src.bits[i], j)) {
        set_bit_to_1(&(dst->bits[i]), j);
      }
    }
  }
  dst->value_type = src.value_type;
}

void nullify(s21_decimal *value) {
  memset(&(value->bits), 0, sizeof(value->bits));
  value->value_type = NORMAL_VALUE;
}

int get_float_exp(unsigned int bits) {
  int exp = 0;
  int ind = 23;
  int power = 1;
  while (ind < 31) {
    if (get_bit(bits, ind)) {
      exp += power;
    }
    ind++;
    power *= 2;
  }
  return exp - 127;
}

int findFirstNonZeroBit(s21_decimal value) {
  int res = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 32; j++) {
      if (get_bit(value.bits[i], j)) {
        res = i;
        break;
      }
    }
  }
  return res;
}

void normalizeInitialize(s21_decimal val1, s21_decimal val2,
                         s21_decimal *val_big, s21_decimal *val_small,
                         int *exp_big, int *exp_small) {
  *exp_big = get_exp(val1) > get_exp(val2) ? get_exp(val1) : get_exp(val2);
  *exp_small = get_exp(val1) < get_exp(val2) ? get_exp(val1) : get_exp(val2);
  *val_big = get_exp(val1) > get_exp(val2) ? val1 : val2;
  *val_small = get_exp(val1) < get_exp(val2) ? val1 : val2;
}

void normalizeCycle1(s21_decimal val1, s21_decimal val2, s21_decimal *val_big,
                     s21_decimal *val_small, int exp_big, int exp_small,
                     int *res, int flag) {
  while (exp_small != exp_big) {
    *res = multiply(val_small, 10);
    if (*res != 0) {
      if (!flag) {
        normalizeInitialize(val1, val2, val_big, val_small, &exp_big,
                            &exp_small);
        normalizeCycle2(val1, val2, val_big, val_small, exp_big, exp_small, res,
                        1);
      }
      break;
    }
    exp_small++;
    set_exp(val_small, exp_small);
  }
}

void normalizeCycle2(s21_decimal val1, s21_decimal val2, s21_decimal *val_big,
                     s21_decimal *val_small, int exp_big, int exp_small,
                     int *res, int flag) {
  while (exp_small != exp_big) {
    *res = divide(val_big, 10);
    if (*res != 0) {
      if (!flag) {
        normalizeInitialize(val1, val2, val_big, val_small, &exp_big,
                            &exp_small);
        normalizeCycle1(val1, val2, val_big, val_small, exp_big, exp_small, res,
                        1);
      }
      break;
    }
    exp_big--;
    set_exp(val_big, exp_big);
  }
}

int normalize(s21_decimal *val1, s21_decimal *val2) {
  int res = 0;
  int exp_big = 0, exp_small = 0;
  s21_decimal val_big, val_small;
  normalizeInitialize(*val1, *val2, &val_big, &val_small, &exp_big, &exp_small);
  if (exp_big != exp_small) {
    if (exp_small + (exp_big - exp_small) < 29) {
      normalizeCycle1(*val1, *val2, &val_big, &val_small, exp_big, exp_small,
                      &res, 0);
    } else {
      normalizeCycle2(*val1, *val2, &val_big, &val_small, exp_big, exp_small,
                      &res, 0);
    }
    if (res == 0) {
      if (get_exp(*val1) > get_exp(*val2)) {
        *val1 = val_big;
        *val2 = val_small;
      } else {
        *val1 = val_small;
        *val2 = val_big;
      }
    }
  }
  return res;
}

int s21_move(s21_decimal *value_x) {
  int buff1 = 0;
  int buff2 = 0;
  int res = get_bit(value_x->bits[2], 31);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 32; j++) {
      buff2 = get_bit((value_x->bits[i]), j);
      if (buff1)
        set_bit_to_1(&value_x->bits[i], j);
      else
        set_bit_to_0(&value_x->bits[i], j);
      buff1 = buff2;
    }
  }
  return res;
}

void s21_undo_move(s21_decimal *value_x) {
  int buff1 = 0;
  int buff2 = 0;
  for (int i = 2; i >= 0; i--) {
    for (int j = 31; j >= 0; j--) {
      buff2 = get_bit((value_x->bits[i]), j);
      if (buff1)
        set_bit_to_1(&value_x->bits[i], j);
      else
        set_bit_to_0(&value_x->bits[i], j);
      buff1 = buff2;
    }
  }
}

void fetch_addition(s21_decimal val1, s21_decimal val2, int *res) {
  if (is_inf(val1) && (is_inf(val2) || is_norm(val2))) {
    *res = 1;
  } else if (is_inf(val2) && (is_inf(val1) || is_norm(val1))) {
    *res = 1;
  } else if (is_ninf(val1) && !is_inf(val2) && !is_nan(val2)) {
    *res = 2;
  } else if (is_ninf(val2) && !is_inf(val1) && !is_nan(val1)) {
    *res = 2;
  } else if (is_nan(val1) || is_nan(val2) || (is_inf(val1) && is_ninf(val2))) {
    *res = 3;
  } else if (is_nan(val2) || is_nan(val1) || (is_inf(val2) && is_ninf(val1))) {
    *res = 3;
  }
}

void fetch_substraction(s21_decimal val1, s21_decimal val2, int *res) {
  if ((is_norm(val1) || is_ninf(val1)) && is_inf(val2)) {
    *res = 2;
  } else if ((is_norm(val2) || is_ninf(val2)) && is_inf(val1)) {
    *res = 1;
  } else if ((is_norm(val1) || is_inf(val1)) && is_ninf(val2)) {
    *res = 1;
  } else if ((is_norm(val2) || is_inf(val2)) && is_ninf(val1)) {
    *res = 2;
  } else if (is_nan(val1) || is_nan(val2) || (is_ninf(val1) && is_ninf(val2)) ||
             (is_inf(val1) && is_inf(val2))) {
    *res = 3;
  } else if (is_nan(val2) || is_nan(val1) || (is_ninf(val2) && is_ninf(val1)) ||
             (is_inf(val2) && is_inf(val1))) {
    *res = 3;
  }
}

void fetch_multiplication(s21_decimal val1, s21_decimal val2, int *res) {
  if (is_nan(val1) || is_nan(val2)) {
    *res = 3;
  } else if (int_equal(val1, 0) && (is_inf(val2) || is_ninf(val2))) {
    *res = 3;
  } else if (int_equal(val2, 0) && (is_inf(val1) || is_ninf(val1))) {
    *res = 3;
  } else if (is_inf(val1) && is_norm(val2)) {
    *res = is_neg(val2) ? 2 : 1;
  } else if (is_inf(val2) && is_norm(val1)) {
    *res = is_neg(val1) ? 2 : 1;
  } else if (is_ninf(val1) && is_norm(val2)) {
    *res = is_neg(val2) ? 1 : 2;
  } else if (is_ninf(val2) && is_norm(val1)) {
    *res = is_neg(val1) ? 1 : 2;
  } else if (is_inf(val1) && is_inf(val2)) {
    *res = 1;
  } else if (is_ninf(val1) && is_ninf(val2)) {
    *res = 1;
  } else if (is_inf(val1) && is_ninf(val2)) {
    *res = 2;
  } else if (is_inf(val2) && is_ninf(val1)) {
    *res = 2;
  }
}

void fetch_equality(s21_decimal val1, s21_decimal val2, int *res) {
  if ((is_inf(val1) && is_inf(val2)) || (is_ninf(val1) && is_ninf(val2))) {
    *res = 1;
  } else {
    *res = 0;
  }
}

void fetch_greater(s21_decimal val1, s21_decimal val2, int *res) {
  if (is_nan(val1) || is_nan(val2)) {
    *res = 0;
  } else if (is_inf(val1) && !is_inf(val2)) {
    *res = 1;
  } else if (!is_inf(val1) && is_inf(val2)) {
    *res = 0;
  } else if (is_inf(val1) && is_inf(val2)) {
    *res = 0;
  } else if (is_ninf(val1) && !is_ninf(val2)) {
    *res = 0;
  } else if (!is_ninf(val1) && is_ninf(val2)) {
    *res = 1;
  } else if (is_ninf(val1) && is_ninf(val2)) {
    *res = 0;
  }
}

void fetch_division(s21_decimal val1, s21_decimal val2, int *res) {
  if (is_nan(val1) || is_nan(val2)) {
    *res = 3;
  } else if (int_equal(val1, 0) && int_equal(val2, 0)) {
    *res = 3;
  } else if (!int_equal(val1, 0) && is_norm(val1) && is_norm(val2) &&
             int_equal(val2, 0)) {
    *res = is_neg(val1) ? 2 : 1;
  } else if ((is_inf(val1) || is_ninf(val1)) &&
             (is_inf(val2) || is_ninf(val2))) {
    *res = 3;
  } else if (is_inf(val1) && is_norm(val2)) {
    *res = is_neg(val2) ? 2 : 1;
  } else if (is_ninf(val1) && is_norm(val2)) {
    *res = is_neg(val2) ? 1 : 2;
  }
}

void fetch_mod(s21_decimal val1, s21_decimal val2, int *res) {
  if (is_nan(val1) || is_nan(val2)) {
    *res = 3;
  } else if (int_equal(val1, 0) && is_norm(val1) && is_norm(val2) &&
             int_equal(val2, 0)) {
    *res = 3;
  } else if (!int_equal(val1, 0) && is_norm(val1) && is_norm(val2) &&
             int_equal(val2, 0)) {
    *res = 3;
  } else if (is_norm(val1) && (is_inf(val2) || is_ninf(val2))) {
    *res = 1;
  } else if ((is_inf(val1) || is_ninf(val1)) && is_norm(val2)) {
    *res = 3;
  } else if ((is_inf(val1) || is_ninf(val1)) &&
             (is_inf(val2) || is_ninf(val2))) {
    *res = 3;
  }
}

int fetch_err(s21_decimal val1, s21_decimal val2, char operation) {
  int res = 0;
  if (operation == '+') {
    fetch_addition(val1, val2, &res);
  } else if (operation == '-') {
    fetch_substraction(val1, val2, &res);
  } else if (operation == '>') {
    fetch_greater(val1, val2, &res);
  } else if (operation == '=') {
    fetch_equality(val1, val2, &res);
  } else if (operation == '*') {
    fetch_multiplication(val1, val2, &res);
  } else if (operation == '/') {
    fetch_division(val1, val2, &res);
  } else if (operation == '%') {
    fetch_mod(val1, val2, &res);
  }
  return res;
}

void errorInterpreter(s21_decimal *result, int res) {
  switch (res) {
    case 1: {
      result->value_type = INF_VALUE;
      break;
    }
    case 2: {
      result->value_type = N_INF_VALUE;
      set_bit_to_1(&(result->bits[3]), 31);
      break;
    }
    case 3: {
      result->value_type = NAN_VALUE;
      set_bit_to_1(&(result->bits[3]), 31);
      break;
    }
  }
  if (result == NULL) {
    res = 1;
  }
}

void add_or_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                int *res, int neg1, int neg2) {
  if (neg1 && !neg2) {
    if (s21_is_greater(value_1, value_2)) {
      *res = s21_sub(value_1, value_2, result);
      set_bit_to_1(&(result->bits[3]), 31);
    } else {
      *res = s21_sub(value_2, value_1, result);
    }
  } else if (!neg1 && neg2) {
    if (s21_is_greater(value_1, value_2)) {
      *res = s21_sub(value_1, value_2, result);
    } else {
      *res = s21_sub(value_2, value_1, result);
      set_bit_to_1(&(result->bits[3]), 31);
    }
  }
}

void sub_or_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                int *res, int neg1, int neg2) {
  *res = s21_add(value_1, value_2, result);
  if (neg1 && !neg2) {
    *res = *res == 1 ? 2 : *res;
    set_bit_to_1(&(result->bits[3]), 31);
  }
}

void decabs(s21_decimal *val1, s21_decimal *val2, int *neg1, int *neg2) {
  if (is_neg(*val1)) {
    *neg1 = 1;
    set_bit_to_0(&(val1->bits[3]), 31);
  }
  if (is_neg(*val2)) {
    *neg2 = 1;
    set_bit_to_0(&(val2->bits[3]), 31);
  }
}

void shiftInterpreter(int shift, s21_decimal *result, int i, int *tmp) {
  if (shift == 1) {
    set_bit_to_1_2(result, i);
    *tmp = 0;
  } else if (shift == 2) {
    *tmp = 1;
  } else if (shift == 3) {
    set_bit_to_1_2(result, i);
  }
}

int check_decimal(s21_decimal value) {
  int res = 0;
  if (value.value_type == INF_VALUE || value.value_type == N_INF_VALUE ||
      value.value_type == NAN_VALUE) {
    res = 1;
  }
  return res;
}

void switchValues(s21_decimal *value_1, s21_decimal *value_2,
                  s21_decimal *result, int neg1, int neg2) {
  s21_decimal tmp;
  if (s21_is_greater(*value_2, *value_1)) {
    from_decimal_to_decimal(*value_1, &tmp);
    from_decimal_to_decimal(*value_2, value_1);
    from_decimal_to_decimal(tmp, value_2);
    if (!neg1 && !neg2) {
      set_bit_to_1(&(result->bits[3]), 31);
    }
  } else if (neg1 && neg2) {
    set_bit_to_1(&(result->bits[3]), 31);
  }
}

void truncateCycle(s21_decimal *value, int exp) {
  int coef = 0;
  unsigned long long max = 0;
  while (exp) {
    max = value->bits[2];
    coef = max % 10;
    value->bits[2] = max / 10;
    max = coef * MAX_UINT + value->bits[1];
    coef = max % 10;
    value->bits[1] = max / 10;
    max = coef * MAX_UINT + value->bits[0];
    value->bits[0] = max / 10;
    exp--;
  }
}

void substractCycle(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result) {
  int tmp_i = 0;
  for (int i = 0; i < 96; i++) {
    if (get_bit2(value_1, i) && !get_bit2(value_2, i)) {
      set_bit_to_1_2(result, i);
    } else if (!get_bit2(value_1, i) && get_bit2(value_2, i)) {
      tmp_i = i + 1;
      while (!get_bit2(value_1, tmp_i)) {
        set_bit_to_1_2(&value_1, tmp_i);
        tmp_i++;
      }
      set_bit_to_0_2(&value_1, tmp_i);
      set_bit_to_1_2(result, i);
    }
  }
}

void multiplyCycle(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result, int *res) {
  int cycle = 0;
  s21_decimal tmp;
  for (int i = 0; i < 96; i++) {
    if (get_bit2(value_1, i)) {
      from_decimal_to_decimal(value_2, &tmp);
      for (int n = 0; n < cycle && *res == 0; n++) {
        *res = s21_add(tmp, tmp, &tmp);
      }
      if (*res != 0) {
        break;
      }
      *res = s21_add(*result, tmp, result);
      if (*res != 0) {
        break;
      }
      nullify(&tmp);
    }
    if (*res != 0) {
      break;
    }
    cycle++;
  }
}

void divideCycle(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                 int *res, int sign, int scale) {
  int normerr = normalize(&value_1, &value_2);
  if (!normerr) {
    s21_decimal temp, ten;
    nullify(&temp);
    nullify(&ten);
    s21_from_int_to_decimal(10, &ten);
    if (s21_is_equal(value_2, temp)) {
      *res = 3;
    }
    if (s21_is_less(value_1, value_2)) {
      while (scale < 7) {
        *res = s21_mul(value_1, ten, &value_1);
        div_dec(value_1, value_2, result);
        scale++;
      }
    }
    div_dec(value_1, value_2, result);
    setsign(result, sign);
    set_exp(result, scale);
  }
}

int getsign(s21_decimal src) {
  int sign_mark = 0;
  if ((src.bits[3] >> 31)) {
    sign_mark = 1;
  }
  return sign_mark;
}

void setsign(s21_decimal *src, int sign) {
  if (sign) {
    setbit(src, 127);
  } else {
    set_bit_to_0_2(src, 127);
  }
}

s21_decimal div_dec(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result) {
  int res = 0;
  nullify(result);
  s21_decimal fmod, temp, zero;
  nullify(&fmod);
  nullify(&temp);
  nullify(&zero);
  s21_decimal copy_1 = value_1;
  if (s21_is_equal(zero, value_2)) {
    res = 1;
  } else {
    if (s21_is_greater_or_equal(value_1, value_2)) {
      setbit(&temp, 0);
      while (!s21_is_less(value_1, value_2)) {
        s21_decimal copy_2 = value_2;
        while (s21_is_greater(value_1, copy_2)) {
          s21_move(&copy_2);
          s21_move(&temp);
        }
        if (s21_is_greater(copy_2, value_1)) {
          s21_undo_move(&copy_2);
          s21_undo_move(&temp);
        }
        s21_sub(value_1, copy_2, &value_1);
        s21_add(*result, temp, result);
        nullify(&temp);
        setbit(&temp, 0);
      }
      s21_decimal temp1;
      nullify(&temp);
      nullify(&temp1);
      s21_mul(*result, value_2, &temp);
      s21_sub(copy_1, temp, &temp1);
      if (s21_is_greater(temp1, value_2)) {
        setbit(result, 0);
      }
    }
    s21_mul(*result, value_2, &temp);
    s21_sub(copy_1, temp, &fmod);
    if (res) {
      setsign(&fmod, 1);
    }
  }
  return fmod;
}

int is_norm(s21_decimal val) {
  int res = 0;
  if (val.value_type == NORMAL_VALUE) {
    res = 1;
  }
  return res;
}

int is_inf(s21_decimal val) {
  int res = 0;
  if (val.value_type == INF_VALUE) {
    res = 1;
  }
  return res;
}

int is_ninf(s21_decimal val) {
  int res = 0;
  if (val.value_type == N_INF_VALUE) {
    res = 1;
  }
  return res;
}

int is_nan(s21_decimal val) {
  int res = 0;
  if (val.value_type == NAN_VALUE) {
    res = 1;
  }
  return res;
}