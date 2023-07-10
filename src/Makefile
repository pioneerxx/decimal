CC = gcc
FLAGS = -std=c11 -Wall -Wextra -Werror
FLAG_COV = --coverage
LIBS = -lcheck
TEST_LIBS = -lcheck -lm -lpthread

all: gcov_report

clang:
	clang-format -n -style=google s21_*.c

s21_decimal.a:
	$(CC) $(FLAGS) -c s21_add.c s21_div.c s21_floor.c s21_from_decimal_to_float.c s21_from_decimal_to_int.c s21_from_float_to_decimal.c s21_from_int_to_decimal.c s21_is_equal.c s21_is_greater_or_equal.c s21_is_greater.c s21_is_less_or_equal.c s21_is_less.c s21_is_not_equal.c s21_mod.c s21_mul.c s21_negate.c s21_round.c s21_sub.c s21_truncate.c s21_other_funcs.c
	ar rc s21_decimal.a *.o
	ranlib s21_decimal.a

test: s21_decimal.a
	$(CC) $(FLAGS) -c s21_decimal_test.c -o s21_decimal_test.o
	$(CC) s21_decimal_test.o s21_decimal.a $(TEST_LIBS) -o s21_decimal
	./s21_decimal

install lcov:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

leak: s21_decimal.a
	$(CC) $(FLAGS) -c s21_decimal_test.c -o s21_decimal_test.o
	$(CC) s21_decimal_test.o s21_decimal.a $(TEST_LIBS) -o s21_decimal
	CK_FORK=no leaks -quiet -atExit -- ./s21_decimal

gcov_report: test
	$(CC) $(FLAGS) --coverage s21_decimal_test.c s21_add.c s21_div.c s21_floor.c s21_from_decimal_to_float.c s21_from_decimal_to_int.c s21_from_float_to_decimal.c s21_from_int_to_decimal.c s21_is_equal.c s21_is_greater_or_equal.c s21_is_greater.c s21_is_less_or_equal.c s21_is_less.c s21_is_not_equal.c s21_mod.c s21_mul.c s21_negate.c s21_round.c s21_sub.c s21_truncate.c s21_other_funcs.c s21_decimal.h s21_decimal.a $(TEST_LIBS) -o gcov_test
	chmod +x *
	./gcov_test
	lcov -t "gcovReport" -o gcov_test.info -c -d .
	genhtml -o report gcov_test.info
	open report/index.html

clean:
	rm -rf s21_decimal
	rm -rf *.o
	rm -rf *.a
	rm -rf *.gcno
	rm -rf *.gcda
	rm -rf *.gcov
	rm -rf *.info
	rm -rf *.html
	rm -rf *.css
	rm -rf test
	rm -rf gcov_test
	rm -rf ./report

rebuild: clean all