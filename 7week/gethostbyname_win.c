#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
// 10월 17일 목요일 - DNS/IP 주소 간 변환
// 도메인 이름을 입력받아 해당 도메인의 IP 주소를 출력하는 기능을 구현하기

void ErrorHandling(char *message);

int main(int argc, char *argv[]){

    WSADATA wsaData; // 윈속 초기화 정보 저장 구조체
    int i; // 반복문에서 사용할 변수
    struct hostent *host; // 호스트 정보를 저장할 구조체 포인터

    // 입력된 인자 개수를 확인하여 올바른 사용법이 아니라면 프로그램 종료
    if(argc!=2){
        printf("Usage: %s <addr>\n", argv[0]);
        exit(1);
    }

    // 윈속 초기화: 성공 시 0 반환, 실패 시 오류 처리  
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)
        ErrorHandling("WSAStartup() error!"); // WSAStartup 실패 시 에러 처리

    // gethostbyname 사용: 도메인 이름을 통해 호스트 정보를 얻음
    host=gethostbyname(argv[1]); // argv[1]은 프로그램에 입력된 도메인 이름
    if(!host==INVALID_SOCKET) // gethostbyname이 실패한 경우 host는 NULL이므로 이 조건으로 오류 확인
        ErrorHandling("gethost... error!"); // 오류 시 에러 메시지 출력 후 종료

    // 호스트의 공식 이름 출력
    printf("Official name: %s \n", host->h_name);

    // 호스트의 별칭(aliases) 출력, h_aliases는 별칭 문자열 배열
    for(i=0; host->h_aliases[i]; i++)
        printf("Aliases %d: %s \n", i+1,  host->h_aliases[i]);

    // 주소 타입 출력 (IPv4(AF_INET) 또는 IPv6(AF_INET6))
    printf("Address type: %s \n",
        (host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");

    // 호스트의 IP 주소 목록 출력, h_addr_list는 IP 주소의 리스트를 가리키는 포인터 배열
    for(i=0; host->h_addr_list[i]; i++)
        printf("IP addr %d: %s \n", i+1,
                    inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
        // h_addr_list[i]는 IP 주소를 가리키고, inet_ntoa는 네트워크 바이트 순서의 주소를 점 표기법으로 변환함
    
    WSACleanup(); // 윈속 종료 처리 (사용된 소켓 자원 해제)
    return 0;

}

void ErrorHandling(char *message){
    fputs(message, stderr); 
    fputc('\n', stderr);
    exit(1);
}
/* 그냥 궁금한거
WSAStartup와 WSACleanup의 각각의 약자
WSA: Windows Sockets API
WSA: Windows Sockets API
*/