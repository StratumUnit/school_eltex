#pragma once
#include "contacts.h"

void readString(char *buffer, int maxLength);
void handlePrintMenu();
void handleAddContact(struct Node **entry);
void handleDelContact(struct Node **entry);
void handleEditContact(struct Node **entry);