#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// 10월 31일 목요일
// 실습04 - sigaction() 함수를 통해 자식 프로세스 종료 시 좀비 프로세스를 처리하는 함수 사용

// SIGCHLD 시그널을 처리하기 위한 핸들러 함수 선언
void read_childproc(int sig);

int main(int argc, char *argv[]) {

    pid_t pid;
    struct sigaction act; // sigaction 구조체 선언

    // sigaction 구조체 설정
    act.sa_handler = read_childproc; // SIGCHLD 시그널 발생 시 호출할 핸들러 함수 설정
    act.sa_flags = 0; // 추가 옵션을 설정하지 않음
    sigemptyset(&act.sa_mask); // 시그널 핸들러 실행 중 블록할 시그널 집합 초기화

    // SIGCHLD 시그널 발생 시 read_childproc 함수를 호출하도록 등록
    sigaction(SIGCHLD, &act, 0);

    // 첫 번째 자식 프로세스 생성
    pid = fork();
    if (pid == 0) { // 자식 프로세스 코드
        puts("Hi! I'm child process"); // 자식 프로세스 실행 시 메시지 출력
        sleep(10); // 10초 대기
        return 12; // 종료 코드 12로 종료
    } else { // 부모 프로세스 코드
        printf("Child proc id: %d \n", pid); // 첫 번째 자식 프로세스 ID 출력

        // 두 번째 자식 프로세스 생성
        pid = fork();
        if (pid == 0) { // 두 번째 자식 프로세스 코드
            puts("Hi! I'm child process"); // 두 번째 자식 프로세스 실행 시 메시지 출력
            sleep(15); // 15초 대기
            exit(24); // 종료 코드 24로 종료
        } else { // 부모 프로세스 코드
            int i;
            printf("Child proc id: %d \n", pid); // 두 번째 자식 프로세스 ID 출력
            for (i = 0; i < 5; i++) { // 부모 프로세스가 5초마다 '.'을 출력하며 대기
                printf(".");
                sleep(5);
            }
        }
    }

    return 0;
}

// 자식 프로세스가 종료될 때 호출되는 시그널 핸들러 함수
void read_childproc(int sig) {
    int status;
    // 종료된 자식 프로세스의 PID와 종료 상태를 확인
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) // 자식 프로세스가 정상적으로 종료된 경우
        printf("Removed proc id: %d \n", id); // 종료된 자식 프로세스의 PID 출력
        printf("Child send %d \n", WEXITSTATUS(status)); // 자식 프로세스가 반환한 종료 코드 출력
}

/*
좀비 프로세스가 발생하지 않은 상태이다.
자식 프로세스가 종료될 때
sigaction()을 사용한 SIGCHLD 시그널 핸들링을 통해
좀비 프로세스를 적절히 제거했기 때문에 
*/