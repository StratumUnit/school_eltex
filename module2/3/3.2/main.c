#include "net_utils.h"

int main(int argc, char **argv) {
  if (argc < 4) {
    printf("Ошибка: Передайте в качестве аргументов шлюз, адрес сети и число "
           "испытний!");
    return 1;
  }

  char *gateway = argv[1];
  char *network = argv[2];
  char *endptr;

  long val = strtol(argv[3], &endptr, 10);

  if (argv[3] == endptr) {
    printf("Ошибка: аргумент '%s' не является числом!\n", argv[3]);
    return 1;
  }

  int N = (int)val;
  int count;
  for (int i = 0; i < N; i++) {
    unsigned int ip = generateRandomInt();
    if (isIpInNetwork(gateway, ip, network)) {
      count++;
    }
  }
  printf("\n%f %% всех пакетов относятся к локальной сети\n",
         (double)count / N * 100);

  /*
  unsigned int testNum = ipToInt("255.255.255.255");
  printf("%u\n", testNum);
  char buf[16];
  intToIp(testNum, buf);
  printf("%s", buf);
  */
}
