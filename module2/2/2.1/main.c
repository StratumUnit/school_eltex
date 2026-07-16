#include <stdio.h>
#include "contacts.h"

int main(void) {

  int choice;

	printf("Меню\n");
  printf("1. Инициализировать список\n");
  printf("2. Добавить контакт\n");
  printf("3. Удалить контакт\n");
  printf("4. Редактировать контакт\n");
  printf("5. Выход\n");

  scanf("%d", &choice); 

  while(1){

    switch (choice) {

      case 5: return 1;

    }

  }

	return 1;
}

