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

struct Node {
  struct Contact *contact;
  struct Node* next;
  struct Node* prev;
};

int addContact(struct Node **node, int id, const char *firstname,
               const char *lastName, const char *job, const char *phone,
               const char *country, const char *city, const char *street);

int addNode(struct Node **node, struct Node* nodeToIns);
int printPhoneBook(struct Node **entry);
int delContact(struct Node **entry, int index);
int editContact(struct Node **entry, int index, const char *format, ...);
