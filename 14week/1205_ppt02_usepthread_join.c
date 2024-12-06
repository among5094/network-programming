#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// 12월 05일 14주차 
// 실습02) 실습01번 코드에서 발생할 수 있는 타이밍 문제(sleep)를 근본적으로 해결하기 


void* thread_main(void *arg);

int main(int argc, char *argv[]) {

    pthread_t t_id;
    int thread_param=5;
    void* thr_ret;

    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)!=0){
        puts("pthread_create() error");
        return -1;

    };

    if(pthread_join(t_id, &thr_ret)!=0){
        puts("pthread_join() error");
        return -1;
    }

    printf("Thread return message: %s \n", (char*)thr_ret);
    free(thr_ret);
    return 0;
}

void* thread_main(void *arg){
    int i;
    int cnt = *((int*)arg); // 전달받은 값을 역참조
    char * msg=(char*)malloc(sizeof(char)*50);
    strcpy(msg, "Hello, I'am thread~ \n");

    for(i=0; i<cnt; i++){
        sleep(1);
        puts("running thread");
    }
    return (void*)msg;
}


