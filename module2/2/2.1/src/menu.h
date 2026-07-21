#pragma once
#include "contacts.h"

void readString(char *buffer, int maxLength);
void handlePrintMenu();
void handleInitPhoneBook(struct Entry *entry, int initSize);
void handleAddContact(struct Entry *entry);
void handleDelContact(struct Entry *entry);
void handleEditContact(struct Entry *entry);
void handleAutoFill(struct Entry *entry);
