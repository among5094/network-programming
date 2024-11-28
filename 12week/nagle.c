#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
// 11월 21일 목요일 - 실습01) Nagle 옵션 활성화/비활성화 하기

#define NAGLE_CONTROL FALSE // Nagle 알고리즘 제어 (사용 여부 설정)
#define MAX_TRANSMIT 4 // 메시지 전송 반복 횟수

#define BUF_SIZE 200 // 송신 버퍼 크기
#define BUF_RECV_SIZE 1024 // 수신 버퍼 크기

void ErrorHandling(char *message); // 오류 처리 함수 선언

int main(int argc, char *argv[]){

    WSADATA wsaData; // 윈속 초기화 구조체
    SOCKET hSocket; // 소켓 핸들
    char message[BUF_SIZE]; // 송신 메시지 저장 배열
    int str_len, recv_len, recv_cnt; // 메시지 길이 관련 변수
    SOCKADDR_IN servAdr; // 서버 주소 구조체

    int state; // 소켓 옵션 설정 상태를 저장할 변수

    // 인자가 3개(프로그램명, IP, 포트번호)가 아니면 종료
    if(argc != 3){
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // 윈속 라이브러리 초기화
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    // TCP 소켓 생성
    hSocket = socket(PF_INET, SOCK_STREAM, 0); // soket 오타 수정
    if(hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error!");

    //////////////////////////////////////////////////////////////////////////////
    // Nagle 옵션 설정: TCP_NODELAY 옵션을 이용하여 Nagle 알고리즘 활성화/비활성화
    int opt_val = MAX_TRANSMIT;
    state = setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));
    if (state == SOCKET_ERROR)
        ErrorHandling("setsockopt() error!");
    //////////////////////////////////////////////////////////////////////////////

    // 서버 주소 정보 초기화
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET; // IPv4 사용
    servAdr.sin_addr.s_addr = inet_addr(argv[1]); // IP 주소 설정
    servAdr.sin_port = htons(atoi(argv[2])); // 포트 번호 설정

    // 서버에 연결 요청
    if(connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("connect() error!"); \
    else
        puts("Connected..........");

    // 메시지 송수신 루프
    while(1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin); // 메시지 입력

        // 종료 조건: 'q' 또는 'Q' 입력 시 종료
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 메시지 반복 전송 (MAX_TRANSMIT 횟수만큼)
        for(int sendInput = 0; sendInput < MAX_TRANSMIT; sendInput++){
            str_len = send(hSocket, message, strlen(message), 0); // 메시지 전송
            if (str_len == SOCKET_ERROR)
                ErrorHandling("send() error!");
        }
    }

    // 소켓 종료 및 윈속 정리
    closesocket(hSocket);
    WSACleanup();
    return 0;    
}

void ErrorHandling(char *message){
    fputs(message, stderr); // 오류 메시지 출력
    fputc('\n', stderr);
    exit(1); // 프로그램 종료
}
