#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100 // 버퍼 크기 정의

// 11월 14일 목
// 실습01 - 2. IO 멀티플렉싱 기반 서버 구현 (입출력 대상을 묶어서 관리하는 방식으로 서비스 제공)

// 에러 메시지를 출력하는 함수
void error_handling(char *buf);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock; // 서버 소켓과 클라이언트 소켓 선언
    struct sockaddr_in serv_adr, clnt_adr; // 서버와 클라이언트의 주소 정보를 담을 구조체
    struct timeval timeout; // 타임아웃을 설정하기 위한 구조체
    fd_set reads, cpy_reads; // 소켓 관리를 위한 파일 디스크립터 셋 (복사용 포함)

    socklen_t adr_sz; // 주소 크기
    int fd_max, str_len, fd_num, i; // 파일 디스크립터 최대값, 읽은 데이터 길이, select 결과값, 반복문 변수
    char buf[BUF_SIZE]; // 데이터를 저장할 버퍼

    // 포트 번호가 입력되지 않은 경우 사용법을 출력하고 종료
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 서버 소켓 생성 (IPv4, TCP/IP 프로토콜)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    // 서버 주소 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET; // IPv4 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 IP에서 접속 허용
    serv_adr.sin_port = htons(atoi(argv[1])); // 입력받은 포트 번호 사용

    // 서버 소켓을 서버 주소와 연결 (bind)
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // 서버 소켓을 리슨 상태로 설정
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // fd_set을 초기화하고 서버 소켓을 추가
    FD_ZERO(&reads); // reads 초기화
    FD_SET(serv_sock, &reads); // 서버 소켓을 reads에 추가
    fd_max = serv_sock; // 현재 최대 파일 디스크립터는 서버 소켓

    while (1) {
        cpy_reads = reads; // fd_set 복사 (select는 원본을 수정하므로)
        timeout.tv_sec = 5; // 타임아웃 5초 설정
        timeout.tv_usec = 5000; // 추가로 5000 마이크로초 설정

        // 파일 디스크립터 셋에 변화가 생길 때까지 대기
        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;

        // 타임아웃이 발생한 경우, 반복문 재진입
        if (fd_num == 0)
            continue;

        // 변화가 발생한 파일 디스크립터를 모두 확인
        for (i = 0; i < fd_max + 1; i++) {
            // i번째 파일 디스크립터에 변화가 있는지 확인
            if (FD_ISSET(i, &cpy_reads)) {

                // 서버 소켓에 변화가 발생한 경우, 클라이언트의 접속 요청 처리
                if (i == serv_sock) {
                    adr_sz = sizeof(clnt_adr); // 클라이언트 주소 크기 설정
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz); // 클라이언트 연결 수락
                    FD_SET(clnt_sock, &reads); // 새로 연결된 클라이언트 소켓을 reads에 추가

                    if (fd_max < clnt_sock)
                        fd_max = clnt_sock; // fd_max 업데이트 (최대값)
                    printf("connected client on file descriptor: %d \n", clnt_sock); // 연결된 클라이언트 출력

                } else {
                    // 클라이언트 소켓에 데이터가 있는 경우, 데이터를 읽고 에코
                    str_len = read(i, buf, BUF_SIZE);
                    if (str_len == 0) { // 클라이언트가 종료한 경우
                        FD_CLR(i, &reads); // 클라이언트 소켓을 reads에서 제거
                        close(i); // 클라이언트 소켓 종료
                        printf("closed client: %d \n", i); // 종료된 클라이언트 출력
                    } else {
                        write(i, buf, str_len); // 클라이언트에게 받은 데이터를 그대로 전송 (에코)
                    }
                }
            }
        }
    }
    close(serv_sock); // 서버 소켓 종료
    return 0;
}

// 에러 메시지 출력 함수
void error_handling(char *buf) {
    fputs(buf, stderr); // 에러 메시지를 표준 에러에 출력
    fputc('\n', stderr);
    exit(1); // 프로그램 종료
}
