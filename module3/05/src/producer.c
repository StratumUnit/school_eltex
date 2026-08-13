#include "shared.h"
#include <errno.h>

int main(void)
{

    srand(time(NULL));

    int shm_fd = shm_open(SHARED_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open error");
        return -1;
    }

    if (ftruncate(shm_fd, TOTAL_SHM_SIZE) == -1)
    {
        perror("ftruncate error");
        return -1;
    }

    char *shared_data = (char *)mmap(NULL, TOTAL_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("Ошибка");
        return -1;
    }

    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED)
    {
        perror("sem_open error");
        return -1;
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

        sem_wait(sem);
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
        sem_post(sem);
        curr_ptr = new_block_ptr;
    }
    printf("генерация завершена\n");
    while (1)
    {
        int isProcessed = 1;
        char *ptr = shared_data;
        sem_wait(sem);
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
        sem_post(sem);
        if (!isProcessed)
        {
            sleep(3);
        }
        else
        {

            munmap(shared_data, TOTAL_SHM_SIZE);
            shm_unlink(SHARED_NAME);
            sem_close(sem);
            sem_unlink(SEM_NAME);
            printf("Ресурсы успешно удалены. Работа производителя завершена.\n");
            return 0;
        }
    }
}