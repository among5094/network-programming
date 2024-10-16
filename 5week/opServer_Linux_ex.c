#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4

void error_handling (char *message);
int calculate(int opnum, int opnds [], char oprator);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock; char opinfo[BUF_SIZE];
    int result, opnd_cnt, i; int recv_cnt, recv_len;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    
    if(argc!=2){
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    // 서버 소켓 생성 (IPv4, TCP 소켓, 기본 프로토콜)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    // 서버 주소 구조체 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;  // 주소 체계는 IPv4 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 서버의 IP는 자동으로 할당
    serv_adr.sin_port = htons(atoi(argv[1]));  // 입력받은 포트를 네트워크 바이트 순서로 변환

    // 서버 소켓과 서버 주소 구조체를 바인딩 (소켓과 IP, 포트를 연결)
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // 서버가 연결 요청을 대기할 수 있게 설정 (최대 5개의 연결 요청을 대기)
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // 클라이언트 주소 크기 설정
    clnt_adr_sz = sizeof(clnt_adr);

    // 5번의 클라이언트 연결을 처리
    for(i = 0; i < 5; i++){
        opnd_cnt = 0;  // 피연산자 개수를 초기화

        // 클라이언트 연결을 수락 (클라이언트와의 연결을 허용)
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1)
            error_handling("accept() error");  // 연결 수락 오류 시 처리

        // 클라이언트로부터 피연산자의 개수를 읽음 (1바이트로 수신)
        read(clnt_sock, &opnd_cnt, 1);

        recv_len = 0;  // 수신한 데이터의 총 길이를 0으로 초기화

        // 피연산자의 개수와 연산자까지 모두 수신할 때까지 반복하여 데이터를 수신
        while((opnd_cnt * OPSZ + 1) > recv_len) {
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);  // 데이터를 수신
            recv_len += recv_cnt;  // 수신된 데이터의 총 길이를 갱신
        }

        // 수신한 데이터를 기반으로 계산 수행 (피연산자는 opinfo 배열에 있음)
        result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);

        // 계산 결과를 클라이언트로 전송
        write(clnt_sock, (char*)&result, sizeof(result));

        // 클라이언트 소켓 닫기
        close(clnt_sock);
    }
    
    close(serv_sock); // 서버 소켓을 닫음 (모든 클라이언트와의 통신이 끝난 후 서버 소켓 종료)
    return 0;
}

// 오류 발생 시 에러 메시지를 출력하고 프로그램 종료하는 함수
void error_handling(char *message){
    fputs(message, stderr);  // 표준 오류 출력에 메시지를 출력
    fputc('\n', stderr);     // 줄바꿈 문자 추가
    exit(1);                 // 프로그램 종료
}

int calculate(int opnum, int opnds [], char op)
{
    int result=opnds[0], i;
    switch(op)
    {
    case '+':
        for (i=1; i<opnum; i++) result+=opnds[i]; break;
    case '-':
        for(i=1; i<opnum; i++) result-=opnds[i]; break;
    case '*':
        for(i=1; i<opnum; i++) result*=opnds[i]; break;
    }
    return result;

}
