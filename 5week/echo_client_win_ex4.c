#include <stdio.h>       // 표준 입출력 함수 사용을 위한 헤더
#include <stdlib.h>      // exit() 함수 사용을 위한 헤더
#include <string.h>      // 문자열 관련 함수 사용을 위한 헤더
#include <winsock2.h>    // 윈도우 소켓 프로그래밍 관련 함수 및 구조체 정의
// 10월 03일 온라인 강의 에코 클라이언트 소켓 버퍼 크기 설정

#define BUF_SIZE 5120 // 사용자가 입력하는 버퍼 size
#define BUF_RECV_SIZE 1024 // 실제 시스템 간에 데이터를 주고받을 수 있는 버퍼 크기 -> 서버에서 안 바꿨으니까

// 오류 발생 시 메시지를 출력하고 프로그램 종료
void ErrorHandling(char *message);

int main(int argc, char *argv[]) {

    WSADATA wsaData;     // 윈도우 소켓 초기화 정보 저장
    SOCKET hSocket;      // 클라이언트 소켓 (서버와의 연결에 사용)
    char message[BUF_SIZE];  // 서버와 주고받을 데이터를 저장할 버퍼
    int str_len, recv_len, recv_cnt; //송신된 메시지의 길이, 수신된 전체 데이터의 길이, 한 번에 받은 데이터의 길이
    SOCKADDR_IN servAdr;  // 서버 주소 정보를 저장할 구조체

    ////////////////////////////////////////////////////////////////////
    int state;
    int snd_buf=512, rcv_buf=512;
    ////////////////////////////////////////////////////////////////////

    // 프로그램 실행 시 IP 주소와 포트 번호를 인자로 받아야 함
    if(argc != 3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    } 

    // 윈도우 소켓 라이브러리 초기화 (버전 2.2 사용)
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    // 클라이언트 소켓 생성 (IPv4, TCP 소켓, 기본 프로토콜)
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");

    ////////////////////////////////////////////////////////////////////
    state=setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
    if(state)
        ErrorHandling("setsockopt() error!");
    ////////////////////////////////////////////////////////////////////

    // 서버 주소 구조체 초기화 및 설정
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;  // 주소 체계는 IPv4 사용
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);  // 입력받은 IP 주소 설정
    servAdr.sin_port = htons(atoi(argv[2]));  // 입력받은 포트 번호 설정

    // 서버에 연결 요청 (클라이언트 소켓을 서버 소켓과 연결)
    if(connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("connect() error!");
    else
        puts("Connected...........");  // 연결 성공 메시지 출력

    // 메시지 송수신 루프
    while(1)
    {
        // 사용자에게 입력을 요청하고 메시지를 읽음
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        // 입력받은 메시지가 "q" 또는 "Q"일 경우 프로그램 종료
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 입력받은 메시지를 서버에 전송
        str_len = send(hSocket, message, strlen(message), 0);


        recv_len = 0; // 수신된 데이터 길이를 0으로 초기화

        // 서버로부터 메시지를 수신 (메시지 전체가 수신될 때까지 반복)
        while (recv_len<str_len) {

            // 서버로부터 데이터를 수신
            recv_cnt=recv(hSocket, &message[recv_len], BUF_RECV_SIZE-1, 0); 
            
            // 데이터를 못받았을 때 오류 시 처리
            if(recv_cnt==-1)
                ErrorHandling("connect() error!");
            
            recv_len += recv_cnt; // 수신된 데이터의 총 길이를 갱신
        } 
        
        message[recv_len]=0; // 수신된 메시지 끝에 널 문자 추가 (문자열 종료)
        printf("Message from server: %s", message); // 서버로부터 받은 메시지 출력

    }
  
    closesocket(hSocket);// 클라이언트 소켓 닫기
    WSACleanup();// 윈도우 소켓 라이브러리 정리
    return 0;
}

void ErrorHandling(char *message){
    fputs(message, stderr); 
    fputc('\n', stderr);     
    exit(1);               
}

