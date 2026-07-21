#include "contacts.h"
#include "unity.h"
#include "unity_internals.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_initPhoneBook_ShouldSetCorrectInitialState(void) {
  struct Entry entry = {0};

  int result = initPhoneBook(&entry, 3);

  TEST_ASSERT_EQUAL_INT(1, result);
  TEST_ASSERT_EQUAL_INT(0, entry.size);
  TEST_ASSERT_EQUAL_INT(3, entry.capacity);
  TEST_ASSERT_NOT_NULL(entry.contact);

  free(entry.contact);
}

void test_pushContact_ShouldExpandBuffer_WhenCapacityIsReached(void) {
  struct Entry entry = {0};
  initPhoneBook(&entry, 2);

  addContact(&entry, 10, "Ivan", "Ivanov", "Dev", "111", "RU", "NSK", "St1");
  addContact(&entry, 20, "Petr", "Petrov", "CEO", "222", "RU", "NSK", "St2");
  TEST_ASSERT_EQUAL_INT(2, entry.capacity);

  int result =
      addContact(&entry, 30, "Anna", "Nova", "HR", "333", "RU", "MSK", "St3");

  TEST_ASSERT_EQUAL_INT(1, result);
  TEST_ASSERT_EQUAL_INT(3, entry.size);
  TEST_ASSERT_EQUAL_INT(4, entry.capacity);

  free(entry.contact);
}

void test_pushContact_ShouldRejectDuplicateId(void) {
  struct Entry entry = {0};
  initPhoneBook(&entry, 5);

  int res1 = addContact(&entry, 55, "Ivan", "Ivanov", "Dev", "111", "RU", "NSK",
                        "St1");
  TEST_ASSERT_EQUAL_INT(1, res1);

  int res2 = addContact(&entry, 55, "Alex", "Sidorov", "Driver", "444", "RU",
                        "SPB", "St4");

  TEST_ASSERT_EQUAL_INT(-3, res2);
  TEST_ASSERT_EQUAL_INT(1, entry.size);

  free(entry.contact);
}

void test_editContact_ShouldUpdateMultipleFieldsViaFormatString(void) {
  struct Entry entry = {0};
  initPhoneBook(&entry, 5);
  addContact(&entry, 100, "Ivan", "Ivanov", "Builder", "+7999", "Russia",
             "Novosibirsk", "Lenina");

  int result = editContact(&entry, 0, "ifj", 777, "Konstantin", "Developer");

  TEST_ASSERT_EQUAL_INT(1, result);
  TEST_ASSERT_EQUAL_INT(777, entry.contact[0].id);
  TEST_ASSERT_EQUAL_STRING("Konstantin", entry.contact[0].firstName);
  TEST_ASSERT_EQUAL_STRING("Developer", entry.contact[0].job);
  TEST_ASSERT_EQUAL_STRING("Ivanov", entry.contact[0].lastName);

  free(entry.contact);
}

void test_phoneSortBook_ShouldSortContacts(void) {
  struct Entry entry = {0};
  initPhoneBook(&entry, 5);

  addContact(&entry, 1, "Ivan", "Sidorov", "Dev", "111", "RU", "NSK", "St");
  addContact(&entry, 2, "Petr", "Alexandrov", "CEO", "222", "RU", "NSK", "St");
  addContact(&entry, 3, "Anna", "Ivanova", "HR", "333", "RU", "MSK", "St");

  sortBook(&entry);

  TEST_ASSERT_EQUAL_STRING("Alexandrov", entry.contact[0].lastName);
  TEST_ASSERT_EQUAL_STRING("Ivanova", entry.contact[1].lastName);
  TEST_ASSERT_EQUAL_STRING("Sidorov", entry.contact[2].lastName);

  free(entry.contact);
}

void test_delContact_ShouldDelete(void) {
  struct Entry entry = {0};
  initPhoneBook(&entry, 5);

  addContact(&entry, 10, "Ivan", "Ivanov", "Dev", "111", "RU", "NSK", "St1");
  addContact(&entry, 20, "Petr", "Petrov", "CEO", "222", "RU", "NSK", "St2");
  addContact(&entry, 30, "Anna", "Nova", "HR", "333", "RU", "MSK", "St3");

  int result = delContact(&entry, 1);

  TEST_ASSERT_EQUAL_INT(1, result);

  TEST_ASSERT_EQUAL_INT(2, entry.size);

  TEST_ASSERT_EQUAL_INT(20, entry.contact[1].id);
  TEST_ASSERT_EQUAL_STRING("Petr", entry.contact[1].firstName);
  TEST_ASSERT_EQUAL_INT(10, entry.contact[0].id);

  result = delContact(&entry, 1);

  TEST_ASSERT_EQUAL_INT(1, result);

  TEST_ASSERT_EQUAL_INT(1, entry.size);
  TEST_ASSERT_EQUAL_INT(10, entry.contact[0].id);

  result = delContact(&entry, getIndex(&entry, 10));

  TEST_ASSERT_EQUAL_INT(1, result);
  TEST_ASSERT_EQUAL_INT(0, entry.size);

  free(entry.contact);
}

void test_delContact_ShouldReturnError_WhenIndexIsInvalid(void) {
  struct Entry entry = {0};
  initPhoneBook(&entry, 5);
  addContact(&entry, 10, "Ivan", "Ivanov", "Dev", "111", "RU", "NSK", "St1");

  int res1 = delContact(&entry, -1);
  int res2 = delContact(&entry, 1);
  struct Entry emptyEntry = {0};
  initPhoneBook(&emptyEntry, 5);
  int res3 = delContact(&emptyEntry, 0);

  TEST_ASSERT_EQUAL_INT(-2, res1);
  TEST_ASSERT_EQUAL_INT(-2, res2);
  TEST_ASSERT_EQUAL_INT(-2, res3);

  TEST_ASSERT_EQUAL_INT(1, entry.size);

  free(entry.contact);
  free(emptyEntry.contact);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_initPhoneBook_ShouldSetCorrectInitialState);
  RUN_TEST(test_pushContact_ShouldExpandBuffer_WhenCapacityIsReached);
  RUN_TEST(test_pushContact_ShouldRejectDuplicateId);
  RUN_TEST(test_editContact_ShouldUpdateMultipleFieldsViaFormatString);
  RUN_TEST(test_phoneSortBook_ShouldSortContacts);
  RUN_TEST(test_delContact_ShouldDelete);
  RUN_TEST(test_delContact_ShouldReturnError_WhenIndexIsInvalid);

  return UNITY_END();
}
