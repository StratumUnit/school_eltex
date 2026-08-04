#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 8192
#define PRIORITY 1
#define EXIT_PRIORITY 32
volatile sig_atomic_t keep_running = 1;

mqd_t queue_d1;
mqd_t queue_d2;

typedef enum
{
    CREATOR,
    CLIENT
} proccess_type;

void handle_sigint(int sig)
{
    keep_running = 0;
}

void *recive_msg(void *type)
{
    char buf[BUF_SIZE];
    unsigned msg_prio;
    mqd_t current = (*(proccess_type *)type == CREATOR) ? queue_d1 : queue_d2;
    while (keep_running)
    {
        int bytes = mq_receive(current, buf, BUF_SIZE, &msg_prio);
        if (bytes == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            if (!keep_running)
            {
                break;
            }
            perror("Ошибка получения сообщения");
            pthread_exit(0);
        }
        buf[bytes] = '\0';

        if (msg_prio == EXIT_PRIORITY)
        {
            printf("\nСобеседник завершил работу\n");
            kill(getpid(), SIGINT);
            keep_running = 0;
            break;
        }
        printf("\nСобеседник: %s", buf);
        printf(">");
        fflush(stdout);
    }
    pthread_exit(0);
}

int creator_func()
{
    char buf[BUF_SIZE];
    pthread_t tid;
    static proccess_type role_arg = CREATOR;
    pthread_create(&tid, NULL, recive_msg, (void *)&role_arg);

    while (keep_running)
    {
        printf(">");
        if (fgets(buf, BUF_SIZE, stdin) == NULL)
            break;
        if (mq_send(queue_d2, buf, strlen(buf) + 1, PRIORITY) == -1)
        {
            perror("Ошибка отправки сообщения");
            return -1;
        }
    }
    printf("Создатель тут");
    fflush(stdout);
    mq_send(queue_d2, "END", 4, EXIT_PRIORITY);
    mq_close(queue_d1);
    mq_close(queue_d2);
    pthread_join(tid, NULL);
    return 0;
}

int client_func()
{

    char buf[BUF_SIZE];
    pthread_t tid;
    static proccess_type role_arg = CLIENT;
    pthread_create(&tid, NULL, recive_msg, (void *)&role_arg);

    while (keep_running)
    {
        printf(">");
        if (fgets(buf, BUF_SIZE, stdin) == NULL)
            break;
        // buf[strcspn(buf, "\n")] = '\0';
        if (mq_send(queue_d1, buf, strlen(buf) + 1, PRIORITY) == -1)
        {
            perror("Ошибка отправки сообщения");
            return -1;
        }
    }
    mq_send(queue_d1, "EOF", 4, EXIT_PRIORITY);
    mq_close(queue_d1);
    mq_close(queue_d2);
    pthread_join(tid, NULL);
    return 0;
}

int main(int argc, char *argv[])
{

    proccess_type type;
    char queue_name[256];
    char name1[260];
    char name2[260];
    struct mq_attr attr1;
    struct mq_attr attr2;
    if (argc == 1)
    {
        fprintf(stderr, "Не указано имя очереди\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    strncpy(queue_name, argv[1], 256);
    snprintf(name1, 260, "%s_1", queue_name);
    snprintf(name2, 260, "%s_2", queue_name);

    queue_d1 = mq_open(name1, O_RDWR | O_CREAT | O_EXCL, 0644, NULL);

    if (queue_d1 == (mqd_t)-1)
    {
        if (errno != EEXIST)
        {
            perror("Ошибка создания очереди");
            exit(EXIT_FAILURE);
        }
    }

    queue_d2 = mq_open(name2, O_RDWR | O_CREAT | O_EXCL, 0644, NULL);

    if (queue_d2 == (mqd_t)-1)
    {
        if (errno != EEXIST)
        {
            perror("Ошибка создания очереди");
            exit(EXIT_FAILURE);
        }
    }

    if (queue_d1 != (mqd_t)-1 && queue_d2 != (mqd_t)-1) // Очереди создались, значит это процесс СОЗДАТЕЛЬ
    {
        type = CREATOR;
        creator_func();
        mq_unlink(name1);
        mq_unlink(name2);
    }
    else // Возникла ошибка EEXIST - процесс КЛИЕНТ
    {

        queue_d1 = mq_open(name1, O_RDWR, 0644, NULL);

        if (queue_d1 == (mqd_t)-1)
        {

            perror("Ошибка создания очереди");
            exit(EXIT_FAILURE);
        }

        queue_d2 = mq_open(name2, O_RDWR, 0644, NULL);

        if (queue_d2 == (mqd_t)-1)
        {

            perror("Ошибка создания очереди");
            exit(EXIT_FAILURE);
        }

        type = CLIENT;
        client_func();
    }
}