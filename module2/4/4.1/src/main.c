#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int choice;
  struct Node *node = NULL;


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

      handleAddContact(&node);
      break;

    case 2:

      handleDelContact(&node);
      break;

    case 3:
     handleEditContact(&node);
      break;

    case 4:

      printPhoneBook(&node);
      break;

    case 5:
      if(node != NULL){
        free(node->contact);
      free(node);
      }
      return 1;
    }
  }

  return 1;
}
