#include <stdio.h>       // 표준 입출력 함수 사용을 위한 헤더
#include <stdlib.h>      // exit() 함수 사용을 위한 헤더
#include <string.h>      // 문자열 관련 함수 사용을 위한 헤더
#include <unistd.h>      // UNIX 시스템 호출 관련 함수 (read, write, close)
#include <arpa/inet.h>   // 인터넷 프로토콜(IP) 관련 구조체와 함수 정의
#include <sys/socket.h>  // 소켓 관련 함수와 구조체 정의

// 버퍼 크기 정의 (서버와 클라이언트 간 송수신할 데이터의 최대 크기)
#define BUF_SIZE 1024

// 오류 발생 시 메시지를 출력하고 프로그램 종료
void error_handling(char *message);

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;  // 서버 소켓과 클라이언트 소켓을 저장할 변수
    char message[BUF_SIZE];    // 클라이언트와 주고받을 데이터를 저장할 버퍼
    int str_len, i;            // 읽은 데이터의 길이와 클라이언트 번호 저장 변수

    struct sockaddr_in serv_adr;  // 서버 주소 정보를 저장할 구조체
    struct sockaddr_in clnt_adr;  // 클라이언트 주소 정보를 저장할 구조체
    socklen_t clnt_adr_sz;        // 클라이언트 주소 크기

    // 프로그램 실행 시 포트 번호가 인자로 제공되지 않으면 사용법을 출력하고 종료
    if(argc!=2){
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    // 서버 소켓 생성 (IPv4, TCP 소켓, 기본 프로토콜)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    // 서버 주소 구조체 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;  // 주소 체계는 IPv4 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 서버의 IP는 자동으로 할당
    serv_adr.sin_port = htons(atoi(argv[1]));  // 입력받은 포트를 네트워크 바이트 순서로 변환

    // 서버 소켓과 서버 주소 구조체를 바인딩 (소켓과 IP, 포트를 연결)
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // 서버가 연결 요청을 대기할 수 있게 설정 (최대 5개의 연결 요청을 대기)
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // 클라이언트 주소 크기 설정
    clnt_adr_sz = sizeof(clnt_adr);

    // 5번의 클라이언트 연결을 처리
    for(i = 0; i < 5; i++){
        // 클라이언트의 연결 요청을 수락 (수락 시 클라이언트와 연결된 소켓 생성)
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n", i + 1);  // 연결된 클라이언트 번호 출력

        // 클라이언트가 보낸 데이터를 읽고 그대로 다시 전송 (에코 서버 기능)
        while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
            write(clnt_sock, message, str_len);

        // 클라이언트 소켓을 닫음 (하나의 클라이언트와 통신이 끝나면 소켓 종료)
        close(clnt_sock);
    }

    // 서버 소켓을 닫음 (모든 클라이언트와의 통신이 끝난 후 서버 소켓 종료)
    close(serv_sock);
    return 0;
}

// 오류 발생 시 에러 메시지를 출력하고 프로그램 종료하는 함수
void error_handling(char *message){
    fputs(message, stderr);  // 표준 오류 출력에 메시지를 출력
    fputc('\n', stderr);     // 줄바꿈 문자 추가
    exit(1);                 // 프로그램 종료
}
