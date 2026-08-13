#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define TOKEN_PATH "/tmp"
#define PROJ_ID 'a'

struct Block
{
    int next_addr;
    int count;
    int capacity;
    double data[];
};