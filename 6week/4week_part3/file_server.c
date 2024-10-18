#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// 10월 10일 목 - 4week의 Part3 <TCP의 우아한 종료>

#define BUF_SIZE 30 // 데이터를 읽고 쓸 때 사용할 버퍼의 크기 정의

void error_handling(char *message);

int main(int argc, char *argv[]){

    int serv_sd, clnt_sd; // 서버와 클라이언트 소켓 디스크립터 선언
    FILE *fp; // 파일 포인터 선언
    char buf[BUF_SIZE]; // 데이터를 읽고 쓸 버퍼 선언
    int read_cnt; // 파일에서 읽은 바이트 수를 저장하는 변수

    struct sockaddr_in serv_adr, clnt_adr; // 서버와 클라이언트 주소 정보를 저장할 구조체 선언
    socklen_t clnt_adr_sz; // 클라이언트 주소 구조체 크기 변수

    // 인자 개수 확인, 사용법이 올바르지 않으면 프로그램 종료
    if(argc!=2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 파일을 클라이언트에게 전송하기 위해서  읽기모드로 열기
    // (파일명: "file_server.c", 읽기 전용, 바이너리 모드)
    fp=fopen("file_server.c", "rb");
    if(fp == NULL) // 파일 열기 실패 시 에러 처리
        error_handling("Failed to open file");

    // 소켓 생성: PF_INET(IPv4), SOCK_STREAM(TCP 소켓)
    serv_sd=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sd == -1) // 소켓 생성 실패 시 에러 처리
        error_handling("Socket creation error");

    // 서버 주소 구조체 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr)); // 구조체 메모리 초기화
    serv_adr.sin_family=AF_INET; // 주소 체계 설정 (IPv4)
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY); // 서버의 IP 주소 설정 (모든 IP에서 연결 허용)
    serv_adr.sin_port=htons(atol(argv[1])); // 포트 번호 설정 (네트워크 바이트 순서로 변환)

    // 소켓과 서버 주소를 바인딩
    if(bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("Bind error");

    // 소켓을 수신 대기 상태로 설정 (대기열 크기: 5)
    if(listen(serv_sd, 5) == -1)
        error_handling("Listen error");

    // 클라이언트 연결 요청 수락
    clnt_adr_sz=sizeof(clnt_adr); // 클라이언트 주소 구조체 크기 설정
    clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if(clnt_sd == -1) // 연결 수락 실패 시 에러 처리
        error_handling("Accept error");

    // 파일 내용을 읽어 클라이언트에게 전송
    while(1){
        // 파일에서 데이터를 읽어 buf에 저장 (최대 BUF_SIZE 바이트)
        read_cnt=fread((void*)buf, 1, BUF_SIZE, fp);

        // 읽은 데이터의 크기가 BUF_SIZE보다 작으면 (파일 끝에 도달한 경우)
        if(read_cnt<BUF_SIZE){
            write(clnt_sd, buf, read_cnt); // 읽은 만큼만 전송
            break; // 루프 종료 (파일 전송 완료)
        }
        write(clnt_sd, buf, BUF_SIZE); // BUF_SIZE만큼 전송
    }

    sleep(2); // 2초 대기 (클라이언트가 데이터를 모두 받을 시간을 줌)

    //////////////////////////////////////////////////////////

    // 핵심내용: 파일 전송 후 출력 스트림(SHUT_WR)에 대한 Half-close
    // 클라이언트에게 EOF가 전송되고, 클라이언트는 파일전송이 완료되었음을 인식할 수 있다.
    shutdown(clnt_sd, SHUT_WR);

    // 출력 스트림(SHUT_WR)만 닫았기 때문에 입력 스트림을 통한 데이터 수신 가능
    read(clnt_sd, buf, BUF_SIZE); // 데이터를 buf에 읽어 저장

    //////////////////////////////////////////////////////////

    printf("Message from client: %s \n", buf); // 클라이언트로부터 받은 메시지 출력
    
    fclose(fp); // 파일 닫기
    close(clnt_sd); // 클라이언트 소켓 닫기
    close(serv_sd); // 서버 소켓 닫기
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr); // 에러 메시지 출력
    fputc('\n', stderr); 
    exit(1);
}
