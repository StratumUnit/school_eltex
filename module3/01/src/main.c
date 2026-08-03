#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum
{
  PIPE,
  MKFIFO
} pipe_type;

struct fileMeta
{
  char file_name[256];
  long file_size;
};

int main(int argc, char *argv[])
{

  pid_t pid;
  int rv;

  int file_d;
  mode_t mode;
  int flags = O_RDONLY;
  pipe_type type = PIPE;

  int pipe_data[2];
  int pipe_sync[2];

  int read_fd1 = -1;
  int write_fd1 = -1;
  int read_fd2 = -1;
  int write_fd2 = -1;
  char pipe_name[50];

  char buffer[] = "test str";
  char buf[100];

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-p") == 0)
    {
      if (i + 1 < argc)
      {
        strcpy(pipe_name, argv[i + 1]);
        pipe_name[sizeof(pipe_name) - 1] = '\0';
        type = MKFIFO;
        i++;
        break;
      }
      else
      {
        fprintf(stderr, "Ошибка, после флага -p должно идти имя ");
        exit(EXIT_FAILURE);
      }
    }
  }

  if (type == MKFIFO)
  {
    char buf_in[50];
    char buf_out[50];
    printf("Именованный канал\n");

    if (pipe_name[0] == '\0')
    {
      strcpy(pipe_name, "fifo0001");
    }

    snprintf(buf_in, 50, "%s_in", pipe_name);
    snprintf(buf_out, 50, "%s_out", pipe_name);

    if (mkfifo(buf_in, 0644) == -1)
    {
      if (errno != EEXIST)
      {
        perror("Невозможно создать fifo in");
        exit(EXIT_FAILURE);
      }
    }

    int fd_in = open(buf_in, O_RDWR);
    if (fd_in == -1)
    {
      perror("Невозможно открыть fifo in");
      exit(EXIT_FAILURE);
    }

    if (mkfifo(buf_out, 0644) == -1)
    {
      if (errno != EEXIST)
      {
        perror("Невозможно создать fifo out");
        exit(EXIT_FAILURE);
      }
    }

    int fd_out = open(buf_out, O_RDWR);
    if (fd_out == -1)
    {
      perror("Невозможно открыть fifo out");
      exit(EXIT_FAILURE);
    }

    write_fd1 = fd_in;
    read_fd1 = fd_in;

    write_fd2 = fd_out;
    read_fd2 = fd_out;
  }
  else
  {
    printf("Неименованный канал\n");

    if (pipe(pipe_data) == -1)
    {
      fprintf(stderr, "Невозможно создать pipe #1");
    }

    write_fd1 = pipe_data[1];
    read_fd1 = pipe_data[0];

    if (pipe(pipe_sync) == -1)
    {
      fprintf(stderr, "Невозможно создать pipe #2");
    }
    write_fd2 = pipe_sync[1];
    read_fd2 = pipe_sync[0];
  }

  switch (pid = fork())
  {

  case -1:
    perror("Ошибка fork");
    exit(EXIT_FAILURE);

  case 0:
    struct fileMeta metadata;
    char copy_name[50];
    char chunk_buf[512];
    while (1)
    {
      // printf("Успех некий\n");
      char ready = 'R';
      int res = write(write_fd2, &ready, 1);
      if (res == -1)
      {
        perror("Ошибка. Сигнал готовности не был отправлен");
        exit(EXIT_FAILURE);
      }

      res = read(read_fd1, &metadata, sizeof(struct fileMeta));
      if (res == -1)
      {
        perror("Ошибка чтения метаданных");
        exit(EXIT_FAILURE);
      }
      if (metadata.file_size == -1)
      {
        exit(EXIT_SUCCESS);
      }
      snprintf(copy_name, sizeof(copy_name), "%s.copy", metadata.file_name);
      int fd = open(copy_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd == -1)
      {
        perror("Ошибка создания файла");
        exit(EXIT_FAILURE);
      }
      int bytes_left = metadata.file_size;
      while (bytes_left > 0)
      {
        int bytes_to_read;
        if (bytes_to_read > 512)
        {
          bytes_to_read = 512;
        }
        else
        {
          bytes_to_read = bytes_left;
        }

        int readed = read(read_fd1, chunk_buf, bytes_to_read);
        if (readed <= 0)
        {
          perror("Ошибка при чтении контента из канала или канал закрылся");
          break;
        }
        bytes_left -= bytes_to_read;
        write(fd, chunk_buf, readed);
      }
    }

  default:
  {
    struct stat st;
    struct fileMeta curFile = {'\0', 0};
    char answer[5];
    char buffer[512];
    int fd;
    for (int i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "-p") == 0)
      {
        i++;
        continue;
      }
      fd = open(argv[i], 0644);
      if (fd == -1)
      {
        perror("Ошибка. Файла с указанным именем не существует");
        exit(EXIT_FAILURE);
      }

      int res = read(read_fd2, answer, 1);
      if (res == -1)
      {
        perror("Ошибка. Сигнал готовности не был принят");
        exit(EXIT_FAILURE);
      }
      stat(argv[i], &st);
      strncpy(curFile.file_name, argv[i], sizeof(curFile.file_name) - 1);
      curFile.file_name[sizeof(curFile.file_name) - 1] = '\0';
      curFile.file_size = st.st_size;
      write(write_fd1, &curFile, sizeof(curFile));

      int bytes_left = curFile.file_size;
      int bytes_to_write;
      while (bytes_to_write = read(fd, buffer, 512))
      {
        if (bytes_to_write == -1)
        {
          perror("Ошибка чтения файла");
          exit(EXIT_FAILURE);
        }
        write(write_fd1, buffer, bytes_to_write);
      }
    }
  }
  }

  return 0;
}
