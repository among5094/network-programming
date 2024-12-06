#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

// 12월 05일 14주차 
// 실습05) 멀티쓰레드 기반의 다중접속 클라이언트 구현 on Linux
// 

#define BUF_SIZE 100
#define NAME_SIZE 20

void *send_msg(void* arg);
void *recv_msg(void* arg); // 수정: 선언부 매개변수 수정
void error_handling(char *msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;

    pthread_t snd_thread, rcv_thread;
    void *thread_return;

    if (argc != 4) {
        printf("Usage: %s <IP> <PORT> <name> \n", argv[0]);
        exit(1);
    }

    sprintf(name, "[%s]", argv[3]);
    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // 수정: htonl(argv[1]) -> inet_addr(argv[1])
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");

    printf("%d\n", sock);

    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock); // 송신 쓰레드 생성
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock); // 수신 쓰레드 생성

    pthread_join(snd_thread, &thread_return); // 송신 쓰레드 종료 대기
    pthread_join(rcv_thread, &thread_return); // 수신 쓰레드 종료 대기

    close(sock);
    return 0;
}

void* send_msg(void *arg) {
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    printf("%d\n", sock);

    while (1) {
        fgets(msg, BUF_SIZE, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        send(sock, name_msg, strlen(name_msg), 0); // 수정: sned -> send
    }
}

void* recv_msg(void *arg) { // 수정: 함수 매개변수와 정의 일치
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    int str_len;

    while (1) {
        str_len = recv(sock, &name_msg[0], NAME_SIZE + BUF_SIZE - 1, 0);
        if (str_len == -1)
            return (void*)-1;

        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}

void error_handling(char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
