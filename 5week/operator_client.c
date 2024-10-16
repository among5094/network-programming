#include <stdio.h>       // 표준 입출력 함수 사용을 위한 헤더
#include <stdlib.h>      // exit() 함수 사용을 위한 헤더
#include <string.h>      // 문자열 관련 함수 사용을 위한 헤더
#include <winsock2.h>    // 윈도우 소켓 프로그래밍 관련 함수 및 구조체 정의

// 버퍼 크기 정의 (서버와 주고받을 데이터의 최대 크기)
#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

// 오류 처리를 위한 함수 선언
void ErrorHandling(char* message);  

int main(int argc, char *argv[]){

    WSADATA wsaData;     // 윈도우 소켓 초기화 정보 저장
    SOCKET hSocket;      // 클라이언트 소켓 (서버와의 연결에 사용)

    char opmsg[BUF_SIZE];  
    int result,opndCnt,i;         
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

    ///////////////////////////////////////////////////////
    fputs("Operand count: ", stdout);
    scanf("%d", &opndCnt);
    opmsg[0]=(char)opndCnt;

    for(i=0;i<opndCnt; i++)
    {
        printf("Operand %d: ", i+1);
        scanf("%d", (int *)&opmsg[i*OPSZ+1]);
    }
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opndCnt*OPSZ+1]);

    send (hSocket, opmsg, opndCnt*OPSZ+2,0);
    recv(hSocket, &result, RLT_SIZE, 0);


    printf("Operation result: %d\n", result);

    ///////////////////////////////////////////////////////

    
    closesocket(hSocket);// 클라이언트 소켓 닫기
    WSACleanup();// 윈도우 소켓 라이브러리 정리
    return 0;
}

// 오류 발생 시 에러 메시지를 출력하고 프로그램을 종료하는 함수
void ErrorHandling(char *message){
    fputs(message, stderr);  // 표준 오류 출력에 메시지 출력
    fputc('\n', stderr);     
    exit(1);                 // 프로그램 종료
}
