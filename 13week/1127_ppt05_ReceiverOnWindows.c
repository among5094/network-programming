#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // 브로드캐스트 및 멀티캐스트 관련 구조체 사용을 위한 헤더 파일

// 11월 28일 목요일
// 실습5) 브로드캐스트 Receiver 구현 on Windows

#define BUF_SIZE 30 // 수신할 데이터의 버퍼 크기

void error_handling(char *message); // 에러 처리 함수 선언

int main(int argc, char *argv[]) {

    WSADATA wsaData; // 윈속 초기화를 위한 구조체
    int recv_sock; // 데이터를 수신하기 위한 소켓
    struct sockaddr_in adr; // 수신 주소 정보를 저장할 구조체
    int str_len; // 수신한 데이터의 길이를 저장할 변수
    char buf[BUF_SIZE]; // 수신 데이터를 저장할 버퍼

    // 프로그램 실행 시 입력 인자 확인
    if (argc != 2) {
        printf("Usage: %s <PORT>\n", argv[0]); // 사용법 안내 메시지
        exit(1); // 입력 인자가 올바르지 않으면 프로그램 종료
    }

    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // 초기화 실패 시 에러 처리
        error_handling("WSAStartup() error!");

    // 수신 소켓 생성 (UDP 사용)
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (recv_sock == INVALID_SOCKET) // 소켓 생성 실패 시 에러 처리
        error_handling("socket() error");

    // 소켓 주소 설정
    memset(&adr, 0, sizeof(adr)); // 구조체 초기화
    adr.sin_family = AF_INET; // 주소 체계 지정 (IPv4)
    adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에서 수신
    adr.sin_port = htons(atoi(argv[1])); // 수신할 포트 번호 설정

    // 소켓에 주소 바인딩
    if (bind(recv_sock, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR) // 바인딩 실패 시 에러 처리
        error_handling("bind() error");

    // 데이터 수신 루프
    while (1) {
        str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0); // 데이터 수신
        if (str_len < 0)
            break;
        buf[str_len] = 0; // 문자열 끝에 NULL 추가
        fputs(buf, stdout); // 수신된 데이터 출력
    }

    // 소켓 닫기 및 윈속 종료
    closesocket(recv_sock); // 소켓 닫기
    WSACleanup(); // 윈속 정리
    return 0; // 프로그램 정상 종료
}

// 에러 처리 함수
void error_handling(char *message) {
    fputs(message, stderr); // 에러 메시지를 표준 에러 출력
    fputc('\n', stderr); // 개행 문자 추가
    exit(1); // 프로그램 종료
}
