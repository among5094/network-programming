#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
// 10월 17일 목요일 - DNS/IP 주소 간 변환 (gethostbyname() 사용)
// '8.8.8.8'에 대해 도메인 이름 'dns.google'이 나오도록 수정

void ErrorHandling(char *message);

int main(int argc, char *argv[]){

    WSADATA wsaData; // 윈속 초기화 정보 저장 구조체
    int i; // 반복문에서 사용할 변수
    struct hostent *host; // 호스트 정보를 저장할 구조체 포인터
    struct in_addr addr;  // IP 주소 저장을 위한 구조체

    // 프로그램 인자 확인
    if(argc != 2) {
        printf("Usage: %s <IP address>\n", argv[0]);
        exit(1);
    }

    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    // IP 주소를 문자열에서 네트워크 바이트 순서로 변환
    addr.s_addr = inet_addr(argv[1]); 

    // 네트워크 바이트 순서의 IP 주소를 이용해 도메인 이름을 찾음
    host = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET); 
    if (host == NULL)
        ErrorHandling("gethostbyaddr() error!");

    // 도메인 이름 출력
    printf("Official name: %s \n", host->h_name);

    // 별칭 출력
    for (i = 0; host->h_aliases[i]; i++)
        printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);

    // 주소 타입 출력 (IPv4 또는 IPv6)
    printf("Address type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

    // 윈속 종료 처리
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message) {
    fputs(message, stderr); 
    fputc('\n', stderr);
    exit(1);
}
