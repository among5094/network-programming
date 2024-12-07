#include <stdio.h>  // 표준 입출력 라이브러리 포함
#include <stdlib.h>  // 동적 메모리 할당 및 해제를 위한 라이브러리 포함
#include <string.h>  // 문자열 처리 관련 함수 사용을 위한 라이브러리 포함
#include <pthread.h>  // pthread 라이브러리를 포함하여 쓰레드 관련 함수와 자료형 사용 가능
#include <unistd.h>  // sleep() 함수를 사용하기 위한 라이브러리 포함

// 12월 05일 14주차 
// 실습02) 실습01번 코드에서 발생할 수 있는 타이밍 문제(sleep)를 근본적으로 해결하기

// 쓰레드만의 main 함수 별도로 정의(쓰레드는 별도의 실행흐름을 갖기 때문에)
void* thread_main(void *arg); 

int main(int argc, char *argv[]) { 

    pthread_t t_id; // 쓰레드 ID를 저장할 변수
    int thread_param = 5; // 쓰레드에 전달할 인자
    void* thr_ret; // 쓰레드의 반환 값을 저장할 포인터

    // 쓰레드 생성(쓰레드 ID 저장 위치, 쓰레드 속성 (NULL로 기본 속성 사용), 실행할 함수, 함수에 전달할 인자)
    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0) { 
        puts("pthread_create() error"); 
        return -1; 
    }

    // 생성된 쓰레드의 종료를 기다림
    if(pthread_join(t_id, &thr_ret) != 0) { 
        // pthread_join: 쓰레드가 종료될 때까지 대기(대기할 쓰레드 ID, 쓰레드의 반환 값을 저장할 포인터)
        puts("pthread_join() error"); 
        return -1;
    }

    // 쓰레드가 반환한 메시지를 출력
    printf("Thread return message: %s \n", (char*)thr_ret); 
    free(thr_ret); // 동적 할당된 메모리 해제
    return 0; // 프로그램 정상 종료
}

void* thread_main(void *arg) { // 쓰레드에서 실행될 함수 정의
    int i; // 반복문에서 사용할 변수
    int cnt = *((int*)arg); // 전달받은 포인터를 역참조하여 정수 값으로 변환
    char *msg = (char*)malloc(sizeof(char) * 50); // 동적 메모리 할당, 반환할 메시지 저장
    strcpy(msg, "Hello, I'am thread~ \n"); // 반환할 메시지 저장

    for(i = 0; i < cnt; i++) { // 전달받은 횟수(cnt)만큼 반복
        sleep(1); // 1초 대기
        puts("running thread"); // 현재 쓰레드 실행 중임을 알리는 메시지 출력
    }
    return (void*)msg; // 쓰레드가 종료될 때 반환할 메시지 반환
}
