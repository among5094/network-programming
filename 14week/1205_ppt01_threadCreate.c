#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 12월 05일 14주차 
// 실습01) 쓰레드 생성해보기

void* thread_main(void *arg);

int main(int argc, char *argv[]) {

    pthread_t t_id;
    int thread_param=5;

    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)!=0){
        puts("pthread_create() error");
        return -1;

    };
    sleep(10); 
    puts("end of main");

    return 0;
}

void* thread_main(void *arg){
    int i;
    int cnt = *((int*)arg); // 전달받은 값을 역참조
    for(i=0; i<cnt; i++){
        sleep(1);
        puts("running thread");
    }
    return NULL;
}