#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

#define OP_COUNT 5

struct Func
{
  double (*funcPtr)(double, double);
  char funcName[64];
  void *handle;
};

typedef struct
{
  const char *name;
  double (*func)(double, double);
} FuncInfo;

typedef FuncInfo (*getinf)(void);

int main(void)
{

  int operation;
  double a, b;
  double result;
  struct Func func[OP_COUNT] = {0};
  int op_count = 0;

  DIR *dir = opendir("libs");
  if (!dir)
  {
    perror("Не удалось открыть папку libs");
    return 1;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL)
  {
    if (strstr(entry->d_name, ".so") != NULL)
    {
      if (op_count >= OP_COUNT)
        break;

      char path[256];
      snprintf(path, sizeof(path), "libs/%.250s", entry->d_name);

      void *handle = dlopen(path, RTLD_LAZY);
      if (!handle)
      {
        fprintf(stderr, "Ошибка dlopen: %s\n", dlerror());
        continue;
      }

      getinf get = dlsym(handle, "getInfo");
      char *error = dlerror();
      if (error != NULL)
      {
        fprintf(stderr, "Ошибка dlsym в файле %s: %s\n", entry->d_name, error);
        dlclose(handle);
        continue;
      }

      FuncInfo info = get();

      func[op_count].funcPtr = info.func;
      strncpy(func[op_count].funcName, info.name, sizeof(func[op_count].funcName));
      func[op_count].handle = handle;
      op_count++;
    }
  }

  closedir(dir);

  if (op_count == 0)
  {
    printf("Плагины не найдены в папке libs/!\n");
    return 1;
  }

  while (1)
  {
    printf("---Доступные бинарные операци---\n");
    for (int i = 0; i < op_count; i++)
    {
      printf("%d. %s\n", i + 1, func[i].funcName);
    }
    printf("%d. Выход\n", op_count + 1);
    printf("Введите номер операции: ");

    scanf("%d", &operation);

    if (operation == op_count + 1)
    {
      for (int i = 0; i < op_count; i++)
      {
        if (func[i].handle)
          dlclose(func[i].handle);
      }
      return 0;
    }

    if (operation < 1 || operation > op_count)
    {
      printf("Неверный номер операции! Попробуйте снова.\n");
      continue;
    }

    printf("Введите аргументы:\n");
    printf("a=");
    scanf("%lf", &a);
    printf("b=");
    scanf("%lf", &b);

    result = func[operation - 1].funcPtr(a, b);

    if (isnan(result))
    {
      printf("Деление на ноль!!!!\n");
      continue;
    }

    printf("Результат: %.3f\n", result);
  }
  return 1;
}