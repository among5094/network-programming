#include <stdio.h>
#include <unistd.h>
// 10월 31일 목요일

int main(int argc, char *argv[]){

    // fork 함수를 호출하여 새로운 프로세스를 생성
    // pid에는 자식 프로세스에서는 0이, 부모 프로세스에서는 자식 프로세스의 PID가 저장됨
    pid_t pid = fork();

    // pid가 0이면 자식 프로세스임을 의미
    if(pid==0){
        puts("Hi I'am a child process"); // 자식 프로세스에서 출력
    }else{
        // 부모 프로세스의 경우 자식 프로세스의 PID를 출력
        printf("Child Process ID: %d \n", pid);
        sleep(30); // 부모 프로세스가 30초 동안 대기(자식 프로세스가 종료되어 좀비 상태로 남을 수 있음)
    }

    // 자식 프로세스인 경우
    if(pid==0)
        puts("End child process"); // 자식 프로세스가 종료될 때 출력
    else
        puts("End parent process"); // 부모 프로세스가 종료될 때 출력
    return 0;
}
/*
자식 프로세스가 먼저 종료되었지만,
부모 프로세스가 sleep(30)으로 대기 중이기 때문에
종료된 자식 프로세스의 종료 상태가 커널에 남아있는 좀비 상태가 발생
---
부모 프로세스가 wait()이나 waitpid()를 통해
자식 프로세스의 종료 상태를 수거하지 않았으므로,
자식 프로세스의 PID (5957)는 커널의 프로세스 테이블에 남아있는 좀비 프로세스
---


*/