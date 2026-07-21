#include <stdlib.h>

#define NAME_LEN 40
#define JOB_LEN 100
#define PHONE_LEN 30
#define COUNTRY_LEN 60
#define CITY_LEN 50
#define STREET_LEN 100

struct Address {

  char country[COUNTRY_LEN];
  char city[CITY_LEN];
  char street[STREET_LEN];
};

struct Contact {
  int id;
  char firstName[NAME_LEN];
  char lastName[NAME_LEN];
  char job[JOB_LEN];
  char phoneNumber[PHONE_LEN];
  struct Address address;
};

struct Entry {

  struct Contact *contact;
  int size;
  int capacity;
  int initCapacity;
};

int initPhoneBook(struct Entry *entry, int size);

int addContact(struct Entry *entry, int id, const char *firstname,
               const char *lastName, const char *job, const char *phone,
               const char *country, const char *city, const char *street);
int expandPhoneBook(struct Entry *entry);
int shrinkPhoneBook(struct Entry *entry);
int freePhoneBook(struct Entry *entry);
int printPhoneBook(struct Entry *entry);
int delContact(struct Entry *entry, int index);
int editContact(struct Entry *entry, int index, const char *format, ...);
int getIndex(struct Entry *entry, int id);
void sortBook(struct Entry *entry);
