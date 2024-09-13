#include <stdio.h>        // 표준 입출력 함수 사용을 위한 헤더 파일
#include <stdlib.h>       // 표준 라이브러리 함수 사용을 위한 헤더 파일 (예: exit, atoi)
#include <string.h>       // 문자열 관련 함수 사용을 위한 헤더 파일 (예: memset)
#include <unistd.h>       // UNIX 표준 함수 사용을 위한 헤더 파일 (예: close)
#include <arpa/inet.h>    // 인터넷 관련 함수 및 구조체 사용을 위한 헤더 파일 (예: sockaddr_in, htonl, htons)
#include <sys/socket.h>   // 소켓 프로그래밍 관련 함수 사용을 위한 헤더 파일 (예: socket, bind, listen, accept)

// 오류 처리 함수의 선언
void error_handling(char *message);  

int main(int argc, char *argv[]) {
    int serv_sock;                 // 서버 소켓 파일 디스크립터
    int clnt_sock;                 // 클라이언트 소켓 파일 디스크립터

    struct sockaddr_in serv_addr;  // 서버 주소 정보를 담는 구조체
    struct sockaddr_in clnt_addr;  // 클라이언트 주소 정보를 담는 구조체
    socklen_t clnt_addr_size;      // 클라이언트 주소 크기를 저장하는 변수

    char message[] = "Hello World!";  // 클라이언트에 보낼 메시지

    // 명령행 인자가 2개인지 확인 (실행 파일명 + 포트번호)
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);  // 프로그램 종료
    }

    // 1. 소켓 생성
    // socket(int domain, int type, int protocol)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);  // IPv4 주소체계 사용, TCP 방식의 소켓 dfdfdfdfdf
    if (serv_sock == -1)
        error_handling("socket() error");  // 소켓 생성 실패 시 오류 처리

    // 2. 서버 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));           // serv_addr 구조체를 0으로 초기화
    serv_addr.sin_family = AF_INET;                     // 주소 체계는 IPv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);      // 서버 IP는 현재 사용하는 모든 IP (로컬 호스트의 IP)
    serv_addr.sin_port = htons(atoi(argv[1]));          // 포트번호는 명령행 인자로 받은 값을 네트워크 바이트 순서로 변환

    // 3. 소켓에 서버 주소 바인딩
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");  // 바인딩 실패 시 오류 처리

    // 4. 연결 요청 대기 상태로 전환
    if (listen(serv_sock, 5) == -1)  // 최대 5개의 클라이언트 연결 요청을 대기
        error_handling("listen() error");  // listen 실패 시 오류 처리

    // 5. 클라이언트 요청 수락
    clnt_addr_size = sizeof(clnt_addr);  // 클라이언트 주소 구조체 크기 저장
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);  // 클라이언트 연결 수락

    if (clnt_sock == -1)
        error_handling("accept() error");  // 클라이언트 연결 수락 실패 시 오류 처리

    // 6. 클라이언트에 메시지 전송
    write(clnt_sock, message, sizeof(message));  // "Hello World!" 메시지를 클라이언트에 전송

    // 7. 클라이언트와 서버 소켓 닫기
    close(clnt_sock);  // 클라이언트 소켓 닫기
    close(serv_sock);  // 서버 소켓 닫기

    return 0;  // 프로그램 정상 종료
}

// 오류 처리 함수 정의
void error_handling(char *message) {
    fputs(message, stderr);  // 에러 메시지를 표준 오류로 출력
    fputc('\n', stderr);     // 개행 문자 출력
    exit(1);                 // 프로그램 종료
}
