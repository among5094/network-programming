#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
//10월 17일 목요일 - DNS/IP 주소 간 변환( gethostbyaddr() 함수 사용 )
// IP 주소를 입력받아 해당 IP 주소의 도메인 이름을 찾아주는 기능을 구현한 코드

void ErrorHandling(char *message);

int main(int argc, char *argv[]){

    WSADATA wsaData; // 윈속 초기화 정보 저장 구조체
    int i; // 반복문에서 사용할 변수
    struct hostent *host; // 호스트 정보를 저장할 구조체 포인터
    SOCKADDR_IN addr; // IP 주소 정보를 저장할 구조체

    // 프로그램 사용법을 출력하고, 입력 인자가 2개가 아니면 종료
    if(argc!=2){
        printf("Usage: %s <addr>\n", argv[0]);
        exit(1);
    }

    // 윈속 초기화: 성공 시 0 반환, 실패 시 오류 처리
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)
        ErrorHandling("WSAStartup() error!");  // WSAStartup 실패 시 에러 처리

    // gethostname과는 다른 부분 -> gethostbyaddr 사용하기
    // gethostbyaddr을 사용하기 위해 IP 주소를 네트워크 바이트 순서로 변환
    memset(&addr, 0, sizeof(addr)); // addr 구조체를 0으로 초기화
    addr.sin_addr.s_addr=inet_addr(argv[1]); // 입력받은 IP 주소를 네트워크 바이트 순서로 변환
    host=gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET); // IP 주소를 통해 도메인 이름을 가져오는 함수
    if(!host) // host가 NULL이면 gethostbyaddr 실패
        ErrorHandling("gethost... error!");

    // 호스트의 공식 이름 출력
    printf("Official name: %s \n", host->h_name);

    // 호스트의 별칭(aliases) 출력, h_aliases는 별칭 문자열 배열
    for(i=0; host->h_aliases[i]; i++)
        printf("Aliases %d: %s \n", i+1,  host->h_aliases[i]);

    // 주소 타입 출력 (IPv4(AF_INET) or IPv6(AF_INET6))  
    printf("Address type: %s \n",
        (host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");

    /*  여기는 gethostbyname과는 다르게 없는 부분:
        gethostbyname은 도메인 이름을 사용하여 여러 IP 주소를 반환하지만,
        gethostbyaddr은 주어진 IP 주소에 대한 도메인 이름만 반환함 
    
    for(i=0; host->h_addr_list[i]; i++)
        printf("IP addr %d: %s \n", i+1,
                    inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

    */

    WSACleanup(); // 윈속 종료 처리 (사용된 소켓 자원 해제)
    return 0; 

}

void ErrorHandling(char *message){
    fputs(message, stderr); //  에러 메시지 출력
    fputc('\n', stderr);
    exit(1);
}
