#include <stdlib.h>

#define NAME_LEN 40
#define JOB_LEN 100
#define PHONE_LEN 30
#define COUNTRY_LEN 60
#define CITY_LEN 50
#define STREET_LEN 100

struct Address
{

  char country[COUNTRY_LEN];
  char city[CITY_LEN];
  char street[STREET_LEN];
};

struct Contact
{
  int id;
  char firstName[NAME_LEN];
  char lastName[NAME_LEN];
  char job[JOB_LEN];
  char phoneNumber[PHONE_LEN];
  struct Address address;
};

struct TreeNode
{
  struct Contact *contact;
  struct TreeNode *left;
  struct TreeNode *right;
};

int insert(struct TreeNode **root, struct TreeNode *newNode);
void printTree(struct TreeNode *root, int lvl);
int delNode(struct TreeNode **root, const char *lastname);
int delContact(struct TreeNode **root, const char *lastname);
int findConctact(struct TreeNode **root, const char *lastname, struct TreeNode **node);
int addContact(struct TreeNode **root, int id, const char *firstname,
               const char *lastName, const char *job, const char *phone,
               const char *country, const char *city, const char *street);

int printFullContact(struct TreeNode **root, const char *lastname);
int editContact(struct TreeNode **root, const char *lastname, const char *format, ...);
int calcNodesCount(struct TreeNode **root, int *count);
int fillNodeArray(struct TreeNode *root, struct TreeNode **nodes, int *number);
int balanceTree(struct TreeNode **root);
struct TreeNode *buildTreeFromArray(struct TreeNode **nodes, int beg, int end);
void resetOpCount();