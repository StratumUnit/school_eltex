#include "queue.h"
#include "unity.h"
#include "unity_internals.h"
#include <time.h>

#define VERBOSE_TESTS 1

static struct Queue *queue = NULL;

void setUp(void) { initQueue(&queue); }

void tearDown(void) {
  if (queue != NULL) {
    clearQueue(queue);
    free(queue);
    queue = NULL;
  }
}
void test_RandomSimulation() {
  srand(time(NULL));

  if (VERBOSE_TESTS) {
    printf("\nСлучайная симуляция\n");
  }

  int itemsCount = 10;
  if (VERBOSE_TESTS) {
    printf("Генерация %d случайных сообщений:\n", itemsCount);
  }
  for (int i = 0; i < itemsCount; i++) {
    int randomValue = rand() % 900 + 100;
    int randomPriority = rand() % 255 + 1;

    insertItem(queue, randomValue, randomPriority);
    if (VERBOSE_TESTS) {
      printf(" Сгенерировано: Значение = %d, Приоритет = %d\n", randomValue,
             randomPriority);
    }
  }

  TEST_ASSERT_EQUAL_INT(itemsCount, queue->size);

  if (VERBOSE_TESTS) {
    printf("\nИтоговая очередь:");
    printQueue(queue);
  }

  struct Item result = {0, 0, NULL};

  int randomThreshold = rand() % 255 + 1; // Случайный порог от 1 до 5
  if (VERBOSE_TESTS) {

    printf("\nПробуем извлечь элемент с приоритетом не ниже %d (число "
           "<= %d)\n",
           randomThreshold, randomThreshold);
  }
  int status_thresh =
      popItemWithPriorityNotLower(queue, randomThreshold, &result);
  if (status_thresh) {
    if (VERBOSE_TESTS) {

      printf("Успех: Извлечен элемент со значением %d и приоритетом %d\n",
             result.value, result.priority);
    }
    // Проверяем, что код нас не обманул и условие выполнено
    TEST_ASSERT_TRUE(result.priority <= randomThreshold);
  } else {
    if (VERBOSE_TESTS) {

      printf("В очереди нет элементов с приоритетом <= %d\n", randomThreshold);
    }
  }

  int randomTarget = rand() % 5 + 1; // Случайный целевой приоритет
  if (VERBOSE_TESTS) {

    printf("\nПробуем извлечь элемент с приоритетом %d\n", randomTarget);
  }
  int status_spec = popItemWithPriority(queue, randomTarget, &result);
  if (status_spec) {
    if (VERBOSE_TESTS) {

      printf("Успех: Извлечен элемент со значением %d\n", result.value);
    }
    TEST_ASSERT_EQUAL_INT(randomTarget, result.priority);
  } else {
    if (VERBOSE_TESTS) {

      printf("Элемент с точным приоритетом %d отсутствовал в "
             "очереди\n",
             randomTarget);
    }
  }

  if (queue->size > 0) {
    int expectedValue = queue->begin->value;
    int expectedPriority = queue->begin->priority;
    if (VERBOSE_TESTS) {

      printf("\nИзвлекаем самый первый элемент очереди\n");
    }
    int status_first = popFirstItem(queue, &result);

    TEST_ASSERT_EQUAL_INT(1, status_first);
    TEST_ASSERT_EQUAL_INT(expectedValue, result.value);
    TEST_ASSERT_EQUAL_INT(expectedPriority, result.priority);
    if (VERBOSE_TESTS) {

      printf("Успех: Извлечен первый элемент. Значение = %d, Приоритет = "
             "%d\n",
             result.value, result.priority);
    }
  }
  if (VERBOSE_TESTS) {

    printf("\nОчередь после манипуляций");
    printQueue(queue);
  }
}

void test_MaintainPriorityOrder(void) {
  insertItem(queue, 555, 3); // Важность 3
  insertItem(queue, 111, 1); // Важность 1 (Самый приоритетный в вашем случае)
  insertItem(queue, 999, 5); // Важность 5 (Низший приоритет)
  insertItem(queue, 222, 2); // Важность 2

  TEST_ASSERT_EQUAL_INT(4, queue->size);

  struct Item *current = queue->begin;
  TEST_ASSERT_NOT_NULL(current);
  TEST_ASSERT_EQUAL_INT(1, current->priority);
  TEST_ASSERT_EQUAL_INT(111, current->value);

  current = current->next;
  TEST_ASSERT_EQUAL_INT(2, current->priority);

  current = current->next;
  TEST_ASSERT_EQUAL_INT(3, current->priority);

  current = current->next;
  TEST_ASSERT_EQUAL_INT(5, current->priority);
}

void test_PopSpecificPriority(void) {
  insertItem(queue, 100, 2);
  insertItem(queue, 200, 4); // Будем извлекать этот
  insertItem(queue, 300, 5);

  struct Item result;
  int status_fail = popItemWithPriority(queue, 99, &result);
  TEST_ASSERT_EQUAL_INT(0, status_fail); // Должен вернуть 0 (неудача)

  int status_success = popItemWithPriority(queue, 4, &result);
  TEST_ASSERT_EQUAL_INT(1, status_success);  // Успех
  TEST_ASSERT_EQUAL_INT(200, result.value);  // Значение совпало
  TEST_ASSERT_EQUAL_INT(4, result.priority); // Приоритет совпал
  TEST_ASSERT_EQUAL_INT(2, queue->size);     // Размер уменьшился
}

void test_PopPriorityNotLowerThanThreshold(void) {
  insertItem(queue, 777, 3); // В голове отсортированной очереди лежит 3

  struct Item result;

  int status_fail = popItemWithPriorityNotLower(queue, 2, &result);
  TEST_ASSERT_EQUAL_INT(0, status_fail);

  int status_success = popItemWithPriorityNotLower(queue, 4, &result);
  TEST_ASSERT_EQUAL_INT(1, status_success);
  TEST_ASSERT_EQUAL_INT(777, result.value);
  TEST_ASSERT_EQUAL_INT(3, result.priority);
}

int main(void) {

  UNITY_BEGIN();

  RUN_TEST(test_MaintainPriorityOrder);
  RUN_TEST(test_PopSpecificPriority);
  RUN_TEST(test_PopPriorityNotLowerThanThreshold);
  RUN_TEST(test_RandomSimulation);
  return UNITY_END();
}
