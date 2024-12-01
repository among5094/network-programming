#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 11월 28일 목요일
// 실습4) 브로드캐스트 Sender 구현 on Linux

#define BUF_SIZE 30 // 송신할 데이터 버퍼 크기

void error_handling(char *message); // 에러 처리 함수 선언

int main(int argc, char *argv[]) {

    int send_sock; // 데이터를 송신하기 위한 소켓
    struct sockaddr_in broad_adr; // 브로드캐스트 주소를 저장할 구조체
    FILE *fp; // 파일 포인터 (전송할 데이터를 읽기 위해 사용)
    char buf[BUF_SIZE]; // 전송할 데이터 버퍼
    int so_brd = 1; // 브로드캐스트 옵션 활성화 변수

    // 프로그램 실행 시 입력 인자 확인
    if (argc != 3) {
        printf("Usage: %s <Broadcast IP> <PORT>\n", argv[0]);
        exit(1);
    }

    // 송신 소켓 생성 (UDP 사용)
    send_sock = socket(PF_INET, SOCK_DGRAM, 0); 
    if (send_sock == -1) 
        error_handling("socket() error");

    // 브로드캐스트 주소 설정
    memset(&broad_adr, 0, sizeof(broad_adr)); // 구조체 초기화
    broad_adr.sin_family = AF_INET; // 주소 체계 지정 (IPv4)
    broad_adr.sin_addr.s_addr = inet_addr(argv[1]); // 브로드캐스트 IP 주소 설정
    broad_adr.sin_port = htons(atoi(argv[2])); // 브로드캐스트 포트 번호 설정

    // 소켓 옵션 설정: 브로드캐스트 활성화
    if (setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd)) == -1)
        error_handling("setsockopt() error");

    // 파일 열기 실패 시 에러 처리
    if ((fp = fopen("news.txt", "r")) == NULL)
        error_handling("fopen() error");

    // 파일 내용을 읽어 브로드캐스트 전송
    while(!feof(fp)) {
        fgets(buf, BUF_SIZE, fp); // 파일로부터 데이터를 읽음
        if (feof(fp)) // 파일 끝에 도달하면 루프 종료
            break;

        // 데이터를 브로드캐스트 주소로 전송
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr)); 
        
        sleep(2); // 2초 간격으로 데이터 전송
    }

    // 소켓 닫기 및 프로그램 종료
    close(send_sock); // 소켓 닫기
    return 0; // 프로그램 정상 종료
}

// 에러 처리 함수
void error_handling(char *message) {
    fputs(message, stderr); // 에러 메시지를 표준 에러 출력
    fputc('\n', stderr); // 개행 문자 추가
    exit(1); // 프로그램 종료
}
