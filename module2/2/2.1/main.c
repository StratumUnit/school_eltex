#include "contacts.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

  int choice;
  struct Entry entry = {0};
  int initSize = 5;
  int id;

  while (1) {
    printf("Меню\n");
    printf("1. Инициализировать список\n");
    printf("2. Добавить контакт\n");
    printf("3. Удалить контакт\n");
    printf("4. Редактировать контакт\n");
    printf("6. Отобразить спсиок контактов\n");
    printf("7. Выход\n");
    scanf("%d", &choice);
    switch (choice) {

    case 1:

      initPhoneBook(&entry, initSize);

      break;
    case 2:

      pushContact(&entry, 2113, "Ivan", "Ivanov", "Builder", "+76575634",
                  "Russia", "Novosibirsk", "Pushkina");
      break;

    case 3:

      printf("индекс удаляемого элемента: ");
      scanf("%d", &id);
      delContact(&entry, id);
      break;

    case 4:
      editContact(&entry, 1, "ij", 993, "proger");
      break;

    case 6:

      printPhoneBook(&entry);
      break;

    case 7:
      free(entry.contact);
      return 1;
    }
  }

  return 1;
}
