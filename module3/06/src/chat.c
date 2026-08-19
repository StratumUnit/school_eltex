#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define MAX_TEXT_LEN 512
#define NAME_LEN 32

char name[NAME_LEN];
volatile sig_atomic_t keep_running = 1;

struct msg
{
    int type;
    char nickname[NAME_LEN];
    char text[MAX_TEXT_LEN];
};

void handle_sigint(int sig)
{
    keep_running = 0;
}

void *recvMsg(void *arg)
{
    int *sock_fd = (int *)arg;
    struct sockaddr_in recvd_addr;
    socklen_t addr_len;
    while (1)
    {
        struct msg received_msg;
        addr_len = sizeof(recvd_addr);
        recvfrom(*sock_fd, &received_msg, sizeof(received_msg), 0, (struct sockaddr *)&recvd_addr, &addr_len);
        char *sender_ip = inet_ntoa(recvd_addr.sin_addr);

        if (strcmp(name, received_msg.nickname) == 0)
        {
            continue;
        }
        switch (received_msg.type)
        {
        case 0:
            printf("Новый пользователь чата %s [%s]\n", received_msg.nickname, sender_ip);
            break;
        case 1:
            printf("%s [%s]: %s\n", received_msg.nickname, sender_ip, received_msg.text);
            break;
        case 2:
            printf("Пользователь %s [%s] отключился от чата\n", received_msg.nickname, sender_ip);
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int port;
    if (argc < 2)
    {
        perror("Передайте номер порта при запуске");
        return -1;
    }
    port = atoi(argv[1]);

    printf("Введите имя пользователя: ");
    fgets(name, NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1)
    {
        perror("Ошибка создания udp сокета");
        return -1;
    }
    int bind_res = bind(udp_socket, (struct sockaddr *)&local_addr, sizeof(local_addr));
    if (bind_res < 0)
    {
        perror("Ошибка bind");
    }

    struct sockaddr_in broadcast_addr;
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(port);
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    int broadcast_permission = 1;
    if (setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &broadcast_permission, sizeof(broadcast_permission)) < 0)
    {
        perror("Ошибка setsockopt");
        close(udp_socket);
        exit(EXIT_FAILURE);
    }

    struct msg initMsg;

    memset(&initMsg, 0, sizeof(initMsg));
    initMsg.type = 0;
    strncpy(initMsg.nickname, name, NAME_LEN - 1);

    ssize_t bytes_sent = sendto(udp_socket, &initMsg, sizeof(initMsg), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));
    if (bytes_sent == -1)
    {
        perror("Ошибка отправки init");
    }

    pthread_t tid;
    pthread_create(&tid, NULL, recvMsg, &udp_socket);
    while (keep_running)
    {
        struct msg textMsg;
        textMsg.type = 1;
        strncpy(textMsg.nickname, name, NAME_LEN - 1);
        if (fgets(textMsg.text, sizeof(textMsg.text), stdin) == NULL)
        {
            break;
        }
        textMsg.text[strcspn(textMsg.text, "\n")] = '\0';

        ssize_t bytes_sent = sendto(udp_socket, &textMsg, sizeof(textMsg), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));
        if (bytes_sent == -1)
        {
            perror("Ошибка отправки");
        }
    }

    struct msg exitMsg;
    exitMsg.type = 2;
    strncpy(exitMsg.nickname, name, NAME_LEN - 1);
    bytes_sent = sendto(udp_socket, &exitMsg, sizeof(exitMsg), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));
    if (bytes_sent == -1)
    {
        perror("Ошибка отправки exit");
    }
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    close(udp_socket);
}