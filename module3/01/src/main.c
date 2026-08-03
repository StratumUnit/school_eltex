#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

typedef enum { PIPE, MKFIFO } pipe_type;

int main(int argc, char *argv[]) {

  pid_t pid;
  int rv;

  int file_d;
  mode_t mode;
  int flags = O_RDONLY;
  pipe_type type = PIPE;
  int offset = 0;

  int read_fd = -1;
  int write_fd = -1;
char pipe_name[20];

char buffer[] = "test str";
char buf[100];

  for (int i = 1; i < argc; i++) {

    if (strcmp(argv[i], "-p") == 0) {
        strcpy(pipe_name, argv[i+1]);
      type = MKFIFO;
      offset = 2;
      i++;
      continue;
    }
    file_d = open(argv[i], flags);
    if (file_d == -1) {
      perror("Файл с указанным именем не существует");
      continue;
    }
    printf("Файл %d: %s\n", i - offset, argv[i]);
  }

  if (type == MKFIFO) {
    printf("Именованный канал\n");
    const char *fifo_path = "/tmp/fifo0001";
    if (mkfifo(fifo_path, 0644) == -1) {
      if(errno != EEXIST){
      perror("Невозможно создать fifo");
      exit(EXIT_FAILURE);
      }
    }

    if ((write_fd = open(fifo_path, O_RDWR)) == -1) {
      perror("Невозможно открыть fifo");
      exit(EXIT_FAILURE);
    }

  } else {
    printf("Неименованный канал\n");
    int pipefd[2];
    if (pipe(pipefd) == -1) {
      fprintf(stderr, "Невозможно создать pipe");
    }
    write_fd = pipefd[1];
    read_fd = pipefd[0];
  }

  switch (pid = fork()) {

  case -1:
    perror("Ошибка fork");
    exit(EXIT_FAILURE);

  case 0:
    int bytes_read;
    if((bytes_read = read(write_fd, &buf, sizeof(buf))) == -1){
        perror("Невозможно прочитать буфер");
    }
    else{
        //printf("\n%d\n ", bytes_read);
        buf[bytes_read] = '\0';
        printf("Прочитано из fifo: %s\n", buf);
    }

  default: {
   
    write(write_fd, buffer, strlen(buffer));
    }
  }

  return 0;
}