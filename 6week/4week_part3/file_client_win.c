#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
// 10월 10일 목 - 4주차 Part3 <TCP의 우아한 종료>

#define BUF_SIZE 30 // 버퍼 크기 정의

void ErrorHandling(char *message);

int main(int argc, char *argv[]){

    WSADATA wsaData; // 윈속 초기화 정보 저장 구조체
    SOCKET hSocket; // 클라이언트 소켓
    FILE *fp; // 파일 포인터

    char buf[BUF_SIZE]; // 데이터를 저장할 버퍼
    int readCnt; // 읽어온 데이터 크기를 저장할 변수
    SOCKADDR_IN servAdr; // 서버 주소 정보를 저장할 구조체

    // 입력된 인자 개수를 확인 -> 올바른 사용법이 아니면 프로그램 종료
    if(argc!=3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 윈속 초기화: 성공 시 0 반환, 실패 시 오류 처리  
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)
        ErrorHandling("WSAStartup() error!"); // WSAStartup 실패 시 에러 처리

    // 파일을 쓰기 모드로 열기
    fp=fopen("receive.dat", "wb");
    if(fp == NULL) // 파일 열기 실패 시 에러 처리
        ErrorHandling("Failed to open file");

    // 소켓 생성: PF_INET(IPv4), SOCK_STREAM(TCP 소켓)
    hSocket=socket(PF_INET, SOCK_STREAM, 0);
    if(hSocket == INVALID_SOCKET) // 소켓 생성 실패 시 에러 처리
        ErrorHandling("Socket creation error");
    
    // 서버 주소 구조체 초기화 및 설정
    memset(&servAdr, 0, sizeof(servAdr)); // 구조체 메모리 초기화
    servAdr.sin_family=AF_INET; // 주소 체계 설정 (IPv4)
    servAdr.sin_addr.s_addr=inet_addr(argv[1]); // 서버 IP 주소 설정
    servAdr.sin_port=htons(atoi(argv[2])); // 포트 번호 설정 (네트워크 바이트 순서로 변환)
    
    // 서버에 연결 요청
    if(connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("Connect error"); // 연결 실패 시 에러 처리
    
    // 서버로부터 데이터 수신 및 파일에 쓰기
    while((readCnt=recv(hSocket, buf, BUF_SIZE, 0))!=0) // 수신된 데이터가 있을 때까지 반복
        fwrite((void*)buf, 1, readCnt, fp); // 파일에 수신한 데이터 쓰기

    puts("Received file data"); // 파일 수신 완료 메시지 출력

    // 서버에 "Thank you" 메시지 전송
    send(hSocket, "Thank you", 10, 0);

    // 파일 닫기 및 소켓 종료
    fclose(fp); // 파일 닫기
    closesocket(hSocket); // 소켓 닫기
    WSACleanup(); // 윈속 종료 처리
    return 0;
}

// 에러 메시지를 출력하고 프로그램을 종료하는 함수 정의
void ErrorHandling(char *message){
    fputs(message, stderr); // 에러 메시지 출력
    fputc('\n', stderr); // 줄바꿈 문자 출력
    exit(1); // 프로그램 종료
}
