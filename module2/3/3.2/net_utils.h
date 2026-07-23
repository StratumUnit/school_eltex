#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

unsigned int generateRandomInt();
unsigned int ipToInt(const char *ip);
int intToIp(unsigned int num, char *buf);
int isIpInNetwork(char *gateway, unsigned int ipNum, char *network);
