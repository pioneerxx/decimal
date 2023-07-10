#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 79228162514264337593543950335
#define MAX_UINT 4294967296
#define NORMAL_VALUE 0
#define INF_VALUE 1
#define N_INF_VALUE 2
#define NAN_VALUE 3
#define MAX_DEC powf(2.0f, 96.0f) - 1.0
#define MIN_DEC -powf(2.0f, 96.0f) + 1.0

typedef union {
  float value;
  int bits;
} sharespace;

typedef struct {
  unsigned int bits[4];
  int value_type;
} s21_decimal;

// Arithmetic Operators
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Comparison Operators
int s21_is_less(s21_decimal first, s21_decimal second);
int s21_is_less_or_equal(s21_decimal first, s21_decimal second);
int s21_is_greater(s21_decimal first, s21_decimal second);
int s21_is_greater_or_equal(s21_decimal first, s21_decimal second);
int s21_is_equal(s21_decimal first, s21_decimal second);
int s21_is_not_equal(s21_decimal first, s21_decimal second);

// Convertors and parsers
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Another functions
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// Support functions
void set_bit_to_1(unsigned int *value, int bitNum);
void set_bit_to_0(unsigned int *value, int bitNum);
void invert_bit(unsigned int *value, int bitNum);
int get_bit(unsigned int value, int bitNum);
int get_exp(s21_decimal src);
void set_exp(s21_decimal *dst, int exp);
int multiply(s21_decimal *value1, int value2);
void add(s21_decimal *value1, int value2);
int is_neg(s21_decimal value);
void from_decimal_to_decimal(s21_decimal src, s21_decimal *dst);
void nullify(s21_decimal *value);
int get_float_exp(unsigned int bits);
int findFirstNonZeroBit(s21_decimal value);
int normalize(s21_decimal *val1, s21_decimal *val2);
int s21_move(s21_decimal *value_x);
void s21_undo_move(s21_decimal *value_x);
int fetch_err(s21_decimal val1, s21_decimal val2, char operation);
int get_bit2(s21_decimal value, int bitNum);
int int_equal(s21_decimal value, int v);
int divide(s21_decimal *value1, int value2);
void set_bit_to_1_2(s21_decimal *value, int bitNum);
void set_bit_to_0_2(s21_decimal *value, int bitNum);
void errorInterpreter(s21_decimal *result, int res);
void add_or_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                int *res, int neg1, int neg2);
void decabs(s21_decimal *val1, s21_decimal *val2, int *neg1, int *neg2);
void set_bit(unsigned int *value, int bitNum, int bit);
void shiftInterpreter(int shift, s21_decimal *result, int i, int *tmp);
int check_decimal(s21_decimal value);
void truncateCycle(s21_decimal *value, int exp);
void sub_or_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                int *res, int neg1, int neg2);
void switchValues(s21_decimal *value_1, s21_decimal *value_2,
                  s21_decimal *result, int neg1, int neg2);
void substractCycle(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result);
void multiplyCycle(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result, int *res);
void divideCycle(s21_decimal value_1, s21_decimal value_2, s21_decimal *result,
                 int *res, int sign, int scale);
void normalizeInitialize(s21_decimal val1, s21_decimal val2,
                         s21_decimal *val_big, s21_decimal *val_small,
                         int *exp_big, int *exp_small);
void normalizeCycle1(s21_decimal val1, s21_decimal val2, s21_decimal *val_big,
                     s21_decimal *val_small, int exp_big, int exp_small,
                     int *res, int flag);
void normalizeCycle2(s21_decimal val1, s21_decimal val2, s21_decimal *val_big,
                     s21_decimal *val_small, int exp_big, int exp_small,
                     int *res, int flag);
void fetch_addition(s21_decimal val1, s21_decimal val2, int *res);
void fetch_substraction(s21_decimal val1, s21_decimal val2, int *res);
void fetch_multiplication(s21_decimal val1, s21_decimal val2, int *res);
void fetch_equality(s21_decimal val1, s21_decimal val2, int *res);
void fetch_greater(s21_decimal val1, s21_decimal val2, int *res);
void fetch_division(s21_decimal val1, s21_decimal val2, int *res);
void fetch_mod(s21_decimal val1, s21_decimal val2, int *res);
s21_decimal div_dec(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result);
void setsign(s21_decimal *src, int sign);
int getsign(s21_decimal src);
int is_norm(s21_decimal val);
int is_inf(s21_decimal val);
int is_ninf(s21_decimal val);
int is_nan(s21_decimal val);
int from_decimal_to_double(s21_decimal src, long double *dst);
int get_scale(s21_decimal dst);
void set_scale(s21_decimal *dst, int scale);
int get_sign(s21_decimal dst);
void set_sign(s21_decimal *dst);
int get_bit3(s21_decimal dst, int index);