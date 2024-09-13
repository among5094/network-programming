#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>  // Windows에서 소켓 프로그래밍을 위한 헤더 파일

void ErrorHandling(char* message);  // 오류 처리를 위한 함수 선언

int main(int argc, char* argv[]) {

    WSADATA wsaData;  // 윈속 초기화를 위한 구조체
    SOCKET hSocket;   // 소켓 파일 디스크립터
    struct sockaddr_in servAddr;  // 서버 주소 정보를 저장할 구조체

    char message[30];  // 서버로부터 수신할 데이터를 저장할 버퍼
    int recv_len;      // 수신한 데이터의 길이를 저장하는 변수

    // 사용법이 잘못된 경우, 즉 IP 주소와 포트 번호가 입력되지 않으면 프로그램 종료
    if (argc != 3) {
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    // 1. 윈속(Windows Sockets) 초기화
    // MAKEWORD(2, 2)는 Winsock 2.2 버전을 요청
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    // 2. 소켓 생성
    // PF_INET: IPv4 주소 체계, SOCK_STREAM: TCP 소켓 생성
    hSocket = socket(PF_INET, SOCK_STREAM, 0);  
    if (hSocket == INVALID_SOCKET)  // 소켓 생성 실패 시
        ErrorHandling("socket() error!");

    // 3. 서버 주소 설정
    memset(&servAddr, 0, sizeof(servAddr));  // servAddr 구조체를 0으로 초기화
    servAddr.sin_family = AF_INET;  // IPv4 주소 체계 사용
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);  // 입력받은 IP 주소를 네트워크 바이트 순서로 변환
    servAddr.sin_port = htons(atoi(argv[2]));  // 입력받은 포트 번호를 네트워크 바이트 순서로 변환

    // 4. 서버에 연결 요청
    // 서버의 IP 주소와 포트에 연결
    if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)  // 연결 실패 시
        ErrorHandling("connect() error!");

    // 5. 서버로부터 데이터 수신
    // 서버로부터 데이터를 수신하여 message 배열에 저장, recv_len에 수신한 데이터 길이 저장
    recv_len = recv(hSocket, message, sizeof(message) - 1, 0);  
    if (recv_len == -1)  // 수신 실패 시
        ErrorHandling("recv() error!");

    // 6. 수신한 메시지의 끝에 NULL 문자 추가하여 문자열로 처리
    message[recv_len] = '\0';  
    printf("Message from server: %s \n", message);  // 서버로부터 수신한 메시지 출력

    // 7. 소켓 종료 및 윈속 종료
    closesocket(hSocket);  // 소켓을 닫음
    WSACleanup();  // Winsock 종료
    return 0;
}

// 오류 메시지를 출력하고 프로그램을 종료하는 함수
void ErrorHandling(char *message) {  
    fputs(message, stderr);  // 표준 오류 출력 스트림에 오류 메시지 출력
    fputc('\n', stderr);     // 개행 문자 출력
    exit(1);  // 프로그램 강제 종료
}
