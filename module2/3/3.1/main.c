#include "chmod.h"

int main() {

  int option;
  int bits = 0;

  while (1) {

    printf("1. Ввести права доступа\n");
    printf("2. Узнать права доступа файла\n");
    printf("3. Изменить права доступа\n");
    printf("9. Выход\n");
    printf("Выберете действие: ");
    scanf("%d", &option);
    getchar();
    switch (option) {

    case 1: {
      char buf[10];
      printf("Введите права доступа в числовом или буквенном формате: ");
      if (fgets(buf, 10, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
        parsePermissionBits(buf, &bits);
      }
      break;
    }

    case 2: {
      char buf[100];
      printf("Введите имя файла: ");
      if (fgets(buf, 100, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
        getFilePermissions(buf, &bits);
      }

      break;
    }
    case 3: {

      char buf[100];
      printf("Введите команду: ");
      if (fgets(buf, 100, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
        changePermissions(buf, &bits);
      }

      break;
    }
    case 9:
      return 0;

    default:
      printf("Неопределённая операция\n");
    }
  }
  return 0;
}
