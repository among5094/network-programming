#include <stdio.h>   
#include <unistd.h>  
#define BUF_SIZE 30 

// 11월 07일 
// 실습01 - 파이프 생성 후 데이터 전송

int main(int argc, char *argv[]) {

    int fds[2]; // 파이프 파일 디스크립터 배열: fds[0]은 읽기용, fds[1]은 쓰기용
    char str[] = "Who are you?"; // 자식 프로세스가 쓸 문자열
    char buf[BUF_SIZE]; // 부모 프로세스가 읽은 데이터를 저장할 버퍼
    pid_t pid;// 프로세스 ID를 저장할 변수

    pipe(fds); // 파이프 생성, fds에 파이프의 읽기와 쓰기 파일 디스크립터 할당
    pid = fork(); // 새로운 프로세스 생성 (자식 프로세스 생성)


    if (pid == 0) { // pid가 0이면 자식 프로세스
        printf("**Ready To Write**\n"); // 쓰기 준비 완료 메시지 출력 (2번)
        sleep(5); // 5초 대기 후 데이터 쓰기 (쓰기를 지연시켜 보기 위한 의도적 대기)
        write(fds[1], str, sizeof(str)); // 파이프에 str 문자열 쓰기
        printf("**Write Finished**\n"); // 쓰기 완료 메시지 출력

    } else { // pid가 0이 아니면 부모 프로세스

        printf("**Ready To Read**\n"); // 읽기 준비 완료 메시지 출력 (1번)
        read(fds[0], buf, BUF_SIZE); // 파이프에서 데이터를 읽어 buf에 저장
        puts(buf); // 읽은 데이터를 콘솔에 출력 (3번)
    }

    return 0;
}
/*
1. 부모 프로세스는 read() 함수 호출 시 블로킹 상태로 대기
2. 자식 프로세스가 5초 대기 후 write()를 호출하여 데이터를 작성
3. 데이터가 쓰여지면 부모 프로세스의 read()가 해제되어 데이터를 읽고 출력(-> buf부분)

*/