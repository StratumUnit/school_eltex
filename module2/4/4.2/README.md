# Структура данных - очередь с приоритетом на базе односвязного списка
Узлы списка представлены структурой `Item`, содержащей, значение `value`, приоритет `priority` и указатель на следующий элемент `next`.
```c
struct Item {
  int value;
  unsigned char priority;
  struct Item *next;
};
```

Структура Queue содержит указатель на начало очереди `begin` и текущий размер `size`

```c
struct Queue {
  struct Item *begin;
  int size;
};
```
## Реализация
Все функции возвращают результат операции в виде
- 1 — успех
- 0 — очередь пуста/элемент не найден  

### Алгоритм Вставки
Вставка элементов в очередь происходит прямым сравнением приоритетов.
Элементы располагаются в порядке убывания приоритета ( число `priority` возрастает ).
Приоритет это целое число от 0 до 255, причем чем меньше число тем **важнее** сообщение.
```c 
while (iter != NULL) {
    if (newItem->priority < iter->priority) {
        newItem->next = iter;
        if (!prev) queue->begin = newItem; // Вставка в начало
        else prev->next = newItem;         // Вставка в середину списка
        queue->size++;
        return 1;
    }
    prev = iter;
    iter = iter->next;
}
```

### Извлечение элементов

Извлеченные элементы копируются в структуру, переданную по указателю `struct Item *result`

#### 1. Из начала очереди

В функцию передается указатель на очередь `queue`, и указатель на структуру `result` типа `Item`, в которую будет скопирован извлечённый элемент.

```c
int popFirstItem(struct Queue *queue, struct Item *result) {
  if (queue == NULL || queue->begin == NULL || result == NULL) {
    return 0;
  }
  struct Item *itemToDel = (queue->begin); //запоминаем элемент для удаления
  
  queue->begin = queue->begin->next;
  result->value = itemToDel->value;
  result->priority = itemToDel->priority;
  free(itemToDel);
  queue->size--;
  return 1;
}
```

#### 2. С заданным приоритетом

Функция производит линейный поиск первого подходящего элемента по всей длине списка.
```c
while (iter != NULL) {
    if (iter->priority == priority) {
      struct Item *itemToDel = iter;
      if (prev == NULL) {
        queue->begin = iter->next;

      } else {
        prev->next = iter->next;
      }
      result->value = itemToDel->value;
      result->priority = itemToDel->priority;
      free(itemToDel);
      queue->size--;
      return 1;
    }
    prev = iter;
    iter = iter->next;
  }
```
#### 3. С приоритетом не ниже заданного

Эта функция извлекает элемент, приоритет которого равен или выше указанного порога `priority` (что в текущей логике означает числовое значение, меньшее или равное заданному).
А так как очередь отсортирована от самого высокого приоритета к самому низкому, то для выполнения условия достаточно извлечь самый первый элемент, кроме случая, когда заданный порог оказался выше (строже) приоритета всех имеющихся в списке сообщений.
```c
int popItemWithPriorityNotLower(struct Queue *queue, unsigned char priority,
                                struct Item *result) {
  if (queue == NULL || queue->begin == NULL || result == NULL) {
    return 0;
  }

  if (queue->begin->priority > priority) {
    return 0;
  }

  return popFirstItem(queue, result);
}
```

## Тестовая программа
Тестовая программа написана на фреймворке Unity, она проверяет очередь в два этапа: строгими модульными тестами и случайной симуляцией. Перед каждым тестом функция `setUp` создает чистую очередь, а `tearDown` её удаляет, что исключает взаимовлияние тестов.

Сначала модульные тесты проверяют корректность сортировки при вставке и точечную выборку:

```c
void test_Queue_MaintainPriorityOrder(void) {
    insertItem(queue, 555, 3);
    insertItem(queue, 111, 1); // Высший приоритет (число меньше)
    
    // Проверяем, что элемент с приоритетом 1 автоматически встал в голову
    TEST_ASSERT_EQUAL_INT(1, queue->begin->priority);
}

void test_Queue_PopSpecificPriority(void) {
    insertItem(queue, 200, 4);
    struct Item result;
    
    int status = popItemWithPriority(queue, 4, &result);
    TEST_ASSERT_EQUAL_INT(1, status);
    TEST_ASSERT_EQUAL_INT(200, result.value);
}

```  
Третий модульный тест проверяет логику извлечения по порогу важности. Он контролирует, чтобы функция корректно отклоняла выборку, если приоритет в голове списка хуже (число больше) заданного порога, и успешно извлекала данные, если условие выполняется.

```c
void test_PopPriorityNotLowerThanThreshold(void) {
  insertItem(queue, 777, 3); // В голове отсортированной очереди лежит 3

  struct Item result;

  // Проверяем строгое условие выборки (порог 2, число должно быть <= 2)
  int status_fail = popItemWithPriorityNotLower(queue, 2, &result);
  TEST_ASSERT_EQUAL_INT(0, status_fail); // Должен быть возврат 0 (элемент не подходит)

  // Проверяем мягкое условие выборки (порог 4, число должно быть <= 4)
  int status_success = popItemWithPriorityNotLower(queue, 4, &result);
  TEST_ASSERT_EQUAL_INT(1, status_success); // Должен быть возврат 1 (успех)
  TEST_ASSERT_EQUAL_INT(777, result.value);
  TEST_ASSERT_EQUAL_INT(3, result.priority);
}
```

Затем запускается итоговый имитационный тест. Он воссоздает работу системы в реальных условиях, наполняя очередь случайными сообщениями с приоритетами от 0 до 255 и проверяя выборку данных по разным критериям:


```c
void test_Queue_RandomSimulation(void) {
    srand(time(NULL)); 
    
    // Имитация случайной генерации 10 сообщений
    for (int i = 0; i < 10; i++) {
        insertItem(queue, rand() % 900 + 100, rand() % 256);
    }
    
    printQueue(queue); // Выводим отсортированную цепочку для отчета

    struct Item result = {0, 0, NULL};
    int randomThreshold = rand() % 256;
    
    // Имитация выборки по случайному порогу
    if (popItemWithPriorityNotLower(queue, randomThreshold, &result)) {
        TEST_ASSERT_TRUE(result.priority <= randomThreshold);
    }
}

```


