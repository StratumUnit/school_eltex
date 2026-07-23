#pragma once
#include "contacts.h"

void readString(char *buffer, int maxLength);
void handlePrintMenu();
//void handleInitPhoneBook(struct Node **entry, int initSize);
void handleAddContact(struct Node **entry);
void handleDelContact(struct Node **entry);
void handleEditContact(struct Node **entry);
//void handleAutoFill(struct Node *entry);
