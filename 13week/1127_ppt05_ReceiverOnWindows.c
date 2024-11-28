
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // for struct ip_mreq
// 실습5-. 브로드캐스트 Receiver 구현 on Windows

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[]){

    WSADATA wsaData;
    int recv_sock;
    struct sockaddr_in adr;
    int str_len;
    char buf[BUF_SIZE];
 
    if (argc!=2){
        printf("Usage: %s <PORT>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup (MAKEWORD (2, 2), &wsaData)!=0)
        error_handling("WSAStartup() error!");

    recv_sock=socket (PF_INET, SOCK_DGRAM, 0);

    memset(&adr, 0, sizeof(adr));
    adr.sin_family=AF_INET;
    adr.sin_addr.s_addr=htonl(INADDR_ANY);
    adr.sin_port=htons(atoi(argv[1]));

    if (bind(recv_sock, (SOCKADDR*)&adr, sizeof(adr))==-1)
        error_handling("bind() error");

    
    while(1)
    {
        str_len=recvfrom (recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);
        if(str_len<0)
            break;
        buf[str_len]=0;
        fputs(buf, stdout);
    }
    
    closesocket(recv_sock);
    WSACleanup();
    return 0;

}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);

}