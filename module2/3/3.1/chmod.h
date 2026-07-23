#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int parsePermissionBits(char *permissions, int *bits);
int getFilePermissions(char *filepath, int *bits);
int changePermissions(char *cmd, int *bits);
void printPermissions(int bits);
int maskToStr(int mask);
