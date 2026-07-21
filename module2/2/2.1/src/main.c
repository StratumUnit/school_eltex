#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int choice;
  struct Entry entry = {0};
  int initSize = 5;

  while (1) {
    handlePrintMenu();

    char buffer[20];

    fgets(buffer, sizeof(buffer), stdin);

    if (sscanf(buffer, "%d", &choice) != 1) {
      printf("Ошибка. Нужно ввести цифру, а не буквы\n");
      continue;
    }

    switch (choice) {

    case 1:

      handleInitPhoneBook(&entry, initSize);
      break;

    case 2:

      handleAddContact(&entry);
      break;

    case 3:

      handleDelContact(&entry);
      break;

    case 4:
      handleEditContact(&entry);
      break;

    case 5:

      printPhoneBook(&entry);
      break;
    case 6:
      handleAutoFill(&entry);
      break;

    case 7:
      free(entry.contact);
      return 1;
    }
  }

  return 1;
}
