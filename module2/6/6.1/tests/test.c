#include "../unity/unity.h"
#include "../src/contacts.h"
#include <stdlib.h>
#include <string.h>

struct Node *root = NULL;

void setUp(void)
{
    root = NULL;
}

void tearDown(void)
{
    if (root != NULL)
    {
        struct Node *cur = root;
        while (cur != NULL)
        {
            struct Node *next = cur->next;
            free(cur->contact);
            free(cur);
            cur = next;
        }
        root = NULL;
    }
}

void test_AddContact_ShouldSortByLastName(void)
{
    addContact(&root, 1, "Ivan", "Petrov", "Dev", "123", "RU", "MSK", "Lenina");
    addContact(&root, 2, "Alex", "Alexeev", "QA", "456", "RU", "SPB", "Mira");
    addContact(&root, 3, "Sidor", "Sidorov", "PM", "789", "RU", "NSK", "Novaya");

    TEST_ASSERT_NOT_NULL(root);
    TEST_ASSERT_EQUAL_INT(2, root->contact->id);
    TEST_ASSERT_EQUAL_STRING("Alexeev", root->contact->lastName);

    TEST_ASSERT_NOT_NULL(root->next);
    TEST_ASSERT_EQUAL_INT(1, root->next->contact->id);
    TEST_ASSERT_EQUAL_STRING("Petrov", root->next->contact->lastName);
    TEST_ASSERT_EQUAL_PTR(root, root->next->prev);

    TEST_ASSERT_NOT_NULL(root->next->next);
    TEST_ASSERT_EQUAL_INT(3, root->next->next->contact->id);
    TEST_ASSERT_EQUAL_STRING("Sidorov", root->next->next->contact->lastName);
    TEST_ASSERT_EQUAL_PTR(root->next, root->next->next->prev);
    TEST_ASSERT_NULL(root->next->next->next);
}

// Проверка удаления контактов из разных позиций списка
void test_DelContact_ShouldDeleteCorrectly(void)
{
    addContact(&root, 1, "Ivan", "Petrov", "Dev", "123", "RU", "MSK", "Lenina");
    addContact(&root, 2, "Alex", "Alexeev", "QA", "456", "RU", "SPB", "Mira");
    addContact(&root, 3, "Sidor", "Sidorov", "PM", "789", "RU", "NSK", "Novaya");

    // Из середины
    delContact(&root, 1);
    TEST_ASSERT_EQUAL_INT(2, root->contact->id);
    TEST_ASSERT_EQUAL_INT(3, root->next->contact->id);
    TEST_ASSERT_EQUAL_PTR(root, root->next->prev);

    // Из начала
    delContact(&root, 2);
    TEST_ASSERT_EQUAL_INT(3, root->contact->id);
    TEST_ASSERT_NULL(root->prev);

    // Из конца
    delContact(&root, 3);
    TEST_ASSERT_NULL(root);
}

// Проверка изменения фамилии и последующего восстановления порядка
void test_EditContact_ShouldResortWhenLastNameChanges(void)
{
    addContact(&root, 1, "Ivan", "Petrov", "Dev", "123", "RU", "MSK", "Lenina");
    addContact(&root, 2, "Alex", "Alexeev", "QA", "456", "RU", "SPB", "Mira");

    // Меняем фамилию Petrov на Aardvark

    editContact(&root, 1, "l", "Aardvark");

    // Теперь он должен переместиться в начало списка, перед Alexeev
    TEST_ASSERT_NOT_NULL(root);
    TEST_ASSERT_EQUAL_INT(1, root->contact->id);
    TEST_ASSERT_EQUAL_STRING("Aardvark", root->contact->lastName);

    // Проверяем, что вторым элементом стал Alexeev
    TEST_ASSERT_NOT_NULL(root->next);
    TEST_ASSERT_EQUAL_INT(2, root->next->contact->id);
    TEST_ASSERT_EQUAL_STRING("Alexeev", root->next->contact->lastName);

    TEST_ASSERT_EQUAL_PTR(root, root->next->prev);
    TEST_ASSERT_NULL(root->next->next);
}

void test_AddContact_ShouldReturnErrorOnNullPointer(void)
{
    int result = addContact(NULL, 1, "A", "B", "C", "D", "E", "F", "G");
    TEST_ASSERT_EQUAL_INT(-1, result);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_AddContact_ShouldSortByLastName);
    RUN_TEST(test_DelContact_ShouldDeleteCorrectly);
    RUN_TEST(test_EditContact_ShouldResortWhenLastNameChanges);
    RUN_TEST(test_AddContact_ShouldReturnErrorOnNullPointer);

    return UNITY_END();
}