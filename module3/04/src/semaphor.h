#include "shared.h"
int semaphor_lock(int semid)
{

    struct sembuf lock = {0, -1, SEM_UNDO};
    if (semop(semid, &lock, 1) == -1)
    {
        if (errno != EINVAL && errno != EIDRM)
        {
            perror("Ошибка блокировки семафора");
        }
        return -1;
    }
    return 0;
}

int semaphor_unlock(int semid)
{

    struct sembuf unlock[2] = {{0, 0, SEM_UNDO}, {0, 1, SEM_UNDO}};
    if (semop(semid, unlock, 2) == -1)
    {
        if (errno != EINVAL && errno != EIDRM)
        {
            perror("Ошибка разблокировки семафора");
        }
        return -1;
    }
    return 0;
}