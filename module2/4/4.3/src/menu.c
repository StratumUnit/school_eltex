#include "menu.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void readString(char *buffer, int maxLength)
{
  fgets(buffer, maxLength, stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
}

void handlePrintMenu()
{

  printf("\nМеню\n");
  printf("1. Добавить контакт\n");
  printf("2. Удалить контакт\n");
  printf("3. Редактировать контакт\n");
  printf("4. Отобразить список контактов\n");
  printf("5. Отобразить данные контакта\n");
  printf("6. Выход\n\n");
}

static void bufferClear()
{
  while (getchar() != '\n' && getchar() != EOF)
    ;
}

void handleAddContact(struct TreeNode **entry)
{

  char firstName[NAME_LEN];
  char lastName[NAME_LEN];
  char job[JOB_LEN];
  char phoneNumber[PHONE_LEN];
  char country[COUNTRY_LEN];
  char city[CITY_LEN];
  char street[STREET_LEN];

  int id;

  printf("Введите ID: ");
  scanf("%d", &id);

  bufferClear();

  printf("Имя: ");
  readString(firstName, NAME_LEN);

  printf("Фамилия: ");
  readString(lastName, NAME_LEN);

  printf("Работа/Должность: ");
  readString(job, JOB_LEN);

  printf("Номер телефона: ");
  readString(phoneNumber, PHONE_LEN);

  printf("Страна: ");
  readString(country, COUNTRY_LEN);

  printf("Город: ");
  readString(city, CITY_LEN);

  printf("Улица: ");
  readString(street, STREET_LEN);

  int result = addContact(entry, id, firstName, lastName, job, phoneNumber,
                          country, city, street);
  switch (result)
  {
  case -1:

    printf("Телефонный справочник не инициализирован\n");
  case -2:
    printf("Ошибка расширения памяти\n");

  case -3:
    printf("Этот Id уже занят\n");
  case 1:
    printf("Контакт id: %d успешно добавлен\n", id);
  }
}

void handleDelContact(struct TreeNode **entry)
{

  char buf[NAME_LEN];
  printf("Введите фамилию контакта для удаления: ");
  scanf("%s", buf);
  bufferClear();
  int result = delNode(entry, buf);
  printf("result: %d\n", result);
}

void handlePrintFullContact(struct TreeNode **entry)
{
  char nameBuffer[NAME_LEN];
  printf("Введите фамилию контакта для отображения: ");
  scanf("%s", nameBuffer);
  bufferClear();
  printFullContact(entry, nameBuffer);
}

void handleEditContact(struct TreeNode **entry)
{

  char nameBuffer[NAME_LEN];
  printf("Введите фамилию контакта для изменения: ");
  scanf("%s", nameBuffer);
  bufferClear();

  printf("Какое поле вы хотите изменить?\n");
  printf("i - Уникальный ID\n");
  printf("f - Имя\n");
  printf("l - Фамилия\n");
  printf("j - Место работы / Должность\n");
  printf("p - Номер телефона\n");
  printf("C - Страна\n");
  printf("c - Город проживания\n");
  printf("s - Улица\n");
  printf("----------------------------------------\n");
  printf("Введите буквы полей: ");

  char userFormat[8];

  readString(userFormat, 8);

  int success = 1;
  for (int i = 0; userFormat[i] != '\0'; i++)
  {
    char current_char = userFormat[i];

    switch (current_char)
    {
    case 'i':
    {
      int newId;
      printf(" -> Введите новый ID: ");
      if (scanf("%d", &newId) != 1)
      {

        bufferClear();
        success = 0;
        break;
      }

      bufferClear();

      editContact(entry, nameBuffer, "i", newId);

      break;
    }
    case 'f':
    {
      char buf[NAME_LEN];
      printf(" -> Введите новое Имя: ");
      readString(buf, NAME_LEN);
      editContact(entry, nameBuffer, "f", buf);

      break;
    }
    case 'l':
    {
      char buf[NAME_LEN];
      printf(" -> Введите новую Фамилию: ");
      readString(buf, NAME_LEN);
      editContact(entry, nameBuffer, "l", buf);
      strncpy(nameBuffer, buf, NAME_LEN);
      break;
    }
    case 'j':
    {
      char buf[JOB_LEN];
      printf(" -> Введите новую Должность: ");
      readString(buf, JOB_LEN);
      editContact(entry, nameBuffer, "j", buf);
      break;
    }
    case 'p':
    {
      char buf[PHONE_LEN];
      printf(" -> Введите новый Телефон: ");
      readString(buf, PHONE_LEN);
      editContact(entry, nameBuffer, "p", buf);
      break;
    }
    case 'C':
    {
      char buf[COUNTRY_LEN];
      printf(" -> Введите новую Страну: ");
      readString(buf, COUNTRY_LEN);
      editContact(entry, nameBuffer, "C", buf);
      break;
    }

    case 'c':
    {
      char buf[CITY_LEN];
      printf(" -> Введите новый Город: ");
      readString(buf, CITY_LEN);
      editContact(entry, nameBuffer, "c", buf);
      break;
    }
    case 's':
    {
      char buf[STREET_LEN];
      printf(" -> Введите новую Улицу: ");
      readString(buf, STREET_LEN);
      editContact(entry, nameBuffer, "s", buf);
      break;
    }

    default:
      printf("Неизвестный символ формата '%c'\n", current_char);
      break;
    }
  }

  if (success)
  {
    printf("\n Выбранные поля контакта успешно обновлены\n");
  }
}
