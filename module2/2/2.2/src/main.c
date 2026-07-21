#include "calc.h"
#include <stdio.h>

int main(void) {

  int operation;
  double a, b;
  double result;
  while (1) {

    printf("---Доступные бинарные операци---\n");
    printf("1. Сложение +\n");
    printf("2. Вычитание -\n");
    printf("3. Умножение *\n");
    printf("4. Деление /\n");
    printf("5. Выход\n");
    printf("Введите номер операции: ");
    scanf("%d", &operation);

    if (operation == 5) {
      return 1;
    }

    printf("Введите аргументы:\n");
    printf("a=");
    scanf("%lf", &a);
    printf("b=");
    scanf("%lf", &b);

    switch (operation) {

    case 1:
      result = opSum(a, b);
      printf("Результат: %.3f + %.3f = %.3f\n", a, b, result);
      break;

    case 2:
      result = opSub(a, b);
      printf("Результат: %.3f - %.3f = %.3f\n", a, b, result);
      break;
    case 3:
      result = opMult(a, b);
      printf("Результат: %.3f * %.3f = %.3f\n", a, b, result);
      break;
    case 4:
      result = opDiv(a, b);
      if (isnan(result)) {
        printf("Деление на ноль\n");
        continue;
      }

      printf("Результат: %.3f / %.3f = %.3f\n", a, b, result);
      break;
    }
  }

  return 1;
}
