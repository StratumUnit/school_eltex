#include <stdio.h>
#include <stdlib.h>
#include "contacts.h"
#include "menu.h"


int main(void) {
  int choice;
  struct Entry entry = {0};
  int initSize = 5;
  int id = 1;

  while (1) {
    printMenu();

    scanf("%d", &choice);
    switch (choice) {

    case 1:

      initContacts(&entry, initSize);
      //initPhoneBook(&entry, initSize);

      break;
    case 2:
      
      addContact(&entry, &id);
      //pushContact(&entry, 2113, "Ivan", "Ivanov", "Builder", "+76575634",
        //          "Russia", "Novosibirsk", "Pushkina");
      break;

    case 3:

      printf("индекс удаляемого элемента: ");
      //scanf("%d", &id);
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
