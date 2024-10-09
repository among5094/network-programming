#include <stdio.h>       // 표준 입출력 함수 사용을 위한 헤더
#include <stdlib.h>      // exit() 함수 사용을 위한 헤더
#include <string.h>      // 문자열 관련 함수 사용을 위한 헤더
#include <winsock2.h>    // 윈도우 소켓 프로그래밍 관련 함수 및 구조체 정의

// 버퍼 크기 정의 (서버와 주고받을 데이터의 최대 크기)
#define BUF_SIZE 1024

// 오류 발생 시 메시지를 출력하고 프로그램 종료
void ErrorHandling(char *message);

int main(int argc, char *argv[]){

    WSADATA wsaData;     // 윈도우 소켓 초기화 정보 저장
    SOCKET hSocket;      // 클라이언트 소켓 (서버와의 연결에 사용)
    char message[BUF_SIZE];  // 서버와 주고받을 데이터를 저장할 버퍼
    int strLen;          // 수신한 데이터의 길이 저장
    SOCKADDR_IN servAdr;  // 서버 주소 정보를 저장할 구조체

    // 프로그램 실행 시 IP 주소와 포트 번호를 인자로 받아야 함
    if(argc != 3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 윈도우 소켓 라이브러리 초기화 (버전 2.2 사용)
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    // 클라이언트 소켓 생성 (IPv4, TCP 소켓, 기본 프로토콜)
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");

    // 서버 주소 구조체 초기화 및 설정
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;  // 주소 체계는 IPv4 사용
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);  // 입력받은 IP 주소 설정
    servAdr.sin_port = htons(atoi(argv[2]));  // 입력받은 포트 번호 설정

    // 서버에 연결 요청 (클라이언트 소켓을 서버 소켓과 연결)
    if(connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("connect() error!");
    else
        puts("Connected...........");  // 연결 성공 메시지 출력

    // 메시지 송수신 루프
    while(1)
    {
        // 사용자에게 입력을 요청하고 메시지를 읽음
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        // 입력받은 메시지가 "q" 또는 "Q"일 경우 프로그램 종료
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 서버에 메시지 전송
        send(hSocket, message, strlen(message), 0);

        // 서버로부터 메시지를 수신
        strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
        message[strLen] = 0;  // 문자열 끝에 NULL 문자 추가
        printf("Message from server: %s", message);  // 수신한 메시지 출력
    }

    // 클라이언트 소켓 닫기
    closesocket(hSocket);
    // 윈도우 소켓 라이브러리 정리
    WSACleanup();
    return 0;
}

// 오류 발생 시 에러 메시지를 출력하고 프로그램을 종료하는 함수
void ErrorHandling(char *message){
    fputs(message, stderr);  // 표준 오류 출력에 메시지 출력
    fputc('\n', stderr);     // 줄바꿈 문자 추가
    exit(1);                 // 프로그램 종료
}
