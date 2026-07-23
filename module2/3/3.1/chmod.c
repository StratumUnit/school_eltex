#include "chmod.h"

int parsePermissionBits(char *permissions, int *bits) {

  if (permissions == NULL)
    return -1;

  char *endptr;
  long val = strtol(permissions, &endptr, 8);

  if (endptr != permissions && *endptr == '\0') {
    *bits = (int)val;
    maskToStr(*bits);
    return 1;
  }

  if (strlen(permissions) != 9) {
    return -1;
  }
  *bits = 0;
  char rightPerm[] = "rwxrwxrwx";
  for (int i = 0; i < 9; i++) {
    if (permissions[i] == rightPerm[i]) {
      *bits |= (1 << (8 - i));
    } else if (permissions[i] != '-') {
      printf("Недопустимый символ %c в формате\n", permissions[i]);
      return -1;
    }
  }

  maskToStr(*bits);

  return 1;
}

int maskToStr(int mask) {
  int bit;
  for (int i = 8; i >= 0; i--) {
    bit = 1 & (mask >> i);
    printf("%d", bit);
  }
  printf("\n");
  return 1;
}
void printPermissions(int bits) {
  printf("Access: %04o/", bits & 0777);

  char chars[] = "rwxrwxrwx";

  for (int i = 0; i < 9; i++) {
    mode_t mask = 0400 >> i;
    printf((bits & mask) ? "%c" : "-", chars[i]);
  }
  printf("/");
  maskToStr((int)(bits & 0777));
}

int getFilePermissions(char *filepath, int *bits) {

  struct stat fileInfo;
  if (stat(filepath, &fileInfo) != 0)
    return -1;
  mode_t mode = fileInfo.st_mode;

  printPermissions((int)mode);
  *bits = (int)(mode & 0777);
  return 1;
}

int changePermissions(char *cmd, int *bits) {

  char *endptr;
  long val = strtol(cmd, &endptr, 8);

  if (endptr != cmd && *endptr == '\0') {
    *bits = (int)val;
    printPermissions(*bits);
    return 1;
  }

  char *str = cmd;
  char *token = strtok(str, ",");
  int usersBitMask = 0;
  char op;
  int permissions = 0;
  while (token != NULL) {

    while (*token != '\0') {

      while (*token == 'u' || *token == 'g' || *token == 'o' || *token == 'a') {
        if (*token == 'u')
          usersBitMask |= 07 << 6;

        if (*token == 'g')
          usersBitMask |= 07 << 3;

        if (*token == 'o')
          usersBitMask |= 07 << 0;

        if (*token == 'a')
          usersBitMask |= 0777;
        token++;
      }

      op = *token;
      if (op == '+' || op == '-' || op == '=') {
        token++;
      }

      while (*token == 'r' || *token == 'w' || *token == 'x') {
        if (*token == 'r')
          permissions |= 04; // чтение
        if (*token == 'w')
          permissions |= 02; // запись
        if (*token == 'x')
          permissions |= 01; // выполнение
        token++;
      }
      // token++;
    }

    if (op == '+') {
      *bits |= usersBitMask &
               (permissions << 6 | permissions << 3 | permissions << 0);
    } else if (op == '-') {
      *bits &= ~(usersBitMask &
                 (permissions << 6 | permissions << 3 | permissions << 0));
    } else {
      *bits = usersBitMask &
              (permissions << 6 | permissions << 3 | permissions << 0);
    }

    // printf("%s", token);

    token = strtok(NULL, ",");
  }
  printPermissions(*bits);
  return 1;
}
