#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[]){

    WSADATA wsaData;
    SOCKET sock;
    char message[BUF_SIZE];
    int strLen;
    int adr_sz; //추가

    SOCKADDR_IN servAdr;
    SOCKADDR_IN from_adr;

    if(argc!=3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)
        ErrorHandling("WSAStartup() error!");

    // UDP 소켓을 위한 SOCK_DGRAM으로 변경
    sock=socket(PF_INET, SOCK_DGRAM, 0);
    if(sock==INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family=AF_INET;
    servAdr.sin_addr.s_addr=inet_addr(argv[1]);
    servAdr.sin_port=htons(atoi(argv[2]));

    while(1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        if(strlen(message)!=0){
            // sendto: UDP 패킷 전송
            sendto(sock,message, strlen(message),0,(struct sockaddr*)&servAdr, sizeof(servAdr));

            // adr_sz 변수 초기화 (수신하는 주소 크기 설정)
            adr_sz = sizeof(from_adr);
            
            // recvfrom: UDP 패킷 수신
            strLen=recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);

            if(strLen!= -1){
                message[strLen]=0;
                printf("Message from server: %s", message);
            }
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message, stderr); 
    fputc('\n', stderr);
    exit(1);
}
