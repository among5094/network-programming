#include <stdio.h>   
#include <unistd.h>  
#define BUF_SIZE 30 

// 11월 07일 
// 실습02 - 1개의 파이프로 양방향 통신하기

int main(int argc, char *argv[]) {

    int fds[2]; // 파이프 파일 디스크립터 배열: fds[0]은 읽기용, fds[1]은 쓰기용
    char str1[] = "Who are you?"; // 자식 프로세스가 보낼 문자열
    char str2[] = "Thank you for your message"; // 부모 프로세스가 보낼 문자열
    char buf[BUF_SIZE]; // 부모 프로세스가 읽은 데이터를 저장할 버퍼
    pid_t pid; // 프로세스 ID를 저장할 변수

    pipe(fds); // 파이프 생성, fds에 파이프의 읽기와 쓰기 파일 디스크립터 할당
    pid = fork(); // 새로운 프로세스 생성 (자식 프로세스 생성)

  if (pid == 0) { // pid가 0이면 자식 프로세스 실행
        
        // 자식 프로세스가 파이프의 쓰기 디스크립터(fds[1])를 통해 str1을 부모에게 보냄
        write(fds[1], str1, sizeof(str1)); 

        // 부모 프로세스가 메시지를 읽고 응답할 시간을 주기 위해 2초 대기
        sleep(2); 

        // 파이프의 읽기 디스크립터(fds[0])를 통해 부모가 보낸 메시지를 읽어 buf에 저장
        read(fds[0], buf, BUF_SIZE); 

        // 읽어들인 데이터를 화면에 출력
        printf("*Child proc output: %s \n", buf); 
    } else { 
        // pid가 0이 아니면 부모 프로세스 실행

        // 파이프의 읽기 디스크립터(fds[0])를 통해 자식 프로세스가 보낸 메시지를 읽어 buf에 저장
        read(fds[0], buf, BUF_SIZE); 

        // 읽어들인 데이터를 화면에 출력
        printf("*Parent proc output: %s \n", buf);  

        // 파이프의 쓰기 디스크립터(fds[1])를 통해 str2 메시지를 자식 프로세스에 보냄
        write(fds[1], str2, sizeof(str2)); 

        // 자식 프로세스가 메시지를 읽을 시간을 주기 위해 3초 대기
        sleep(3); 
    }

    return 0;
}
