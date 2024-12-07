#include <stdio.h>  // 표준 입출력 라이브러리를 포함
#include <pthread.h>  // pthread 라이브러리를 포함하여 쓰레드 관련 함수와 자료형 사용 가능
#include <unistd.h>  // sleep() 함수를 사용하기 위한 라이브러리 포함

// 12월 05일 14주차 
// 실습01) 쓰레드 생성해보기
/*  컴파일 명령어
gcc 1205_ppt01_threadCreate.c -o -lpthread server
./server 5001  */

void* thread_main(void *arg); // 쓰레드만의 main 함수 별도로 정의(쓰레드는 별도의 실행흐름을 갖기 때문에)

int main(int argc, char *argv[]) {

    pthread_t t_id; // 쓰레드 식별자를 저장할 변수 선언
    int thread_param = 5; // 쓰레드에게 전달할 인자로 사용할 정수 값

    // 쓰레드 생성
    // pthread_create 함수로 쓰레드 생성, 실패 시 0이 아닌 값 반환
    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0) { 
        // 생성된 쓰레드의 ID를 저장할 변수의 주소, 쓰레드 속성 (NULL이면 기본 속성), 실행할 함수의 이름, 함수에 전달할 인자

        puts("pthread_create() error"); // 에러 메시지 출력
        return -1; // 프로그램 종료 코드 -1 반환
    }

    sleep(10); // 메인 함수에서 10초 동안 대기, 쓰레드가 실행될 시간을 제공
    puts("end of main"); // 메인 함수 종료를 알리는 메시지 출력

    return 0; // 프로그램 정상 종료
}

// 쓰레드에서 실행될 함수 정의
void* thread_main(void *arg) { 


    int i; // 반복문에서 사용할 변수 선언

    int cnt = *((int*)arg); // 매개변수 arg로 전달된 것은 pthread_create함수의 네 번째 인자
    // 전달받은 포인터를 역참조하여 정수 값으로 변환
    // arg는 void* 형으로 전달되므로 int*로 캐스팅 후 역참조 필요

    // 전달받은 값(cnt)만큼 반복
    for(i = 0; i < cnt; i++) { 
        sleep(1); // 1초 대기
        puts("running thread");
    }
    return NULL; // 쓰레드 종료 시 반환값 (NULL 반환)

}
