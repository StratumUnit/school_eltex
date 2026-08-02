#include <stdio.h>
#include <stdlib.h>

struct Item {

  int value;
  unsigned char priority;
  struct Item *next;
};

struct Queue {
  struct Item *begin;
  int size;
};

void initQueue(struct Queue **queue);

int insertItem(struct Queue *queue, int value, int priority);

int popFirstItem(struct Queue *queue, struct Item *result);
int popItemWithPriority(struct Queue *queue, unsigned char priority,
                        struct Item *result);
int popItemWithPriorityNotLower(struct Queue *queue, unsigned char priority,
                                struct Item *result);
int clearQueue(struct Queue *queue);
void printQueue(const struct Queue *queue);
