#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

// CH18-4: 쓰레드 동기화
// 뮤텍스 구현 (p413)

#define NUM_THREAD 100 // 생성할 쓰레드 수
void *thread_inc(void *arg); // 증가 작업을 수행할 쓰레드 함수
void *thread_des(void *arg); // 감소 작업을 수행할 쓰레드 함수

long long num = 0; // 전역 변수: 증가 및 감소 작업 대상
pthread_mutex_t mutex; // 뮤텍스 변수

int main(int argc, char *argv[]) {

    pthread_t thread_id[NUM_THREAD]; // 쓰레드 ID를 저장할 배열
    int i;

    pthread_mutex_init(&mutex, NULL); // 뮤텍스 초기화
    
    // NUM_THREAD 개수만큼 쓰레드 생성
    for(i = 0; i < NUM_THREAD; i++) {

       for(i = 0; i < NUM_THREAD; i++) {
            if (i % 2) // i가 홀수일 경우 (1, 3, 5, ...) 증가 작업 실행
                pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
            else // i가 짝수일 경우 (0, 2, 4, ...) 감소 작업 실행
                pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
        }
    }

    // 생성된 쓰레드가 종료될 때까지 대기
    for(i = 0; i < NUM_THREAD; i++)
        pthread_join(thread_id[i], NULL);

    // 최종 결과 출력
    printf("result: %lld \n", num);

    pthread_mutex_destroy(&mutex); // 뮤텍스 해제 및 자원 소멸
    return 0;
}

// 증가 작업을 수행하는 쓰레드 함수
void *thread_inc(void *arg) {
    int i;

    // 뮤텍스 잠금으로 임계영역 보호
    pthread_mutex_lock(&mutex);
    for (i = 0; i < 50000000; i++)
        num += 1; // <임계영역!!> 전역 변수 num 값을 증가
    pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제

    return NULL; // 쓰레드 종료
}

// 감소 작업을 수행하는 쓰레드 함수
void *thread_des(void *arg) {
    int i;

    for (i = 0; i < 50000000; i++) {
        pthread_mutex_lock(&mutex); // 뮤텍스 잠금으로 임계영역 보호
        num -= 1; // <임계영역!!> 전역 변수 num 값을 감소
        pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
    }

    return NULL; // 쓰레드 종료
}
