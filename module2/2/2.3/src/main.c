#include "calc.h"
#include <stdio.h>


#define OP_COUNT 5


struct Func{
  double (*funcPtr) (double, double);
  char *funcName;
};



int main(void) {

  int operation;
  double a, b;
  double result;
  struct Func func[OP_COUNT] = { {opSum, "Add"}, {opSub, "Subtraction"}, {opMult, "Muiltiplication"}, {opDiv, "Division"}, {opDiv, "Div2"}};
  

while(1){
    printf("---Доступные бинарные операци---\n");
    for(int i = 0; i < OP_COUNT; i++){
      printf("%d. %s\n", i+1, func[i].funcName);
    }
    printf("%d. Выход\n", OP_COUNT+1);
    printf("Введите номер операции: ");

    scanf("%d", &operation);

    if (operation == OP_COUNT+1) {
      return 1;
    }

    printf("Введите аргументы:\n");
    printf("a=");
    scanf("%lf", &a);
    printf("b=");
    scanf("%lf", &b);

    result = func[operation-1].funcPtr(a,b);

    if(isnan(result)){
      printf("Деление на ноль!!!!\n");
      continue;
    }

    printf("Результат: %.3f\n", result);
      
    
  }
return 1;
}