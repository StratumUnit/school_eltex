#include <stdlib.h>
#include <stdio.h>


struct Item {

    int value;
    int priority;
    struct Item *next;
};

struct Queue {
    struct Item* begin;
    int size;
};

int initQueue(struct Queue** queue);


int insertItem(struct Queue** queue, int value, int priority);
void printQueue(const struct Queue *queue);