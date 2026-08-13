#include "shared.h"
#include "semaphor.h"

int main(void)
{
    key_t key = ftok(TOKEN_PATH, PROJ_ID);

    if (key == -1)
    {
        perror("Ошибка генерации ключа");
        return -1;
    }

    int shmid = shmget(key, 0, 0);
    if (shmid == -1)
    {
        perror("Ошибка монтирования разделяемой памяти");
        return -1;
    }

    int semid = semget(key, 0, 0);
    if (semid == -1)
    {
        perror("Ошибка создания семафора");
    }

    char *shared_data = (char *)shmat(shmid, NULL, 0);

    if (shared_data == (void *)-1)
    {
        perror("Ошибка shmat");
        exit(1);
    }

    int offset = 0;
    int processed_count = 0;
    while (1)
    {

        semaphor_lock(semid);
        struct Block *block = (struct Block *)(shared_data + offset);

        int count = block->count;
        int next_offset = block->next_addr;
        if (count > 0)
        {
            block->count = 0;
            processed_count++;
            double *tmp_data = malloc(sizeof(double) * count);
            memcpy(tmp_data, block->data, sizeof(double) * count);
            semaphor_unlock(semid);

            offset = next_offset;
            double max_elem;
            double min_elem;
            for (int i = 0; i < count; i++)
            {
                if (i == 0)
                {
                    max_elem = tmp_data[i];
                    min_elem = max_elem;
                }
                if (tmp_data[i] > max_elem)
                {
                    max_elem = tmp_data[i];
                }
                if (tmp_data[i] < min_elem)
                {
                    min_elem = tmp_data[i];
                }
            }
            free(tmp_data);
            printf("Min: %0.3f, Max: %0.3f\n", min_elem, max_elem);
            sleep(1);
            continue;
        }
        if (next_offset == 0)
        {
            if (processed_count == 0)
            {
                semaphor_unlock(semid);
                shmdt(shared_data);
                printf("Потребитель [%d]: Память пуста, работа завершена. Выхожу.\n", getpid());
                return 0;
            }

            semaphor_unlock(semid);
            sleep(1);
            offset = 0;
            processed_count = 0;
            continue;
        }
        offset = next_offset;
        semaphor_unlock(semid);
    }
}