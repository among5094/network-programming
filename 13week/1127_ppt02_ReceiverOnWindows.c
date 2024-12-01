#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // 멀티캐스트 관련 구조체(ip_mreq)를 사용하기 위한 헤더 파일

// 11월 28일 목요일
// 실습2) 멀티캐스트 Receiver 구현 on Windows

#define BUF_SIZE 30 // 수신할 데이터의 버퍼 크기

void ErrorHandling(char *message); // 에러 처리 함수 선언

int main(int argc, char *argv[]) {

    WSADATA wsaData; // 윈속 초기화를 위한 구조체
    SOCKET hRecvSock; // 데이터 수신에 사용할 소켓
    SOCKADDR_IN adr; // 소켓 주소 구조체
    struct ip_mreq joinAdr; // 멀티캐스트 그룹 가입 정보를 저장하는 구조체
    char buf[BUF_SIZE]; // 데이터를 수신할 버퍼
    int strLen; // 수신된 데이터의 길이를 저장할 변수

    // 프로그램 실행 시 인자의 개수를 확인
    if (argc != 3) {
        printf("Usage: %s <GroupIP> <PORT>\n", argv[0]);
        exit(1); 
    }

    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
        ErrorHandling("WSAStartup() error!");

    // 수신 소켓 생성 (UDP 사용)
    hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
    if (hRecvSock == INVALID_SOCKET) 
        ErrorHandling("socket() error");

    // 소켓 주소 설정
    memset(&adr, 0, sizeof(adr)); // 구조체 초기화
    adr.sin_family = AF_INET; // 주소 체계 지정 (IPv4)
    adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에서 수신
    adr.sin_port = htons(atoi(argv[2])); // 수신할 포트 번호 설정

    // 소켓에 주소 바인딩
    if (bind(hRecvSock, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");

    // 멀티캐스트 그룹 가입 설정
    joinAdr.imr_multiaddr.s_addr = inet_addr(argv[1]); // 멀티캐스트 그룹의 IP 주소
    joinAdr.imr_interface.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스 사용

    // 멀티캐스트 그룹 가입 실패 시 에러 처리
    if (setsockopt(hRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
            (void*)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR) 
        ErrorHandling("setsockopt() error");

    // 데이터 수신 루프
    while (1) {
        strLen = recvfrom(hRecvSock, buf, BUF_SIZE - 1, 0, NULL, 0); // 데이터 수신
        
        // 수신 실패 시 루프 종료
        if (strLen < 0) 
            break;

        buf[strLen] = 0; // 문자열 끝에 NULL 추가
        fputs(buf, stdout); // 수신된 데이터 출력
    }

    // 소켓 닫기 및 윈속 종료
    closesocket(hRecvSock); // 소켓 닫기
    WSACleanup(); // 윈속 정리
    return 0; // 프로그램 정상 종료
}

// 에러 처리 함수
void ErrorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr); 
    exit(1);
}
