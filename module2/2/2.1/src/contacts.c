#include "contacts.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int initPhoneBook(struct Entry *entry, int n) {
  if (entry == NULL)
    return -1;

  // выделяем память под запись контакта
  entry->contact = (struct Contact *)malloc(n * sizeof(struct Contact));

  if (entry->contact == NULL)
    return -2;

  entry->capacity = n;
  entry->initCapacity = n;
  entry->size = 0;

  for (int i = 0; i < n; i++) {

    entry->contact[i].id = 0;
  }
  return 1;
}

int addContact(struct Entry *entry, int id, const char *firstname,
               const char *lastName, const char *job, const char *phone,
               const char *country, const char *city, const char *street) {

  if (entry == NULL || entry->contact == NULL) {
    return -1;
  }

  if (getIndex(entry, id) >= 0) {
    return -3;
  }

  if (entry->size >= entry->capacity) {
    int expandResult = expandPhoneBook(entry);
    if (expandResult <= 0) {
      return expandResult;
    }
  }

  int num = entry->size;
  entry->contact[num].id = id;

  snprintf(entry->contact[num].firstName, NAME_LEN, "%s", firstname);
  snprintf(entry->contact[num].lastName, NAME_LEN, "%s", lastName);
  snprintf(entry->contact[num].job, JOB_LEN, "%s", job);
  snprintf(entry->contact[num].phoneNumber, PHONE_LEN, "%s", phone);

  snprintf(entry->contact[num].address.country, COUNTRY_LEN, "%s", country);
  snprintf(entry->contact[num].address.city, CITY_LEN, "%s", city);
  snprintf(entry->contact[num].address.street, STREET_LEN, "%s", street);

  entry->size++;
  sortBook(entry);
  return 1;
}

int expandPhoneBook(struct Entry *entry) {

  if (entry == NULL || entry->contact == NULL) {
    return -1;
  }
  int newCapacity = entry->capacity * 2;
  size_t newSize = newCapacity * sizeof(struct Contact);
  void *temp = realloc(entry->contact, newSize);
  if (temp != NULL) {
    entry->contact = temp;
    entry->capacity = newCapacity;
    return 1;
  }

  return -2;
}

int shrinkPhoneBook(struct Entry *entry) {

  if (entry == NULL || entry->contact == NULL) {
    return -1;
  }

  int newCapacity = entry->capacity / 2;

  if (newCapacity < entry->initCapacity) {
    return 0;
  }

  size_t newSize = newCapacity * sizeof(struct Contact);
  void *temp = realloc(entry->contact, newSize);
  if (temp != NULL) {
    entry->contact = temp;
    entry->capacity = newCapacity;
    return 1;
  }

  return -2;
}

int delContact(struct Entry *entry, int index) {

  if (entry == NULL || entry->contact == NULL) {
    return -1;
  }
  if (index < 0 || index >= entry->size) {
    return -2;
  }

  if (index < entry->size - 1) {

    entry->contact[index] = entry->contact[entry->size - 1];
  }
  entry->size--;

  if (entry->size <= entry->capacity / 4) {
    shrinkPhoneBook(entry);
  }
  sortBook(entry);
  return 1;
}
int printPhoneBook(struct Entry *entry) {
  if (entry == NULL || entry->contact == NULL) {
    return -1;
  }

  if (entry->size == 0) {
    printf("\nТелефонная книга пуста. Нет записей для отображения\n");
    return 0;
  }

  printf("\n=================================== ТЕЛЕФОННАЯ КНИГА "
         "===================================\n");
  printf(" Записей: %d | Текущая емкость буфера: %d мест\n", entry->size,
         entry->capacity);
  printf("====================================================================="
         "===================\n");

  printf("%-6s | %-15s | %-15s | %-15s | %-15s | %s\n", "Id", "firstName",
         "lastName", "phoneNumber", "job", "Address (Contry, City, Street)");
  printf("---------------------------------------------------------------------"
         "-------------------\n");

  for (int i = 0; i < entry->size; i++) {
    struct Contact c = entry->contact[i];

    printf("%-6d | %-15.30s | %-15.30s | %-15.30s | %-15.30s | %s, %s, %s\n",
           c.id, c.firstName, c.lastName, c.phoneNumber, c.job,
           c.address.country, c.address.city, c.address.street);

    if ((i + 1) % 20 == 0 && (i + 1) < entry->size) {
      printf("\n--- Нажмите [Enter] для отображения следующих записей ---");
      while (getchar() != '\n')
        printf("\n");
    }
  }

  printf("====================================================================="
         "===================\n");
  return 1;
}
int editContact(struct Entry *entry, int index, const char *format, ...) {

  if (entry == NULL || entry->contact == NULL) {
    return -1;
  }

  if (index < 0 || index >= entry->size)
    return -2;

  va_list ptr;

  va_start(ptr, format);
  for (const char *c = format; *c; c++) {
    switch (*c) {

    case 'i':
      entry->contact[index].id = va_arg(ptr, int);
      break;

    case 'f':
      snprintf(entry->contact[index].firstName, NAME_LEN, "%s",
               va_arg(ptr, const char *));
      break;

    case 'l':
      snprintf(entry->contact[index].lastName, NAME_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    case 'j':
      snprintf(entry->contact[index].job, JOB_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    case 'p':
      snprintf(entry->contact[index].phoneNumber, PHONE_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    case 'C':
      snprintf(entry->contact[index].address.country, COUNTRY_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    case 'c':
      snprintf(entry->contact[index].address.city, CITY_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    case 's':
      snprintf(entry->contact[index].address.street, STREET_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    default:
      break;
    }
  }
  va_end(ptr);
  sortBook(entry);
  return 1;
}

int getIndex(struct Entry *entry, int id) {
  if (entry == NULL || entry->contact == NULL) {
    return -1;
  }

  for (int i = 0; i < entry->size; i++) {
    if (id == entry->contact[i].id) {
      return i;
    }
  }
  return -2;
}

static int compareContactsByLastName(const void *a, const void *b) {
  const struct Contact *contactA = (const struct Contact *)a;
  const struct Contact *contactB = (const struct Contact *)b;

  int res = strcmp(contactA->lastName, contactB->lastName);

  // Если фамилии абсолютно одинаковые, сортируем по имени
  if (res == 0) {
    return strcmp(contactA->firstName, contactB->firstName);
  }

  return res;
}

// Сама функция сортировки всего справочника
void sortBook(struct Entry *entry) {
  if (entry == NULL || entry->contact == NULL || entry->size <= 1) {
    return; // Сортировать нечего
  }

  // Вызываем встроенную быструю сортировку qsort:
  // 1. Указатель на начало массива
  // 2. Количество реальных элементов в массиве
  // 3. Размер одной ячейки (структуры Contact)
  // 4. Указатель на нашу функцию сравнения
  qsort(entry->contact, entry->size, sizeof(struct Contact),
        compareContactsByLastName);
}
