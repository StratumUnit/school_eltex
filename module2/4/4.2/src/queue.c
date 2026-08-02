#include "queue.h"

void initQueue(struct Queue **queue) {
  *queue = malloc(sizeof(struct Queue));
  (*queue)->begin = NULL;
  (*queue)->size = 0;
}

int insertItem(struct Queue *queue, int value, int priority) {

  if (queue == NULL)
    return 0;

  if (priority < 0 || priority > 255) {
    return -1;
  }

  struct Item *newItem = malloc(sizeof(struct Item));
  newItem->value = value;
  newItem->priority = (unsigned char)priority;
  newItem->next = NULL;

  if (queue->begin == NULL) {
    queue->begin = newItem;
    queue->size++;
    return 1;
  }
  struct Item *iter = queue->begin;
  struct Item *prev = NULL;
  while (iter != NULL) {
    if (newItem->priority < iter->priority) {
      newItem->next = iter;
      if (!prev) {
        queue->begin = newItem;
      } else {
        prev->next = newItem;
      }
      queue->size++;
      return 1;
    }
    prev = iter;
    iter = iter->next;
  }

  prev->next = newItem;
  queue->size++;
  return 1;
}

int popFirstItem(struct Queue *queue, struct Item *result) {
  if (queue == NULL || queue->begin == NULL || result == NULL) {
    return 0;
  }
  struct Item *itemToDel = (queue->begin);
  queue->begin = queue->begin->next;

  result->value = itemToDel->value;
  result->priority = itemToDel->priority;

  free(itemToDel);
  queue->size--;
  return 1;
}

int popItemWithPriority(struct Queue *queue, unsigned char priority,
                        struct Item *result) {

  if (queue == NULL || queue->begin == NULL || result == NULL) {
    return 0;
  }

  struct Item *iter = queue->begin;
  struct Item *prev = NULL;

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
  return 0;
}
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

int clearQueue(struct Queue *queue) {
  if (queue == NULL || queue->begin == NULL) {
    return 0;
  }
  struct Item *iter = queue->begin;

  while (iter != NULL) {
    struct Item *itemToFree = iter;
    iter = iter->next;
    free(itemToFree);
    queue->size--;
  }
  return 1;
}

void printQueue(const struct Queue *queue) {
  if (queue == NULL) {
    return;
  }
  int i = 0;
  struct Item *iter = queue->begin;
  printf("\n\nQueue|size: %d\n", queue->size);
  while (iter != NULL) {
    printf("%d. Priority: %d Value: %d\n", i, iter->priority, iter->value);
    i++;
    iter = iter->next;
  }
}
