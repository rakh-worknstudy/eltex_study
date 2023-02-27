#include "eltex_calc.h"
#include <stdio.h>

long double calc_add(long double *result, long double *current) {
  *result += *current;
  return *result;
}
long double calc_sub(long double *result, long double *current) {
  *result -= *current;
  return *result;
}
long double calc_mul(long double *result, long double *current) {
  *result *= *current;
  return *result;
}
long double calc_div(long double *result, long double *current) {
  *result /= *current;
  return *result;
}

int menu(void) {
  long double result = 0.0;
  long double current = 0.0;
  while (1) {
    printf("Текущее значение: %Lf\n", result);
    printf("1 - Прибавить\n" \
           "2 - Вычесть\n"   \
           "3 - Умножить\n"  \
           "4 - Разделить\n" \
           "5 - Сброс\n"     \
           "0 - Выход\n");
    char tempmore = getchar();
    if (tempmore == '0')
      break;
    if (tempmore == '5') {
      result = 0.0L;
      continue;
    }
    if (tempmore > '5' || tempmore < '0')
      continue;
    printf("n = ");
    //  string = eltex_string_get(&string);
    if (scanf("%Lf", &current) == 1) {
      switch (tempmore) {
        case '1':
          calc_add(&result, &current);
          break;
        case '2':
          calc_sub(&result, &current);
          break;
        case '3':
          calc_mul(&result, &current);
          break;
        case '4':
          calc_div(&result, &current);
          break;
        default:
          break;
      }
    } else {
      printf("Плохой ввод\n");
    }
    getchar();
  }
}