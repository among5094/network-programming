#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// CH18-4: 쓰레드 동기화
// 세마포어 구현 (p416~419)

// 함수 선언
void* read(void *arg); // 숫자를 입력받는 쓰레드 함수
void* accu(void *arg); // 입력받은 숫자를 누적 계산하는 쓰레드 함수

// 세마포어 변수
static sem_t sem_one; // 입력 완료를 알리는 세마포어
static sem_t sem_two; // 계산 완료를 알리는 세마포어

// 공유 변수
static int num;

int main(int argc, char *argv[]) {

    pthread_t id_t1, id_t2; // 쓰레드 ID
    sem_init(&sem_one, 0, 0); // sem_one 초기화: 초기값 0
    sem_init(&sem_two, 0, 1); // sem_two 초기화: 초기값 1

    // 두 개의 쓰레드 생성
    pthread_create(&id_t1, NULL, read, NULL); // 숫자 입력을 처리하는 쓰레드 생성
    pthread_create(&id_t2, NULL, accu, NULL); // 누적 계산을 처리하는 쓰레드 생성

    // 쓰레드 종료 대기
    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);

    // 세마포어 자원 해제
    sem_destroy(&sem_one);
    sem_destroy(&sem_two);

    return 0; // 프로그램 종료
}

// 숫자 입력을 처리하는 쓰레드 함수
void* read(void *arg) {
    int i;
    for (i = 0; i < 5; i++) { // 총 5회 숫자 입력

        fputs("Input num: ", stdout); // 숫자 입력 요청 메시지 출력

        sem_wait(&sem_two); // sem_two가 1일 때까지 대기 (계산 작업 완료 신호 대기)
        scanf("%d", &num); // 사용자로부터 숫자 입력
        sem_post(&sem_one); // sem_one을 1로 설정하여 입력 완료 신호 전달
    }

    return NULL; // 쓰레드 정상 종료
}

// 입력받은 숫자를 누적 계산하는 쓰레드 함수
void* accu(void *arg) {
    
    int sum = 0, i;
    for (i = 0; i < 5; i++) { // 총 5회 누적 계산

        sem_wait(&sem_one); // sem_one이 1일 때까지 대기 (입력 완료 신호 대기)
        sum += num; // 입력받은 숫자를 합산
        sem_post(&sem_two); // sem_two를 1로 설정하여 계산 완료 신호 전달
    }
    printf("Result: %d \n", sum); // 누적 합산 결과 출력
    return NULL; // 쓰레드 정상 종료
}
