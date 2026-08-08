#pragma once
#include "contacts.h"

void readString(char *buffer, int maxLength);
void handlePrintMenu();
// void handleInitPhoneBook(struct Node **entry, int initSize);
void handleAddContact(struct TreeNode **entry);
void handleDelContact(struct TreeNode **entry);
void handleEditContact(struct TreeNode **entry);
void handlePrintFullContact(struct TreeNode **entry);
// void handleAutoFill(struct Node *entry);
