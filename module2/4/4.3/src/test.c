#include "unity.h"
#include "contacts.h"
#include <string.h>
#include <stdlib.h>

struct TreeNode *testRoot = NULL;

void freeTestTree(struct TreeNode *node)
{
  if (node == NULL)
    return;
  freeTestTree(node->left);
  freeTestTree(node->right);
  free(node->contact);
  free(node);
}

void setUp(void)
{
  resetOpCount();
  testRoot = NULL;
}

void tearDown(void)
{
  freeTestTree(testRoot);
  testRoot = NULL;
}

void test_AddContact_ShouldInsertInCorrectBstOrder(void)
{

  int res1 = addContact(&testRoot, 1, "Ivan", "Petrov", "Dev", "111", "RU", "MSK", "Lenina");
  int res2 = addContact(&testRoot, 2, "Alex", "Alexeev", "QA", "222", "RU", "SPB", "Mira");
  int res3 = addContact(&testRoot, 3, "Sidor", "Sidorov", "PM", "333", "RU", "NSK", "Novaya");

  TEST_ASSERT_EQUAL_INT(0, res1);
  TEST_ASSERT_EQUAL_INT(0, res2);
  TEST_ASSERT_EQUAL_INT(0, res3);

  TEST_ASSERT_NOT_NULL(testRoot);
  TEST_ASSERT_EQUAL_STRING("Petrov", testRoot->contact->lastName);

  TEST_ASSERT_NOT_NULL(testRoot->left);
  TEST_ASSERT_EQUAL_STRING("Alexeev", testRoot->left->contact->lastName);

  TEST_ASSERT_NOT_NULL(testRoot->right);
  TEST_ASSERT_EQUAL_STRING("Sidorov", testRoot->right->contact->lastName);
}

void test_FindContact_ShouldFindExistingAndReturnMinusOneForMissing(void)
{
  addContact(&testRoot, 1, "Ivan", "Petrov", "Dev", "111", "RU", "MSK", "Lenina");
  addContact(&testRoot, 2, "Alex", "Alexeev", "QA", "222", "RU", "SPB", "Mira");

  struct TreeNode *foundNode = NULL;

  int resFound = findConctact(&testRoot, "Alexeev", &foundNode);
  TEST_ASSERT_EQUAL_INT(0, resFound);
  TEST_ASSERT_NOT_NULL(foundNode);
  TEST_ASSERT_EQUAL_STRING("Alex", foundNode->contact->firstName);

  struct TreeNode *missingNode = NULL;
  int resMissing = findConctact(&testRoot, "Unknown", &missingNode);
  TEST_ASSERT_EQUAL_INT(-1, resMissing);
  TEST_ASSERT_NULL(missingNode);
}

void test_DelNode_LeafNode_ShouldRemoveSuccessfully(void)
{
  addContact(&testRoot, 1, "Ivan", "Petrov", "Dev", "111", "RU", "MSK", "Lenina");
  addContact(&testRoot, 2, "Alex", "Alexeev", "QA", "222", "RU", "SPB", "Mira"); // Левый лист

  int delRes = delNode(&testRoot, "Alexeev");
  TEST_ASSERT_EQUAL_INT(0, delRes);

  TEST_ASSERT_NULL(testRoot->left);
}

void test_DelNode_NodeWithOneChild_ShouldBypassNode(void)
{
  addContact(&testRoot, 1, "Ivan", "Petrov", "Dev", "111", "RU", "MSK", "Lenina");
  addContact(&testRoot, 2, "Alex", "Alexeev", "QA", "222", "RU", "SPB", "Mira");
  addContact(&testRoot, 3, "Boris", "Borisov", "Dev", "444", "RU", "EKB", "Sad");
  printTree(testRoot, 0);

  int delRes = delNode(&testRoot, "Alexeev");
  TEST_ASSERT_EQUAL_INT(0, delRes);

  TEST_ASSERT_NOT_NULL(testRoot->left);
  TEST_ASSERT_EQUAL_STRING("Borisov", testRoot->left->contact->lastName);
}

