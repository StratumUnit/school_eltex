#include "contacts.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPERATIONS_TO_BALANCE 5

static int opCount = 0;

int insert(struct TreeNode **node, struct TreeNode *newNode)
{

  if (*node == NULL)

  {
    *node = newNode;
    return 0;
  }

  if (strcmp(newNode->contact->lastName, (*node)->contact->lastName) < 0)
  {
    return insert(&(*node)->left, newNode);
  }
  else
  {
    return insert(&(*node)->right, newNode);
  }
}

int addContact(struct TreeNode **node, int id, const char *firstname,
               const char *lastName, const char *job, const char *phone,
               const char *country, const char *city, const char *street)
{

  if (node == NULL)
  {
    return -1;
  }

  struct TreeNode *newNode = malloc(sizeof(struct TreeNode));
  newNode->contact = malloc(sizeof(struct Contact));

  newNode->contact->id = id;
  snprintf(newNode->contact->firstName, NAME_LEN, "%s", firstname);
  snprintf(newNode->contact->lastName, NAME_LEN, "%s", lastName);
  snprintf(newNode->contact->job, JOB_LEN, "%s", job);
  snprintf(newNode->contact->phoneNumber, PHONE_LEN, "%s", phone);

  snprintf(newNode->contact->address.country, COUNTRY_LEN, "%s", country);
  snprintf(newNode->contact->address.city, CITY_LEN, "%s", city);
  snprintf(newNode->contact->address.street, STREET_LEN, "%s", street);
  newNode->left = NULL;
  newNode->right = NULL;

  int result = insert(node, newNode);

  if (result == 0)
  {
    opCount++;
    if (opCount >= OPERATIONS_TO_BALANCE)
    {
      balanceTree(node);
      opCount = 0;
    }
  }

  return result;
}

int delNode(struct TreeNode **root, const char *lastname)
{
  if (root == NULL || *root == NULL)
  {
    return -1;
  }

  int cmpRes = strcmp((*root)->contact->lastName, lastname);
  if (cmpRes < 0)
  {
    return delNode(&(*root)->right, lastname);
  }
  else if (cmpRes > 0)
  {
    return delNode(&(*root)->left, lastname);
  }
  else
  {
    struct TreeNode *forDel = (*root);
    if ((*root)->left == NULL && (*root)->right == NULL)
    {
      free(forDel->contact);
      free(forDel);
      (*root) = NULL;
    }
    else if ((*root)->left == NULL)
    {
      *root = (*root)->right;
      free(forDel->contact);
      free(forDel);
    }
    else if ((*root)->right == NULL)
    {
      *root = (*root)->left;
      free(forDel->contact);
      free(forDel);
    }
    else
    {
      struct TreeNode **successorRef = &((*root)->right);
      while ((*successorRef)->left != NULL)
      {
        successorRef = &((*successorRef)->left);
      }
      struct TreeNode *successor = *successorRef;
      free(forDel->contact);

      forDel->contact = successor->contact;

      *successorRef = successor->right;

      free(successor);
    }

    return 0;
  }
}

int delContact(struct TreeNode **root, const char *lastname)
{
  int res = delNode(root, lastname);

  if (res == 0)
  {
    opCount++;
    if (opCount >= OPERATIONS_TO_BALANCE)
    {
      balanceTree(root);
      opCount = 0;
    }
  }
  return res;
}

void printTree(struct TreeNode *root, int lvl)
{

  if (root == NULL)
  {
    return;
  }

  printTree(root->right, lvl + 1);

  for (int i = 0; i < lvl * 3; i++)
  {
    printf(" ");
  }

  printf("%d:%s\n", root->contact->id, root->contact->lastName);

  printTree(root->left, lvl + 1);
}

int printFullContact(struct TreeNode **root, const char *lastname)
{

  if (root == NULL || *root == NULL)
  {
    return -1;
  }

  struct TreeNode *foundNode = NULL;
  findConctact(root, lastname, &foundNode);

  if (foundNode == NULL)
  {
    return -2;
  }

  printf("%-6s | %-15s | %-15s | %-15s | %-15s | %s\n", "Id", "firstName",
         "lastName", "phoneNumber", "job", "Address (Contry, City, Street)");
  printf("---------------------------------------------------------------------"
         "-------------------\n");
  struct Contact *c = foundNode->contact;

  printf("%-6d | %-15.30s | %-15.30s | %-15.30s | %-15.30s | %s, %s, %s\n",
         c->id, c->firstName, c->lastName, c->phoneNumber, c->job,
         c->address.country, c->address.city, c->address.street);
  return 0;
}

