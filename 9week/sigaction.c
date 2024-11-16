#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// 10월 31일 목요일
// 실습4번 시그널 핸들링 예제

// 자식 프로세스의 종료를 처리하는 시그널 핸들러 함수
void read_childproc(int sig);

int main(int argc, char *argv[]) {

    pid_t pid;
    struct sigaction act;

    // 시그널 핸들러를 설정
    act.sa_handler = read_childproc; // SIGCHLD 시그널이 오면 read_childproc 함수 호출
    act.sa_flags = 0; // 추가 플래그 설정하지 않음
    sigemptyset(&act.sa_mask); // 시그널 마스크 초기화

    sigaction(SIGCHLD, &act, 0); // SIGCHLD 시그널 발생 시 read_childproc 함수가 호출되도록 설정

    pid=fork(); // 첫 번째 자식 프로세스 생성

    if(pid==0){ // 자식 프로세스일 때 실행
        puts("Hi! I'm child process"); // 자식 프로세스임을 알리는 메시지
        sleep(10);
        return 12;
    }else{ // 부모 프로세스일 때 실행

        printf("Child proc id: %d \n", pid); // 첫 번째 자식 프로세스 ID 출력
        pid=fork(); // 두 번째 자식 프로세스 생성

        //// 두 번째 자식 프로세스일 때 실행
        if(pid==0){
            puts("Hi! I'm child process");
            sleep(15);
            return 24;
        }else{ // 부모 프로세스일 때 실행
            int i;
            printf("Child proc id: %d \n", pid); // 두 번째 자식 프로세스 ID 출력
            for(i=0; i<5; i++){
                printf(".");
                sleep(5);
            }
        }
    }
    return 0;
}

// 자식 프로세스 종료 시 호출되는 시그널 핸들러 함수
void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG); // 종료된 자식 프로세스 ID와 상태 정보를 가져옴

    // 자식 프로세스가 정상 종료되었는지 확인
    if(WIFEXITED(status)){
        printf("Removed proc id: %d \n", id); // 종료된 자식 프로세스 ID 출력
        printf("Child send: %d \n", WIFEXITED(status)); // 자식 프로세스가 반환한 값 출력
    }
}