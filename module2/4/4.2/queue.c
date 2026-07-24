#include "queue.h"

int initQueue(struct Queue** queue){
    (*queue)->begin = NULL;

}

int insertItem(struct Queue** queue, int value, int priority){

    struct Item* newItem = malloc(sizeof(struct Item));
    newItem->value = value;
    newItem->priority = priority;
    newItem->next = NULL;

    if((*queue)->begin == NULL){

        (*queue)->begin = newItem;

        return 1;
    }
    struct Item* iter = (*queue)->begin;

    while(iter->next->priority <= newItem->priority){
        iter = iter->next;
    }

    newItem->next = iter->next;
    iter->next = newItem;
    return 1;

}

void printQueue(const struct Queue* queue){
    if(queue == NULL){
        return;
    }
    struct Item* iter = queue->begin;
    while(iter != NULL){
        printf("Priority: %d  Value: %d\n", iter->priority, iter->value);
        iter = iter->next;
    }
}