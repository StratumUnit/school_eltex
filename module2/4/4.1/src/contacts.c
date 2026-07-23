#include "contacts.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int addContact(struct Node **node, int id, const char *firstname,
               const char *lastName, const char *job, const char *phone,
               const char *country, const char *city, const char *street) {

  if(node == NULL){
    return -1;
  }
  

  struct Node* newNode = malloc(sizeof(struct Node)); 
  newNode->contact = malloc(sizeof(struct Contact));

  newNode->contact->id = id;
  snprintf(newNode->contact->firstName, NAME_LEN, "%s", firstname);
  snprintf(newNode->contact->lastName, NAME_LEN, "%s", lastName);
  snprintf(newNode->contact->job, JOB_LEN, "%s", job);
  snprintf(newNode->contact->phoneNumber, PHONE_LEN, "%s", phone);

  snprintf(newNode->contact->address.country, COUNTRY_LEN, "%s", country);
  snprintf(newNode->contact->address.city, CITY_LEN, "%s", city);
  snprintf(newNode->contact->address.street, STREET_LEN, "%s", street);
  newNode->next = NULL;
  newNode->prev = NULL;

if(*node == NULL){
    *node = newNode;
    return 1;
  }

  struct Node* tmp = *node;

  if(strcmp(newNode->contact->lastName, (*node)->contact->lastName) < 0){

    newNode->next = *node;
    (*node)->prev = newNode;
    *node = newNode;
    return 1;
  }

  while(tmp->next != NULL){

    if(strcmp(newNode->contact->lastName, tmp->next->contact->lastName) < 0){

      newNode->next = tmp->next;
      newNode->prev = tmp;
      tmp->next->prev = newNode;
      tmp->next = newNode;
      return 1;
    }
    else{
      tmp = tmp->next;
    }
  }

    tmp->next = newNode;
    newNode->prev = tmp;

  return 1;
}
int addNode(struct Node** root, struct Node* nodeToIns){

    if(root == NULL){
      return -1;
  }
  struct Contact* contact = nodeToIns->contact;
  int result = addContact(root, contact->id, contact->firstName, contact->lastName,
     contact->job, contact->phoneNumber, contact->address.country,
    contact->address.city, contact->address.street );
    return result;
}

  int delContact(struct Node **node, int id) {

    if (node == NULL) {
      return -1;
    }

    if(*node == NULL){
      return 1;
    }

    struct Node* tmp = *node;

    if((*node)->contact->id == id){
      struct Node* nodeToDelete = *node;
      (*node) = (*node)->next;
      if((*node) != NULL){

        (*node)->prev = NULL;

      }
      free(nodeToDelete->contact);
      free(nodeToDelete);
      return 1;
    }

    while(tmp->next != NULL){
      if(tmp->next->contact->id == id){
        struct Node* nodeToDelete = tmp->next;
        if(nodeToDelete->next == NULL){
          tmp->next = NULL;
        }
        else{
          tmp->next = tmp->next->next;
          nodeToDelete->next->prev = nodeToDelete->prev;
        }
        

        free(nodeToDelete->contact);
        free(nodeToDelete);
        return 1;
      }
      else {
        tmp = tmp->next;
      }
    }


    return 1;
  }


int printPhoneBook(struct Node **root) {

  if (root == NULL) {
    return -1;
  }
  if(*root == NULL) {
    printf("Список пуст");
    return -1;
  }

  printf("\n=================================== ТЕЛЕФОННАЯ КНИГА "
         "===================================\n");
  printf("====================================================================="
         "===================\n");

  printf("%-6s | %-15s | %-15s | %-15s | %-15s | %s\n", "Id", "firstName",
         "lastName", "phoneNumber", "job", "Address (Contry, City, Street)");
  printf("---------------------------------------------------------------------"
         "-------------------\n");


  struct Node* tmp = *root;

  while(tmp!=NULL){

    struct Contact* c = tmp->contact;

    printf("%-6d | %-15.30s | %-15.30s | %-15.30s | %-15.30s | %s, %s, %s\n",
           c->id, c->firstName, c->lastName, c->phoneNumber, c->job,
           c->address.country, c->address.city, c->address.street);
           tmp = tmp->next;
    }
  printf("====================================================================="
         "===================\n");
  return 1;
}


int editContact(struct Node **root, int id, const char *format, ...) {

  struct Node* editedNode = NULL;
  if (root == NULL || *root == NULL) {
    return -1;
  }
  
  int find = 0;

  struct Node* tmp = (*root);

  while(tmp != NULL){
    if(tmp->contact->id == id){
      find = 1;
      break;
    }
    else{
      tmp = tmp->next;
    }
  }

  if(!find){
    return -2;
  }

  va_list ptr;

  va_start(ptr, format);
  for (const char *c = format; *c; c++) {
    switch (*c) {

    case 'i':
      tmp->contact->id = va_arg(ptr, int);
      break;

    case 'f':
      snprintf(tmp->contact->firstName, NAME_LEN, "%s",
               va_arg(ptr, const char *));

        break; 
    case 'l':
      snprintf(tmp->contact->lastName, NAME_LEN, "%s",
               va_arg(ptr, const char *));

        editedNode = tmp;
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
  if(editedNode != NULL){
  
    if (editedNode->prev != NULL) {
        
          editedNode->prev->next = editedNode->next;
      } else {
        
          *root = editedNode->next;
      }

      if (editedNode->next != NULL) {
          editedNode->next->prev = editedNode->prev;
      }

      editedNode->next = NULL;
      editedNode->prev = NULL;

    addNode(root, editedNode);
  }
  return 1;
}