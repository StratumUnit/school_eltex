#include "net_utils.h"
#include <string.h>

unsigned int generateRandomInt() {

  return ((unsigned int)((rand() & 0xFFFF) << 16)) |
         (unsigned int)(rand() & 0xFFFF);
}

unsigned int ipToInt(const char *ip) {

  unsigned int result = 0;
  char *str = strdup(ip);
  char *tok = strtok(str, ".");
  int i = 3;
  while (tok != NULL && i >= 0) {
    result |= (unsigned int)strtol(tok, NULL, 10) << (8 * i);
    tok = strtok(NULL, ".");
    i--;
  }
  free(str);
  return result;
}
unsigned int prefixToNetworkNum(const char *prefix) {
  int number;
  if (sscanf(prefix, "/%d", &number) == 1) {
    return (unsigned int)0xFFFFFFFF << (32 - number);
  }
  printf("Ошибка чтения префикса");
  return 0;
}

int intToIp(unsigned int num, char *buf) {

  snprintf(buf, 16, "%u.%u.%u.%u", num >> 24, (num >> 16) & 0xFF,
           (num >> 8) & 0xFF, num & 0xFF);
  return 1;
}

int isIpInNetwork(char *gateway, unsigned int ipNum, char *network) {
  unsigned int gatewayNum = ipToInt(gateway);
  unsigned int networkNum = 0;
  char buf1[16];
  char buf2[16];
  char buf3[16];

  if (*network == '/') {
    networkNum = prefixToNetworkNum(network);
  } else {
    networkNum = ipToInt(network);
  }
  intToIp(ipNum, buf1);
  intToIp(gatewayNum, buf2);
  intToIp(networkNum, buf3);

  // printf("Ip: %s \n Шлюз: %s \n Маска: %s\n", buf1, buf2, buf3);

  unsigned int a = gatewayNum & networkNum;
  unsigned int b = ipNum & networkNum;
  if ((a ^ b) == (unsigned int)0) {
    printf("Ip адрес %s из этой подсети\n", buf1);
    return 1;
  } else {
    printf("Ip адрес %s из другой подсети\n", buf1);
    return 0;
  }
}
