
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 11월 28일 목요일 
// 실습01) 멀티캐스트 Sender 구현 on Linux

#define TTL 64 // 멀티캐스트 패킷의 TTL(Time-to-Live) 값 정의
#define BUF_SIZE 30 // 전송할 버퍼 크기 정의

void error_handling(char *message); // 에러 처리 함수

int main(int argc, char *argv[]){

    int send_sock; // 데이터를 송신하기 위한 소켓
    struct sockaddr_in mul_adr; // 멀티캐스트 그룹의 주소를 저장할 구조체
    int time_live = TTL; // TTL 값
    FILE *fp; // 파일 포인터
    char buf[BUF_SIZE]; // 파일 데이터를 읽어올 버퍼

    // 프로그램 실행 시 인자의 개수를 확인
    if (argc!=3){
    printf("Usage: %s <GroupIP> <PORT>\n", argv[0]);
    exit(1);
    }

    // 송신 소켓 생성 (UDP 사용)
    send_sock-socket(PF_INET, SOCK_DGRAM, 0);
    memset(&mul_adr, e, sizeof(mul_adr));
    mul_adr.sin_family=AF_INET; // 주소 체계 지정 (IPv4)
    mul_adr.sin_addr.s_addr-inet_addr (argv[1]); // 멀티캐스트 IP 주소
    mul_adr.sin_port-htons (atoi(argv[2])); // 멀티캐스트 포트 번호

    // 소켓 옵션 설정 (TTL 값 지정)
    setsockopt(send_sock, IPPROTO_IP,
        IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

    // 전송할 파일 열기
    if((fp=fopen("news.txt", "r")) ==NULL)
        error_handling("fopen() error");
    
    // 파일 내용을 읽어 멀티캐스트 그룹에 데이터 전송
    while(!feof(fp)) /* Broadcasting*/
    {
        fgets(buf, BUF_SIZE, fp); // 파일에서 데이터를 읽어옴

        sendto(send_sock, buf, strlen(buf),
            0, (struct sockaddr*)&mul_adr, sizeof(mul_adr)); // 데이터를 멀티캐스트 그룹으로 전송
        
        sleep(2);// 2초 간격으로 데이터 전송

    }

    // 파일 및 소켓 닫기
    fclose(fp);
    close(send_sock);
    return 0;
}

void error_handling(char *message){
    fputs (message, stderr);
    fputc('\n', stderr);
    exit(1);
}