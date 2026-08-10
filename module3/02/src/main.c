#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define N 256

struct pid_node
{
  pid_t pid;
  struct pid_node *next;
};

struct Topic
{
  char *name;
  struct pid_node *subs_head;
  struct Topic *next;
};

struct msgbuf
{
  long mtype;
  char mtext[N];
};

const char *FILE_PATH = "/tmp/mq_file";

volatile sig_atomic_t should_terminate = 0;
volatile sig_atomic_t pub_should_terminate = 0;
volatile sig_atomic_t sub_should_terminate = 0;
void sigint_handler(int signum)
{
  should_terminate = 1;
}

void sigint_handler_pub(int signum)
{
  pub_should_terminate = 1;
}

void sigint_handler_sub(int signum)
{
  sub_should_terminate = 1;
}

int add_pid(struct pid_node **root, pid_t pid)
{

  struct pid_node *cur = *root;
  while (cur != NULL)
  {
    if (cur->pid == pid)
    {
      return -1;
    }
    cur = cur->next;
  }
  struct pid_node *new_node = malloc(sizeof(struct pid_node));
  if (new_node == NULL)
  {
    perror("malloc failed in add_pid");
    return -1;
  }
  new_node->pid = pid;
  new_node->next = (*root);
  *root = new_node;
  return 0;
}

int del_pid(struct pid_node **root, pid_t pid)
{

  struct pid_node *cur = *root;
  if (cur->pid == pid)
  {
    struct pid_node *node_to_del = cur;
    *root = node_to_del->next;
    free(node_to_del);
    return 0;
  }
  while (cur->next != NULL)
  {

    if (cur->next->pid == pid)
    {
      struct pid_node *node_to_del = cur->next;

      cur->next = node_to_del->next;

      free(node_to_del);
      return 0;
    }
    cur = cur->next;
  }
  return -1;
}

int add_or_subscribe_topic(struct Topic **head, const char *name, pid_t pid)
{
  struct Topic *cur = *head;
  while (cur != NULL)
  {
    if (strcmp(cur->name, name) == 0)
    {
      add_pid(&(cur->subs_head), pid);
      return 0;
    }
    cur = cur->next;
  }

  struct Topic *new_topic = malloc(sizeof(struct Topic));
  if (new_topic == NULL)
  {
    perror("malloc failed in add_topic");
    return -1;
  }
  new_topic->name = strdup(name);
  if (new_topic->name == NULL)
  {
    perror("strdup failed");
    free(new_topic);
    return -1;
  }
  new_topic->subs_head = NULL;
  add_pid(&(new_topic->subs_head), pid);
  new_topic->next = *head;
  *head = new_topic;
  return 0;
}

int unsubscribe_topic(struct Topic **head, const char *name, pid_t pid)
{
  struct Topic *cur = *head;
  while (cur != NULL)
  {
    if (strcmp(cur->name, name) == 0)
    {

      del_pid(&(cur->subs_head), pid);
      return 0;
    }
    cur = cur->next;
  }
  return -1;
}

int parse_broker_message(const char *mtext,
                         char *out_cmd,
                         pid_t *out_pid,
                         char *out_topic,
                         const char **out_payload)
{

  if (sscanf(mtext, "%[^,],%d,%[^\n]", out_cmd, out_pid, out_topic) < 3)
  {
    return -1;
  }

  const char *ptr = strchr(mtext, '\n');

  if (ptr == NULL)
  {
    *out_payload = "";
  }
  else
  {
    *out_payload = ptr + 1;
  }

  return 0;
}

int send_to_subscribers(struct Topic *head, int msqid, const char *topic_name, const char *payload)
{
  struct Topic *cur = head;
  struct msgbuf buf = {0};
  while (cur != NULL)
  {
    if (strcmp(cur->name, topic_name) == 0)
    {
      struct pid_node *node = cur->subs_head;

      strncpy(buf.mtext, payload, sizeof(buf.mtext) - 1);
      buf.mtext[sizeof(buf.mtext) - 1] = '\0';
      while (node != NULL)
      {
        buf.mtype = node->pid;
        int res = msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);
        if (res == -1)
        {
          perror("Ошибка отправки сообщения подписчику");
        }
        node = node->next;
      }
    }
    cur = cur->next;
  }
  return -1;
}

void safe_broker_shutdown(struct Topic **topics_head, struct pid_node **publishers_head, int msqid)
{
  struct pid_node *pub_cur = *publishers_head;
  while (pub_cur != NULL)
  {
    kill(pub_cur->pid, SIGINT);
    struct pid_node *temp = pub_cur;
    pub_cur = pub_cur->next;
    free(temp);
  }
  *publishers_head = NULL;

  struct Topic *topic_cur = *topics_head;
  while (topic_cur != NULL)
  {
    struct pid_node *sub_cur = topic_cur->subs_head;
    while (sub_cur != NULL)
    {
      kill(sub_cur->pid, SIGINT);
      struct pid_node *temp_sub = sub_cur;
      sub_cur = sub_cur->next;
      free(temp_sub);
    }
    struct Topic *temp_topic = topic_cur;
    if (temp_topic->name)
      free(temp_topic->name);
    topic_cur = topic_cur->next;
    free(temp_topic);
  }
  *topics_head = NULL;

  struct msqid_ds dummy;
  if (msgctl(msqid, IPC_STAT, &dummy) == 0)
  {
    msgctl(msqid, IPC_RMID, NULL);
    printf("Очередь сообщений удалена.\n");
  }

  unlink("/tmp/broker_shared_key");
  printf("Работа завершена успешно.\n");
}

