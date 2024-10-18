#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
// 10월 10일 목 - CH06강 <UDP>

#define BUF_SIZE 30  // 메시지 버퍼 크기 정의

void ErrorHandling(char *message);  

int main(int argc, char *argv[]){

    WSADATA wsaData;  // 윈도우 소켓 초기화를 위한 구조체
    SOCKET sock;  // 소켓 핸들
    char message[BUF_SIZE];  // 메시지 버퍼
    int strLen;  // 수신된 메시지 길이 저장 변수
    int adr_sz; // 추가: 수신 주소 크기를 저장할 변수

    SOCKADDR_IN servAdr;  // 서버 주소 구조체
    SOCKADDR_IN from_adr;  // 수신할 클라이언트 주소 구조체

    // IP 주소와 포트 번호가 입력되지 않은 경우 사용법 안내
    if(argc != 3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 윈도우 소켓 라이브러리 초기화
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    // UDP 소켓 생성 (SOCK_DGRAM 사용)
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    // 서버 주소 구조체 초기화
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;  // IPv4 사용
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);  // IP 주소 설정
    servAdr.sin_port = htons(atoi(argv[2]));  // 포트 번호 설정

    while(1)
    {
        fputs("Input message(Q to quit): ", stdout);  // 메시지 입력 요청
        fgets(message, BUF_SIZE, stdin);  // 메시지 입력 받기

        // 'q' 또는 'Q' 입력 시 종료
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 입력된 메시지가 있으면 전송
        if(strlen(message) != 0){
            // sendto: 서버에 UDP 패킷 전송
            sendto(sock, message, strlen(message), 0, (struct sockaddr*)&servAdr, sizeof(servAdr));

            // adr_sz 변수 초기화 (수신할 주소 크기 설정)
            adr_sz = sizeof(from_adr);
            
            // recvfrom: 서버로부터 UDP 패킷 수신
            strLen = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);

            // 수신된 메시지의 길이가 -1이 아니면 메시지 출력
            if(strLen != -1){
                message[strLen] = 0;  // 문자열 끝에 NULL 문자 추가
                printf("Message from server: %s", message);  // 서버로부터 받은 메시지 출력
            }
        }
    }

    closesocket(sock);  // 소켓 닫기
    WSACleanup();  // 윈도우 소켓 종료
    return 0;
}

void ErrorHandling(char *message){
    fputs(message, stderr); 
    fputc('\n', stderr);
    exit(1);
}
