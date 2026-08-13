#include "shared.h"
#include <errno.h>
#include "semaphor.h"

#define TOTAL_SHM_SIZE (4 * 1024)

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(void)
{

    srand(time(NULL));
    key_t key = ftok(TOKEN_PATH, PROJ_ID);
    if (key == -1)
    {
        perror("Ошибка генерации ключа");
        return -1;
    }

    int shmid = shmget(key, TOTAL_SHM_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (shmid == -1)
    {
        if (errno == EEXIST)
        {
            fprintf(stderr, "Процесс производитель уже работает, разделяемая память выделена\n");
            return -1;
        }
        perror("Ошибка монтирования разделяемой памяти");
        return -1;
    }

    int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (semid == -1)
    {
        perror("Ошибка создания семафора");
        return -1;
    }

    union semun arg;
    arg.val = 1;

    semctl(semid, 0, SETVAL, arg);

    char *shared_data = shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1)
    {
        perror("Ошибка в shmat");
    }

    char *curr_ptr = shared_data;
    char *end_of_shm = shared_data + TOTAL_SHM_SIZE;

    while (curr_ptr + sizeof(struct Block) < end_of_shm)
    {
        size_t bytes_left = end_of_shm - curr_ptr;

        int max_possible_count = (bytes_left - sizeof(struct Block)) / sizeof(double);
        if (max_possible_count < 1)
        {
            break;
        }
        int rand_count = rand() % 15 + 1;
        int count = (rand_count > max_possible_count) ? max_possible_count : rand_count;

        char *new_block_ptr = curr_ptr + sizeof(struct Block) + count * sizeof(double);

        semaphor_lock(semid);
        struct Block *block = (struct Block *)curr_ptr;
        block->count = count;
        block->next_addr = 0;
        if (new_block_ptr + sizeof(struct Block) + sizeof(double) <= end_of_shm)
        {
            block->next_addr = (int)(new_block_ptr - shared_data);
        }

        for (int i = 0; i < count; i++)
        {

            block->data[i] = rand() % 100;
        }
        semaphor_unlock(semid);
        curr_ptr = new_block_ptr;
    }
    printf("генерация завершена\n");
    while (1)
    {
        int isProcessed = 1;
        char *ptr = shared_data;
        semaphor_lock(semid);
        while (ptr != NULL)
        {
            struct Block *block = (struct Block *)ptr;

            if (block->count != 0)
            {
                isProcessed = 0;
                break;
            }
            if (block->next_addr == 0)
            {
                ptr = NULL;
            }
            else
            {
                ptr = shared_data + block->next_addr;
            }
        }
        semaphor_unlock(semid);
        if (!isProcessed)
        {
            sleep(3);
        }
        else
        {
            shmdt(shared_data);
            shmctl(shmid, IPC_RMID, NULL);
            semctl(semid, 0, IPC_RMID);
            printf("Ресурсы успешно удалены. Работа производителя завершена.\n");
            return 0;
        }
    }
}