int findConctact(struct TreeNode **root, const char *lastname, struct TreeNode **node)
{
  struct TreeNode **ptr = node;
  if (root == NULL || *root == NULL)
  {

    *node = NULL;
    return -1;
  }

  int cmpRes = strcmp((*root)->contact->lastName, lastname);
  if (cmpRes == 0)
  {
    *node = *root;
    return 0;
  }
  else if (cmpRes < 0)
  {
    return findConctact(&((*root)->right), lastname, ptr);
  }
  else
  {
    return findConctact(&((*root)->left), lastname, ptr);
  }
  return -1;
}

int editContact(struct TreeNode **root, const char *lastname, const char *format, ...)
{

  if (root == NULL || *root == NULL)
  {
    return -1;
  }

  struct TreeNode *foundNode = NULL;
  findConctact(root, lastname, &foundNode);

  if (foundNode == NULL)
  {
    return -2;
  }
  printf("Контакт найден %s\n", foundNode->contact->lastName);
  struct TreeNode *tmp = foundNode;
  va_list ptr;

  va_start(ptr, format);
  char newLastName[NAME_LEN];
  int lastNameChanged = 0;
  for (const char *c = format; *c; c++)
  {
    switch (*c)
    {

    case 'i':
      tmp->contact->id = va_arg(ptr, int);
      break;

    case 'f':
      snprintf(tmp->contact->firstName, NAME_LEN, "%s",
               va_arg(ptr, const char *));

      break;
    case 'l':
      snprintf(newLastName, NAME_LEN, "%s",
               va_arg(ptr, const char *));
      lastNameChanged = 1;
      break;
    case 'j':
      snprintf(tmp->contact->job, JOB_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    case 'p':
      snprintf(tmp->contact->phoneNumber, PHONE_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    case 'C':
      snprintf(tmp->contact->address.country, COUNTRY_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    case 'c':
      snprintf(tmp->contact->address.city, CITY_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    case 's':
      snprintf(tmp->contact->address.street, STREET_LEN, "%s",
               va_arg(ptr, const char *));
      break;
    default:
      break;
    }
  }
  va_end(ptr);

  if (lastNameChanged)
  {
    struct Contact oldContact = *(tmp->contact);
    snprintf(oldContact.lastName, NAME_LEN, "%s", newLastName);
    delNode(root, tmp->contact->lastName);
    addContact(root, oldContact.id, oldContact.firstName, oldContact.lastName,
               oldContact.job, oldContact.phoneNumber, oldContact.address.country,
               oldContact.address.city, oldContact.address.street);
  }
  return 1;
}

int calcNodesCount(struct TreeNode **root, int *count)
{

  if (root == NULL || *root == NULL)
  {
    return -1;
  }
  (*count)++;
  calcNodesCount(&(*root)->left, count);
  calcNodesCount(&(*root)->right, count);
  return 0;
}

int fillNodeArray(struct TreeNode *root, struct TreeNode **nodes, int *number)
{
  if (root == NULL)
  {
    return -1;
  }

  fillNodeArray(root->left, nodes, number);
  nodes[*number] = root;
  (*number)++;
  fillNodeArray(root->right, nodes, number);
  return 0;
}

struct TreeNode *buildTreeFromArray(struct TreeNode **nodes, int beg, int end)
{
  if (nodes == NULL)
  {
    return NULL;
  }

  if (beg > end)
  {
    return NULL;
  }
  int mid = (end - beg) / 2 + beg;
  struct TreeNode *root = nodes[mid];

  root->left = buildTreeFromArray(nodes, beg, mid - 1);
  root->right = buildTreeFromArray(nodes, mid + 1, end);
  return root;
}

int balanceTree(struct TreeNode **root)
{

  if (root == NULL || *root == NULL)
  {
    return -1;
  }
  int nodeCount = 0;
  calcNodesCount(root, &nodeCount);
  if (nodeCount == 0)
  {
    return -1;
  }

  struct TreeNode **nodes = malloc(sizeof(struct TreeNode *) * nodeCount);
  int num = 0;
  fillNodeArray(*root, nodes, &num);
  *root = buildTreeFromArray(nodes, 0, nodeCount - 1);
  free(nodes);
  return 0;
}

void resetOpCount()
{
  opCount = 0;
}