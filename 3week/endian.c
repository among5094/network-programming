#include <stdio.h>       // 표준 입출력 라이브러리
#include <stdlib.h>      // 표준 라이브러리 (일반 유틸리티 함수 포함)
#include <string.h>      // 문자열 관련 함수
#include <unistd.h>      // 유닉스 표준 함수들 (윈도우에서는 필요 없음)
#include <winsock2.h>    // 윈도우 소켓 라이브러리 (소켓 관련 함수들 포함)

int main(int argc, char* argv[]) {

    
    // 호스트 시스템에서 사용하는 포트 번호 (16비트)이고
    // 이 값을 네트워크 바이트 순서로 변환함
    unsigned short host_port = 0x1234;

    // 네트워크 바이트 순서로 변환된 포트 번호를 저장할 변수
    unsigned short net_port;

    // 호스트 시스템에서 사용하는 IP 주소 (32비트)
    // 이 값을 네트워크 바이트 순서로 변환할 것입니다.
    unsigned long host_addr = 0x12345678;

    // 네트워크 바이트 순서로 변환된 IP 주소를 저장할 변수
    unsigned long net_addr;

    // 윈도우 소켓 초기화 (Winsock 라이브러리 사용 준비)
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);

    // 호스트 바이트 순서에서 네트워크 바이트 순서로 "포트 번호 변환"
    // htons() 함수는 'host to network short'의 약자로, 16비트 값을 빅 엔디안 방식으로 변환
    net_port = htons(host_port);

    // 호스트 바이트 순서에서 네트워크 바이트 순서로 "IP 주소 변환"
    // htonl() 함수는 'host to network long'의 약자로, 32비트 값을 빅 엔디안 방식으로 변환
    net_addr = htonl(host_addr);

    // 호스트 바이트 순서의 포트 번호를 출력
    // %#x 형식 지정자를 사용하여 값을 16진수로 출력
    printf("Host ordered port: %#x \n", host_port);

    // 네트워크 바이트 순서로 변환된 포트 번호를 출력
    printf("Network ordered port: %#x \n", net_port);

    // 호스트 바이트 순서의 IP 주소를 출력
    printf("Host ordered address: %#x \n", host_addr);

    // 네트워크 바이트 순서로 변환된 IP 주소를 출력
    printf("Network ordered address: %#x \n", net_addr);

    // 프로그램 정상 종료
    return 0;
}
