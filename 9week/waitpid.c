#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// 10월 31일 목요일
// 실습03 - 의도적으로 생성한 좀비 프로세스를 처리하는 함수 사용: waitpid()

int main(int argc, char *argv[]) {

    int status; // 자식 프로세스의 종료 상태를 저장할 변수
    pid_t pid = fork(); // 새로운 프로세스를 생성

    // pid가 0이면 자식 프로세스임을 의미
    if (pid == 0) {
        // 자식 프로세스: 15초 동안 대기 후 종료
        sleep(15); // 자식 프로세스가 15초 동안 대기
        return 24; // 자식 프로세스가 종료될 때 종료 코드를 24로 반환
    } else {
        // 부모 프로세스
        // waitpid 함수 사용: 자식 프로세스가 종료될 때까지 주기적으로 확인

        // 자식 프로세스의 종료 상태를 확인할 때까지 루프를 돌림
        while (!waitpid(-1, &status, WNOHANG)) {
            sleep(3); // 3초 동안 대기
            puts("sleep 3sec."); // 3초 동안 대기했다는 메시지 출력
        }

        // 자식 프로세스가 정상 종료되었는지 확인
        if (WIFEXITED(status))
            printf("Child send %d \n", WEXITSTATUS(status)); // 자식 프로세스가 반환한 종료 코드 출력
    }

    return 0;
}

/*
waitpid()를 사용하여
자식 프로세스가 종료될 때 부모 프로세스가 자식의 종료 상태를 수거하므로
좀비 프로세스가 발생하지 않음!
*/