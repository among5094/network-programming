#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// 12월 05일 14주차 
// 실습03) 워커 쓰레드 모델 구현 on Linux

// 워커 쓰레드에서 사용할 함수 선언
void* thread_summation(void *arg);

// 전역 변수: 합계 저장
int sum=0;

int main(int argc, char *argv[]) {
    pthread_t id_t1, id_t2; // 쓰레드 ID 선언
    int range1[]={1, 1000000}; // 첫 번째 쓰레드의 작업 범위
    int range2[]={1000001, 2000000}; // 두 번째 쓰레드의 작업 범위 (수정: 이름 변경)

    // 두 개의 쓰레드 생성
    pthread_create(&id_t1, NULL, thread_summation, (void*)range1); // 첫 번째 쓰레드 생성
    pthread_create(&id_t2, NULL, thread_summation, (void*)range2); // 두 번째 쓰레드 생성 (수정: range2 사용)

    // 쓰레드 작업이 끝날 때까지 대기
    pthread_join(id_t1, NULL); // 첫 번째 쓰레드 종료 대기
    pthread_join(id_t2, NULL); // 두 번째 쓰레드 종료 대기

    // 최종 결과 출력
    printf("result: %d \n", sum);
    return 0;
}

void* thread_summation(void *arg){
    int start=((int*)arg)[0]; // 작업 시작 범위
    int end=((int*)arg)[1]; // 작업 종료 범위

    if(start>1000000){ // 시작 값이 1000000보다 크면 sum에 더하기
        while(start<=end){
            sum+=1; // 수정: 선언되지 않은 변수 `l`을 상수 `1`로 대체
            start++;
        }
    }else if(start<1000000){ // 시작 값이 1000000보다 작으면 sum에서 빼기
        while(start<=end){
            sum-=1; // 수정: 선언되지 않은 변수 `l`을 상수 `1`로 대체
            start++;
        }
    }else{
        printf("Error\n"); // 잘못된 범위일 경우 에러 메시지 출력
        return 0; // 쓰레드 종료
    }
    return NULL; // 쓰레드 정상 종료
}
