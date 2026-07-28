#include "queue.h"


int main(void){

    struct Queue queue1;
    struct Queue *queue = &queue1;
    //initQueue(&queue);

    int option;
    int value;
    int priority;
    while(1){

        printf("Выберете опцию\n");
        printf("1. Инициализировать очередь\n");
        printf("2. Вставить элемент\n");
        printf("3. Удалить элемент\n");
        printf("4. Извлеч элемент\n");
        printf("5. Вывести структуру очереди\n");
        printf("Выберу: ");
        scanf("%d", &option);

        switch(option){

            case 1: {
                initQueue(&queue);
                break;
            }
            case 2: {
                printf("Значение: ");
                scanf("%d", &value);
                printf("Приоритет");
                scanf("%d", &priority);
                insertItem(&queue, value, priority);
                break;
            }
            case 3: {
                //deleteItem(&queue);
                break;
            }
            case 4: {
                break;
            }
            case 5: {
                printQueue(queue);
                break;
            }
            default :
            printf("Не определённая операция\n");
            return 0;
        }
    }
    return 0;
}