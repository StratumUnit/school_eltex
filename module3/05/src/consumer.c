#include "shared.h"

int main(void)
{
    int shm_fd = shm_open(SHARED_NAME, O_RDWR, 0666);

    char *shared_data = (char *)mmap(NULL, TOTAL_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shared_data == MAP_FAILED)
    {
        perror("Ошибка");
        exit(1);
    }
    sem_t *sem = sem_open(SEM_NAME, 0);
    int offset = 0;
    int processed_count = 0;
    while (1)
    {

        if (sem_wait(sem) == -1)
        {
            if (errno == EINVAL)
            {
                printf("Семафор был удален. Завершаю работу.\n");
                munmap(shared_data, TOTAL_SHM_SIZE);
                return 0;
            }
        }
        struct Block *block = (struct Block *)(shared_data + offset);

        int count = block->count;
        int next_offset = block->next_addr;
        if (count > 0)
        {
            block->count = 0;
            processed_count++;
            double *tmp_data = malloc(sizeof(double) * count);
            memcpy(tmp_data, block->data, sizeof(double) * count);
            sem_post(sem);

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
                sem_post(sem);
                munmap(shared_data, TOTAL_SHM_SIZE);
                printf("Потребитель [%d]: Память пуста, работа завершена. Выхожу.\n", getpid());
                return 0;
            }

            sem_post(sem);
            sleep(1);
            offset = 0;
            processed_count = 0;
            continue;
        }
        offset = next_offset;
        sem_post(sem);
    }
}