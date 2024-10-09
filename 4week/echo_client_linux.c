#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  // 오타 수정: winsock2.h -> arpa/inet.h (리눅스용 헤더 파일)
#include <unistd.h>  // 오타 수정: closesocket() 대신 close()를 사용하기 위한 헤더 파일
// 9월 26일 목요일
// 리눅스 호나경에서 동작하는 "에코 클라이언트" 코드

#define BUF_SIZE 1024
void ErrorHandling(char *message);

int main(int argc, char *argv[]){

    int hSocket;  // 오타 수정: SOCKET -> int (리눅스에서는 정수형 사용)
    char message[BUF_SIZE];
    int str_len;  // 오타 수정: strlen 변수 이름을 str_len으로 변경 (표준 함수 strlen과 충돌 방지)
    struct sockaddr_in servAdr;  // 오타 수정: SOCKADDR_IN -> struct sockaddr_in (리눅스에서 사용되는 형식)

    if(argc!=3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 소켓 초기화
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(hSocket == -1)  // 오타 수정: INVALID_SOCKET 대신 리눅스에서 소켓 실패는 -1을 반환
        ErrorHandling("socket() error");

    // 서버 주소 정보 설정
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(atoi(argv[2]));

    // 서버에 연결
    if(connect(hSocket, (struct sockaddr*)&servAdr, sizeof(servAdr)) == -1)  // 오타 수정: connet() -> connect()
        ErrorHandling("connect() error!");
    else
        puts("Connected...........");

    // 메시지 송수신
    while(1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        send(hSocket, message, strlen(message), 0);  // 변수 이름 수정: strlen 함수와 충돌 방지 위해 str_len 대신 strlen 사용
        str_len = recv(hSocket, message, BUF_SIZE - 1, 0);  // 오타 수정: strlen -> str_len 변수 이름 수정
        message[str_len] = 0;  // 문자열 끝에 NULL 추가
        printf("Message from server: %s", message);
    }

    close(hSocket);  // 오타 수정: closesocket() -> close() (리눅스에서 소켓 닫는 함수)
    return 0;
}

void ErrorHandling(char *message){
    fputs(message, stderr); 
    fputc('\n', stderr);
    exit(1);
}

