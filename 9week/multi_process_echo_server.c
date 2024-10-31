#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h> // memset 함수 선언 포함

// 10월 31일 목요일
// 실습04 - sigaction() 함수를 통해 자식 프로세스 종료 시 좀비 프로세스를 처리하는 함수 사용

#define BUF_SIZE 30 // 버퍼 크기 정의
void error_handling(char *message); // 에러 메시지 출력 함수 선언
void read_childproc(int sig); // 시그널 핸들러 함수 선언

int main(int argc, char *argv[]) {

    int serv_sock, clnt_sock; // 서버 소켓과 클라이언트 소켓
    struct sockaddr_in serv_adr, clnt_adr; // 서버와 클라이언트 주소 정보 구조체

    pid_t pid; // 프로세스 ID
    struct sigaction act; // 시그널 처리를 위한 sigaction 구조체
    int str_len, state; // 문자열 길이와 sigaction 상태 변수
    char buf[BUF_SIZE]; // 데이터 송수신을 위한 버퍼
    int adr_sz; // 클라이언트 주소 크기 변수

    // 프로그램 실행 시 포트 번호를 인자로 받는지 확인
    if (argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1); // 인자가 부족하면 프로그램 종료
    }

    // SIGCHLD 시그널을 처리할 핸들러 등록
    act.sa_handler = read_childproc; // 자식 프로세스 종료 시 호출할 함수 설정
    sigemptyset(&act.sa_mask); // 시그널 마스크 초기화
    act.sa_flags = 0; // 추가 옵션 없음
    state = sigaction(SIGCHLD, &act, 0); // SIGCHLD 시그널에 대한 처리 등록

    // 서버 소켓 생성 (TCP 소켓 생성)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    // 서버 주소 구조체 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr)); // 구조체를 0으로 초기화
    serv_adr.sin_family = AF_INET; // IPv4 주소 체계
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 IP에서 연결 허용
    serv_adr.sin_port = htons(atoi(argv[1])); // 입력받은 포트 번호 설정

    // 서버 소켓과 서버 주소 구조체를 바인딩
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error"); // 바인딩 실패 시 에러 처리

    // 서버 소켓이 연결 요청을 대기할 수 있도록 설정 (최대 대기 큐 크기: 5)
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error"); // listen 실패 시 에러 처리

    while (1) {
        // 클라이언트의 주소 정보 크기 설정
        adr_sz = sizeof(clnt_adr);
        // 클라이언트의 연결 요청을 수락
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

        if (clnt_sock == -1) // 연결 실패 시
            continue;
        else
            puts("new client connected..."); // 클라이언트 연결 성공 시 메시지 출력

            pid = fork(); // 새로운 프로세스 생성

            if (pid == -1) { // fork 실패 시
                close(clnt_sock); // 클라이언트 소켓 닫기
                continue;
            }

            if (pid == 0) { // 자식 프로세스
                close(serv_sock); // 자식 프로세스에서는 서버 소켓을 닫음
                // 클라이언트로부터 데이터 읽고, 다시 클라이언트로 전송 (에코)
                while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
                    write(clnt_sock, buf, str_len);

                close(clnt_sock); // 클라이언트 소켓 닫기
                puts("client disconnected..."); // 클라이언트 연결 종료 메시지 출력
                return 0; // 자식 프로세스 종료
            } else
                close(clnt_sock); // 부모 프로세스는 클라이언트 소켓을 닫음
    }
    close(serv_sock); // 서버 소켓 닫기
    return 0;
}

// 자식 프로세스 종료 시 호출되는 시그널 핸들러 함수
void read_childproc(int sig) {
    pid_t pid;
    int status;
    // 종료된 자식 프로세스의 PID와 상태 정보를 비동기로 확인
    pid = waitpid(-1, &status, WNOHANG);
    printf("Removed proc id: %d \n", pid); // 종료된 자식 프로세스의 ID 출력
}

// 에러 발생 시 메시지를 출력하고 프로그램을 종료하는 함수
void error_handling(char *message) {
    fputs(message, stderr); // 에러 메시지 출력
    fputc('\n', stderr); // 줄바꿈 출력
    exit(1); // 프로그램 종료
}
