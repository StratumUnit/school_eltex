#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

#define SHARED_NAME "/shm"
#define TOTAL_SHM_SIZE (4 * 1024)
#define SEM_NAME "/sem"

struct Block
{
    int next_addr;
    int count;
    int capacity;
    double data[];
};