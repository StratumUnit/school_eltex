#include "menu.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void readString(char *buffer, int maxLength) {
  fgets(buffer, maxLength, stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
}

void handlePrintMenu() {

  printf("\nМеню\n");
  printf("1. Инициализировать список\n");
  printf("2. Добавить контакт\n");
  printf("3. Удалить контакт\n");
  printf("4. Редактировать контакт\n");
  printf("5. Отобразить список контактов\n");
  printf("6. Заполнить массив рандомными контактами\n");
  printf("7. Выход\n\n");
}

void handleInitPhoneBook(struct Entry *entry, int initSize) {

  int code = initPhoneBook(entry, initSize);
  if (code == -1) {

    printf("Телефонный справочник не инициализирован\n");
  } else if (code == -2) {
    printf("Ошибка выделения памяти для contacts\n");
  } else {
    printf("Успешно выделена память для %d контактов\n", initSize);
  }
}

static void bufferClear() {
  while (getchar() != '\n' && getchar() != EOF)
    ;
}

void handleAddContact(struct Entry *entry) {

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

  // Вызываем функцию из модуля contacts
  int result = addContact(entry, id, firstName, lastName, job, phoneNumber,
                          country, city, street);
  switch (result) {
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

void handleDelContact(struct Entry *entry) {

  int id;
  printf("Введите id контакта для удаления: ");
  scanf("%d", &id);
  bufferClear();

  int result = getIndex(entry, id);

  switch (result) {
  case -1:
    printf("Телефонный справочник не инициализирован\n");
    break;
  case -2:
    printf("Контакт под таким id отсутствует\n");
    break;
  default:
    if (delContact(entry, result)) {
      printf("Контакт id:%d успешно удален\n", id);
    }
  }
}

void handleEditContact(struct Entry *entry) {

  int inputId;
  printf("Введите Id контакта для изменения: ");
  if (scanf("%d", &inputId) != 1) {
    printf("Id должен быть числом\n");
    return;
  }
  bufferClear();

  int idx = getIndex(entry, inputId);

  if (idx == -2) {
    printf("Контакт с Id %d не найден в базе данных.\n", inputId);
    return;
  }

  printf("\nВыбран контакт: %s %s\n", entry->contact[idx].firstName,
         entry->contact[idx].lastName);
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
  for (int i = 0; userFormat[i] != '\0'; i++) {
    char current_char = userFormat[i];

    switch (current_char) {
    case 'i': {
      int newId;
      printf(" -> Введите новый ID: ");
      if (scanf("%d", &newId) != 1) {

        bufferClear();
        success = 0;
        break;
      }

      bufferClear();
      int checkIdx = getIndex(entry, newId);
      if (checkIdx > 0) {
        printf(" ID %d занят\n", newId);
        success = 0;
        break;
      }
      editContact(entry, idx, "i", newId);
      break;
    }
    case 'f': {
      char buf[NAME_LEN];
      printf(" -> Введите новое Имя: ");
      readString(buf, NAME_LEN);
      bufferClear();
      editContact(entry, idx, "f", buf);
      break;
    }
    case 'l': {
      char buf[NAME_LEN];
      printf(" -> Введите новую Фамилию: ");
      readString(buf, NAME_LEN);
      editContact(entry, idx, "l", buf);
      break;
    }
    case 'j': {
      char buf[JOB_LEN];
      printf(" -> Введите новую Должность: ");
      readString(buf, JOB_LEN);
      editContact(entry, idx, "j", buf);
      break;
    }
    case 'p': {
      char buf[PHONE_LEN];
      printf(" -> Введите новый Телефон: ");
      readString(buf, PHONE_LEN);
      editContact(entry, idx, "p", buf);
      break;
    }
    case 'C': {
      char buf[COUNTRY_LEN];
      printf(" -> Введите новую Страну: ");
      readString(buf, COUNTRY_LEN);
      editContact(entry, idx, "C", buf);
      break;
    }

    case 'c': {
      char buf[CITY_LEN];
      printf(" -> Введите новый Город: ");
      readString(buf, CITY_LEN);
      editContact(entry, idx, "c", buf);
      break;
    }
    case 's': {
      char buf[STREET_LEN];
      printf(" -> Введите новую Улицу: ");
      readString(buf, STREET_LEN);
      editContact(entry, idx, "s", buf);
      break;
    }

    default:
      printf("Неизвестный символ формата '%c'\n", current_char);
      break;
    }
  }

  if (success) {
    printf("\n Выбранные поля контакта успешно обновлены\n");
  }
}

void handleAutoFill(struct Entry *entry) {

  if (entry == NULL || entry->contact == NULL) {
    printf("Справочник не инициализирован!\n");
    return;
  }

  int count;
  printf("Автоматическое заполнение базы\n");
  printf("Сколько тестовых контактов сгенерировать?: ");
  if (scanf("%d", &count) != 1 || count <= 0) {
    printf("Введите корректное число больше нуля\n");
    bufferClear();
    return;
  }
  bufferClear();

  srand(time(NULL));

  const char *firstNames[] = {"Ivan",   "Petr", "Alex",  "Dmitry",
                              "Sergey", "Anna", "Elena", "Maria"};
  const char *lastNames[] = {"Ivanov", "Petrov",   "Sidorov", "Smirnov",
                             "Popov",  "Kuznecov", "Novikov"};
  const char *jobs[] = {"Developer", "Director", "Manager",
                        "Engineer",  "Designer", "Builder"};
  const char *cities[] = {"Novosibirsk", "Moscow", "St-Petersburg",
                          "Ekaterinburg", "Kazan"};
  const char *streets[] = {"Lenina", "Marksa", "Kirov", "Soviet", "New_Street"};

  int nameCount = sizeof(firstNames) / sizeof(firstNames[0]);
  int lastCount = sizeof(lastNames) / sizeof(lastNames[0]);
  int jobCount = sizeof(jobs) / sizeof(jobs[0]);
  int cityCount = sizeof(cities) / sizeof(cities[0]);
  int strCount = sizeof(streets) / sizeof(streets[0]);

  printf("Генерация контактов... \n");

  int addedSuccessfully = 0;
  for (int i = 0; i < count; i++) {
    int randId;
    do {
      randId = 1000 + rand() % 9000;
    } while (getIndex(entry, randId) >= 0);

    char randPhone[20];
    snprintf(randPhone, sizeof(randPhone), "+7999%07d", rand() % 10000000);

    const char *fName = firstNames[rand() % nameCount];
    const char *lName = lastNames[rand() % lastCount];
    const char *job = jobs[rand() % jobCount];
    const char *city = cities[rand() % cityCount];
    const char *street = streets[rand() % strCount];

    int res = addContact(entry, randId, fName, lName, job, randPhone, "Russia",
                         city, street);

    if (res == 1) {
      addedSuccessfully++;
    } else {
      printf("Генерация остановлена по ошибке %d.\n", res);
      break;
    }
  }

  printf("Успешно сгенерировано и добавлено контактов: %d\n",
         addedSuccessfully);
  printf("Текущий размер массива: %d/%d\n", entry->size, entry->capacity);
}
