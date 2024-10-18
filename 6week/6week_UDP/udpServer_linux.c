#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// 10월 10일 목 - CH06강 <UDP>

#define BUF_SIZE 30  // 메시지를 받을 버퍼의 크기를 정의

void error_handling(char *message);  

int main(int argc, char *argv[]){

    int serv_sock;  // 서버 소켓 파일 디스크립터
    char message[BUF_SIZE];  // 수신된 메시지를 저장할 버퍼
    int str_len;  // 수신된 메시지의 길이를 저장하는 변수
    socklen_t clnt_adr_sz;  // 클라이언트 주소 구조체의 크기
    int connectCounter = 1;  // 연결된 클라이언트 수를 추적하는 카운터

    struct sockaddr_in serv_adr, clnt_adr;  // 서버와 클라이언트 주소 구조체

    // 프로그램 이름과 포트 번호가 제공되었는지 확인
    if(argc != 2){
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    // UDP 소켓 생성을 위해 socket 함수의 두 번째 인자로 SOCK_DGRAM 전달
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("UDP socket creation error");  // 소켓 생성 실패 시 에러 처리

    // 서버 주소 구조체 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;  // IPv4 인터넷 프로토콜 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 모든 인터페이스에 바인딩
    serv_adr.sin_port = htons(atoi(argv[1]));  // 포트 번호를 문자열에서 정수로 변환 후 네트워크 바이트 순서로 변환

    // 지정된 주소와 포트에 소켓 바인딩
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // 메시지를 처리하기 위한 무한 루프 시작
    while(1){

        clnt_adr_sz = sizeof(clnt_adr);  // 클라이언트 주소 크기 설정

        // 클라이언트로부터 메시지를 수신
        // recvfrom 함수 호출을 통해서 데이터를 전송한 이의 주소정보도 함께 얻는데,
        // 이 주소정보를 이용해서 수신된 데이터를 역으로 재전송
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0,
        (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        // 수신된 메시지의 길이 출력
        printf("%d\n", str_len);  

        // 메시지가 수신되면 클라이언트 수를 출력하고 카운터 증가
        if(str_len != 0)
            printf("Connected client %d \n", connectCounter++);

        // 클라이언트에게 수신된 메시지를 그대로 전송
        sendto(serv_sock, message, str_len, 0,
        (struct sockaddr*)&clnt_adr, clnt_adr_sz);
    }


    // 서버 소켓 닫기
    //그러나 while문을 탈출하는 break문이 없어서 사실상 의미가 없다.
    close(serv_sock);  
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr); 
    fputc('\n', stderr);
    exit(1);
}