int broker_func()
{

  struct pid_node *publisher = NULL;
  struct Topic *topic = NULL;

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = sigint_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGINT, &sa, NULL) == -1)
  {
    perror("sigaction failed");
    return -1;
  }

  int fd = open(FILE_PATH, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd == -1)
  {
    perror("Ошибка создания файла");
    return 1;
  }
  close(fd);

  key_t key = ftok(FILE_PATH, 'a');
  if (key == -1)
  {
    perror("Ошибка:");
    return 1;
  }

  int msqid;
  msqid = msgget(key, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  if (msqid == -1)
  {
    if (errno == EEXIST)
    {
      printf("Брокер уже запущен, выход");
      return 0;
    }
  }
  long msg_type_filter = 1;
  struct msgbuf msg;
  while (!should_terminate)
  {
    ssize_t bytes = msgrcv(msqid, &msg, sizeof(msg.mtext), msg_type_filter, 0);
    if (bytes == -1)
    {

      if (errno == EINTR || errno == EIDRM)
      {
        break;
      }
      perror("Ошибка msgrcv");
      break;
    }

    char cmd[32];
    pid_t sender_pid;
    char topic_name[128];
    const char *payload = NULL;

    if (parse_broker_message(msg.mtext, cmd, &sender_pid, topic_name, &payload) == 0)
    {

      if (strcmp(cmd, "subscribe") == 0)
      {
        add_or_subscribe_topic(&topic, topic_name, sender_pid);
        printf("Процесс %d подписался на тему %s\n", sender_pid, topic_name);
      }

      else if (strcmp(cmd, "unsubscribe") == 0)
      {
        unsubscribe_topic(&topic, topic_name, sender_pid);
        printf("Процесс %d отписался от темы %s\n", sender_pid, topic_name);
      }

      else if (strcmp(cmd, "send") == 0)
      {

        add_pid(&publisher, sender_pid);

        printf("Получен payload от издателя %d в тему %s: %s\n",
               sender_pid, topic_name, payload);

        send_to_subscribers(topic, msqid, topic_name, payload);
      }
    }
  }
  safe_broker_shutdown(&topic, &publisher, msqid);
  return 0;
}

void send_msg(int msqid, struct msgbuf *msgp, size_t msgsz, int msgflg)
{
  int res = msgsnd(msqid, msgp, msgsz, msgflg);
  if (res == -1)
  {
    perror("");
  }
}

// Издатель
int publisher_func(const char *topic_name)
{

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = sigint_handler_pub;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGINT, &sa, NULL) == -1)
  {
    perror("sigaction failed");
    return -1;
  }

  key_t key = ftok(FILE_PATH, 'a');
  if (key == -1)
  {
    perror("Ошибка:");
    return 1;
  }

  int msqid;
  msqid = msgget(key, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  if (msqid == -1)
  {
    perror("Ошибка подключения к очереди");
  }

  pid_t pid = getpid();
  struct msgbuf msg;
  msg.mtype = 1;
  char input[N];

  while (!pub_should_terminate)
  {
    if (fgets(input, sizeof(input), stdin) == NULL)
    {
      break;
    }
    input[strcspn(input, "\n")] = '\0';

    if (strlen(input) == 0)
    {
      continue;
    }

    snprintf(msg.mtext, sizeof(msg.mtext), "send,%d,%s\n%s", pid, topic_name, input);

    if (msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0) == -1)
    {
      perror("Ошибка отправки сообщения издателем");
      break;
    }
  }
  printf("Завершение работы издателя");
  return 0;
}

// Подписчик

int subscriber_func(int argc, char *argv[])
{

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = sigint_handler_sub;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGINT, &sa, NULL) == -1)
  {
    perror("sigaction failed");
    return -1;
  }

  key_t key = ftok(FILE_PATH, 'a');
  if (key == -1)
  {
    perror("Ошибка:");
    return 1;
  }

  int msqid;
  msqid = msgget(key, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  if (msqid == -1)
  {
    perror("Ошибка подключения к очереди");
  }

  pid_t pid = getpid();
  struct msgbuf msg;

  // Цикл подписки на темы
  for (int i = 2; i < argc; i++)
  {
    snprintf(msg.mtext, sizeof(msg.mtext), "subscribe,%d,%s", pid, argv[i]);
    msg.mtype = 1;
    if (msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0) == -1)
    {
      perror("Ошибка подписки");
      return -1;
    }
  }
  struct msgbuf buf;
  buf.mtype = pid;
  while (!sub_should_terminate)
  {
    ssize_t bytes = msgrcv(msqid, &buf, sizeof(buf.mtext), pid, 0);
    if (bytes == -1)
    {

      if (errno == EINTR || errno == EIDRM)
      {
        break;
      }
      perror("Ошибка msgrcv");
      break;
    }
    printf("Пришло сообщение: %s\n", buf.mtext);
  }

  for (int i = 2; i < argc; i++)
  {
    snprintf(msg.mtext, sizeof(msg.mtext), "unsubscribe,%d,%s", pid, argv[i]);
    msg.mtype = 1;
    if (msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0) == -1)
    {
      perror("Ошибка отписки");
      return -1;
    }
    printf("Сообщение отписки отправлено\n");
  }
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Передайте аргумент -b, -p или -s\n");
    return -1;
  }
  if (strcmp(argv[1], "-b") == 0)
  {
    broker_func();
  }
  else if (strcmp(argv[1], "-p") == 0)
  {
    if (argc == 3)
    {
      publisher_func(argv[2]);
    }
    else
    {
      printf("Передайте в качестве аргумента тему сообщения\n");
    }
  }
  else if (strcmp(argv[1], "-s") == 0)
  {
    subscriber_func(argc, argv);
  }
  return 0;
}