void test_EditContact_FieldChangeWithoutLastName_ShouldKeepStructure(void)
{
  addContact(&testRoot, 1, "Ivan", "Petrov", "Dev", "111", "RU", "MSK", "Lenina");

  int editRes = editContact(&testRoot, "Petrov", "pj", "777", "Senior Dev");
  TEST_ASSERT_EQUAL_INT(1, editRes);

  struct TreeNode *found = NULL;
  findConctact(&testRoot, "Petrov", &found);
  TEST_ASSERT_NOT_NULL(found);
  TEST_ASSERT_EQUAL_STRING("777", found->contact->phoneNumber);
  TEST_ASSERT_EQUAL_STRING("Senior Dev", found->contact->job);
}

void test_EditContact_LastNameChange_ShouldMoveNodeToCorrectPosition(void)
{
  addContact(&testRoot, 1, "Ivan", "Petrov", "Dev", "111", "RU", "MSK", "Lenina");  // Корень
  addContact(&testRoot, 2, "Sidor", "Sidorov", "PM", "333", "RU", "NSK", "Novaya"); // Справа от корня

  int editRes = editContact(&testRoot, "Petrov", "l", "Alexeev");
  TEST_ASSERT_EQUAL_INT(1, editRes);

  struct TreeNode *oldNode = NULL;
  findConctact(&testRoot, "Petrov", &oldNode);
  TEST_ASSERT_NULL(oldNode);

  TEST_ASSERT_EQUAL_STRING("Sidorov", testRoot->contact->lastName);
  TEST_ASSERT_NOT_NULL(testRoot->left);
  TEST_ASSERT_EQUAL_STRING("Alexeev", testRoot->left->contact->lastName);
}

void test_PeriodicBalance_ShouldRebuildTreeToMinimalHeight(void)
{

  addContact(&testRoot, 1, "A", "Antonov", "Dev", "1", "RU", "M", "S");
  addContact(&testRoot, 2, "B", "Borisov", "Dev", "2", "RU", "M", "S");
  addContact(&testRoot, 3, "V", "Volkov", "Dev", "3", "RU", "M", "S");
  addContact(&testRoot, 4, "G", "Gromov", "Dev", "4", "RU", "M", "S");
  addContact(&testRoot, 5, "D", "Dmitriev", "Dev", "5", "RU", "M", "S");

  TEST_ASSERT_NOT_NULL(testRoot);
  TEST_ASSERT_EQUAL_STRING("Dmitriev", testRoot->contact->lastName);

  TEST_ASSERT_NOT_NULL(testRoot->left);
  TEST_ASSERT_EQUAL_STRING("Antonov", testRoot->left->contact->lastName);

  TEST_ASSERT_NOT_NULL(testRoot->left->right);
  TEST_ASSERT_EQUAL_STRING("Borisov", testRoot->left->right->contact->lastName);

  TEST_ASSERT_NOT_NULL(testRoot->right);
  TEST_ASSERT_EQUAL_STRING("Gromov", testRoot->right->contact->lastName);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_DelNode_NodeWithOneChild_ShouldBypassNode);
  RUN_TEST(test_AddContact_ShouldInsertInCorrectBstOrder);
  RUN_TEST(test_FindContact_ShouldFindExistingAndReturnMinusOneForMissing);
  RUN_TEST(test_DelNode_LeafNode_ShouldRemoveSuccessfully);

  RUN_TEST(test_EditContact_FieldChangeWithoutLastName_ShouldKeepStructure);
  RUN_TEST(test_EditContact_LastNameChange_ShouldMoveNodeToCorrectPosition);
  RUN_TEST(test_PeriodicBalance_ShouldRebuildTreeToMinimalHeight);

  return UNITY_END();
}
