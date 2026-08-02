#include "queue.h"

int main(void) {

  struct Queue *queue;
  struct Item *result = malloc(sizeof(struct Item));
  int option;
  int value;
  unsigned char priority;
  while (1) {

    printf("Выберете опцию\n");
    printf("1. Инициализировать очередь\n");
    printf("2. Вставить элемент\n");
    printf("3. Извлечь первый элемент из очереди\n");
    printf("4. Извлечь элемент с заданным приоритетом\n");
    printf("5. Ивзлечь элеиент с приоритетом не меншьше чем заданный\n");
    printf("6. Вывести структуру очереди\n");
    printf("7. Показать последний извлечённый элемент\n");
    printf("0. Выход\n");
    printf("Выберу: ");
    scanf("%d", &option);

    switch (option) {

    case 1: {
      initQueue(&queue);
      break;
    }
    case 2: {
      printf("Значение: ");
      scanf("%d", &value);
      printf("Приоритет: ");
      scanf("%hhu", &priority);
      insertItem(queue, value, priority);
      break;
    }
    case 3: {
      popFirstItem(queue, result);
      break;
    }
    case 4: {
      printf("Приоритет: ");
      scanf("%hhu", &priority);
      popItemWithPriority(queue, priority, result);
      break;
    }
    case 5: {
      printf("Приоритет: ");
      scanf("%hhu", &priority);
      popItemWithPriorityNotLower(queue, priority, result);
      break;
    }
    case 6: {
      printQueue(queue);
      break;
    }
    case 7: {
      printf("\nИзвлечённый элемент\npriority: %d value: %d", result->priority,
             result->value);
      break;
    }
    case 0: {
      clearQueue(queue);
      free(queue);
      free(result);
      return 1;
    }
    default:
      printf("Не определённая операция\n");
      clearQueue(queue);
      free(queue);
      free(result);
      return 0;
    }
  }
}
