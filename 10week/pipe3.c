#include <stdio.h>   
#include <unistd.h>  
#define BUF_SIZE 30 

// 11월 07일 (목)
// 실습03 - 2개의 파이프로 양방향 통신하기

int main(int argc, char *argv[]) {

    int fds1[2], fds2[2]; // 두 개의 파이프 생성
    // fds1: 부모 → 자식 데이터 전송
    // fds2: 자식 → 부모 데이터 전송
    char str1[] = "Who are you?"; // 자식 프로세스가 부모로부터 받을 메시지
    char str2[] = "Thank you for your message"; // 부모 프로세스가 자식으로부터 받을 메시지
    char buf[BUF_SIZE]; // 부모 프로세스가 읽은 데이터를 저장할 버퍼
    pid_t pid; // fork()의 반환값을 저장할 변수 (자식 프로세스인지 부모 프로세스인지 확인)


    // 두 개의 파이프 생성
    pipe(fds1); // 파이프 fds1: 부모가 데이터를 쓰고, 자식이 데이터를 읽음
    pipe(fds2); // 파이프 fds2: 자식이 데이터를 쓰고, 부모가 데이터를 읽음
    
    // fork()를 통해 자식 프로세스 생성
    pid=fork(); 

    // 자식프로세스일 때? 
    if(pid==0){
        // 자식 프로세스
        // fds1[1]에 쓰기: 부모에게 데이터를 보냄
        write(fds1[1], str1, sizeof(str1));

        // fds2[0]에서 읽기: 부모가 보낸 데이터를 읽음
        read(fds2[0], buf, BUF_SIZE);

         // 부모로부터 받은 데이터를 출력
        printf("Child proc output: %s \n", buf);
    }else{
        // 부모 프로세스
        // fds1[0]에서 읽기: 자식이 보낸 데이터를 읽음
        read(fds1[0], buf, BUF_SIZE); 

        // 자식으로부터 받은 데이터를 출력
        printf("Parent proc output: %s \n", buf); 

        // fds2[1]에 쓰기: 자식에게 데이터를 보냄
        write(fds2[1], str2, sizeof(str2)); 

        // 자식 프로세스가 데이터를 읽을 시간을 주기 위해 대기
        sleep(3); 
    }

    return 0;
}

/*
부모와 자식 프로세스는 독립적으로 데이터를 주고받을 수 있다.
프로세스 간 데이터 흐름이 명확하고, 구조적으로 효율적인 방식이다. 
*/