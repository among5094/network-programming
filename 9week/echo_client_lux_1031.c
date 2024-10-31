#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>  // Windows용 소켓 라이브러리

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    WSADATA wsaData;  // WinSock 초기화 정보를 저장할 구조체
    SOCKET sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // WinSock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        error_handling("WSAStartup() error!");
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
        error_handling("connect() error!");
    else
        puts("Connected...........");

    while(1) 
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        send(sock, message, strlen(message), 0);  // send()로 데이터 전송
        str_len = recv(sock, message, BUF_SIZE - 1, 0);  // recv()로 데이터 수신
        if (str_len == -1)
            error_handling("recv() error!");

        message[str_len] = 0;
        printf("Message from server: %s", message);
    }

    closesocket(sock);  // 소켓 닫기
    WSACleanup();       // WinSock 종료
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
