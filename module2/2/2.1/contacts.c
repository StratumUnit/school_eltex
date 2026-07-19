#include "contacts.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int initPhoneBook(struct Entry *entry, int n) {
  if (entry == NULL)
    return 0;

  // выделяем память под запись контакта
  entry->contact = (struct Contact *)malloc(n * sizeof(struct Contact));

  if (entry->contact == NULL)
    return 0;

  entry->capacity = n;
  entry->initCapacity = n;
  entry->size = 0;

  for (int i = 0; i < n; i++) {

    entry->contact[i].id = 0;
  }
  return 1;
}

int pushContact(struct Entry *entry, int id, const char *firstname,
                const char *lastName, const char *job, const char *phone,
                const char *country, const char *city, const char *street) {

  if (entry == NULL || entry->contact == NULL) {
    return 0;
  }

  if (entry->size >= entry->capacity) {
    if (!expandPhoneBook(entry)) {
      return 0;
    }
  }

  entry->size++;
  int num = entry->size - 1;
  entry->contact[num].id = id;

  snprintf(entry->contact[num].firstName, NAME_LEN, "%s", firstname);
  snprintf(entry->contact[num].lastName, NAME_LEN, "%s", lastName);
  snprintf(entry->contact[num].job, JOB_LEN, "%s", job);
  snprintf(entry->contact[num].phoneNumber, PHONE_LEN, "%s", phone);

  snprintf(entry->contact[num].address.country, COUNTRY_LEN, "%s", country);
  snprintf(entry->contact[num].address.city, CITY_LEN, "%s", city);
  snprintf(entry->contact[num].address.street, STREET_LEN, "%s", street);

  return 1;
}

int expandPhoneBook(struct Entry *entry) {

  if (entry == NULL || entry->contact == NULL) {
    return 0;
  }
  int newCapacity = entry->capacity * 2;
  size_t newSize = newCapacity * sizeof(struct Contact);
  void *temp = realloc(entry->contact, newSize);
  if (temp != NULL) {
    entry->contact = temp;
    entry->capacity = newCapacity;
    return 1;
  }

  return 0;
}

int shrinkPhoneBook(struct Entry *entry) {

  if (entry == NULL || entry->contact == NULL) {
    return 0;
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

  return 0;
}

int delContact(struct Entry *entry, int index) {

  if (entry == NULL || entry->contact == NULL) {
    return 0;
  }
  if (index < 0 || index >= entry->size) {
    return 0;
  }

  if (index < entry->size - 1) {

    entry->contact[index] = entry->contact[entry->size - 1];
  }
  entry->size--;

  if (entry->size <= entry->capacity / 4) {
    shrinkPhoneBook(entry);
  }

  return 1;
}

int printPhoneBook(struct Entry *entry) {

  if (entry == NULL || entry->contact == NULL) {
    printf("Массив пуст");
    return 0;
  }

  printf("PhoneBook\n");
  printf("%-5s | %-15s | %-15s | %-15s | %-15s | %s\n", "ID", "Имя", "Фамилия",
         "Телефон", "Работа", "Адрес (Страна, Город, Улица)");
  printf("---------------------------------------------------------------------"
         "---------------------\n");
  for (int i = 0; i < entry->size; i++) {

    struct Contact c = entry->contact[i];

    printf("%-5d | %-15s | %-15s | %-15s | %-15s | %s, %s, %s\n", c.id,
           c.firstName, c.lastName, c.phoneNumber, c.job, c.address.country,
           c.address.city, c.address.street);
  }

  return 1;
}
int editContact(struct Entry *entry, int index, const char *format, ...) {

  if (entry == NULL || entry->contact == NULL) {
    printf("error");
    return 0;
  }

  if (index < 0 || index >= entry->size)
    return 0;

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
  return -1;
}
