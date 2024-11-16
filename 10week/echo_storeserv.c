#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

// 11월 07일 목
// 실습04 - pipe() 기반 에코 서비스 서버 만들기

// 함수 선언
void error_handling(char *message); // 오류 발생 시 메시지를 출력하고 프로그램 종료
void read_childproc(int sig); // 자식 프로세스 종료 시 호출되는 시그널 핸들러

// 전역 버퍼 선언
char buf[BUF_SIZE];

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock; // 서버 소켓과 클라이언트 소켓
    struct sockaddr_in serv_adr, clnt_adr; // 서버와 클라이언트의 주소 정보 구조체
    int fds[2]; // 파이프 파일 디스크립터 배열: fds[0]은 읽기용, fds[1]은 쓰기용

    pid_t pid; // 프로세스 ID를 저장할 변수
    struct sigaction act; // 시그널 핸들러 설정용 구조체
    socklen_t adr_sz; // 클라이언트 주소 크기 저장용 변수
    int str_len, state; // 문자열 길이와 상태 저장 변수

    // 프로그램 실행 시 포트 번호를 전달하지 않은 경우
    if (argc != 2) {
        printf("Usage: %s <port> \n", argv[0]); // 사용법 출력
        exit(1); // 프로그램 종료
    }

    // SIGCHLD 시그널 처리 설정
    act.sa_handler = read_childproc; // 시그널 발생 시 호출할 핸들러 함수
    sigemptyset(&act.sa_mask); // 시그널 마스크 초기화
    sigaction(SIGCHLD, &act, 0); // SIGCHLD 발생 시 read_childproc 호출하도록 설정

    // 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0); // TCP/IP 소켓 생성

    // 서버 주소 정보 설정
    memset(&serv_adr, 0, sizeof(serv_adr)); // 구조체 초기화
    serv_adr.sin_family = AF_INET; // IPv4 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 IP에서 연결 허용
    serv_adr.sin_port = htons(atoi(argv[1])); // 포트 번호 설정

    // 소켓에 주소 할당
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // 연결 요청 대기 상태로 전환
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // 파이프 생성
    pipe(fds); // fds[0]: 읽기, fds[1]: 쓰기

    // 파일 저장을 위한 자식 프로세스 생성
    pid = fork(); // 새로운 프로세스 생성
    
    if (pid == 0) { 
        // 자식 프로세스
        FILE *fp = fopen("echomsg.txt", "wt"); // 메시지를 저장할 파일 열기
        char msgbuf[BUF_SIZE]; // 메시지를 읽어올 버퍼
        int i, len;

        // 부모 프로세스가 파이프에 쓰는 데이터를 읽고 파일에 저장
        for (i = 0; i < 3; i++) {
            len = read(fds[0], msgbuf, BUF_SIZE); // 파이프에서 데이터 읽기
            fwrite((void*)msgbuf, 1, len, fp); // 파일에 데이터 쓰기
        }
        fclose(fp); // 파일 닫기
        return 0; // 자식 프로세스 종료
    }

    // 메인 루프: 클라이언트 연결 요청 처리
    while (1) {
        adr_sz = sizeof(clnt_adr);
        // 클라이언트 연결 수락
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if (clnt_sock == -1)
            continue; // accept 실패 시 다음 요청 처리
        else
            puts("new client connected..."); // 새로운 클라이언트 연결 메시지 출력

        pid = fork(); // 클라이언트 요청 처리를 위한 자식 프로세스 생성

        if (pid == 0) { 
            // 자식 프로세스: 클라이언트 요청 처리
            close(serv_sock); // 자식 프로세스에서 서버 소켓은 필요 없으므로 닫음
            // 클라이언트로부터 데이터 수신 및 처리
            while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
                write(clnt_sock, buf, str_len); // 에코 데이터 전송
                write(fds[1], buf, str_len); // 파이프에 데이터 쓰기 (파일 저장용)
            }
            close(clnt_sock); // 클라이언트 소켓 닫기
            puts("client disconnected..."); // 클라이언트 연결 종료 메시지 출력
            return 0; // 자식 프로세스 종료
        } else
            close(clnt_sock); // 부모 프로세스에서는 클라이언트 소켓 닫기
    }

    close(serv_sock); // 서버 소켓 닫기
    return 0;
}

// 오류 발생 시 메시지 출력 후 프로그램 종료
void error_handling(char *message) {
    fputs(message, stderr); // 표준 오류 출력에 메시지 출력
    fputc('\n', stderr); // 줄바꿈 문자 출력
    exit(1); // 프로그램 종료
}

// 자식 프로세스 종료 시 호출되는 핸들러
void read_childproc(int sig) {
    int status;
    // 종료된 자식 프로세스 ID 및 상태 정보 수집
    pid_t id = waitpid(-1, &status, WNOHANG);

    // 자식 프로세스가 정상적으로 종료된 경우
    if (WIFEXITED(status)) {
        printf("Removed proc id: %d \n", id); // 종료된 자식 프로세스 ID 출력
        printf("Child send: %d \n", WEXITSTATUS(status)); // 자식 프로세스의 종료 코드 출력
    }
}
