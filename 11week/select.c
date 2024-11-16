#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100 // 버퍼 크기 정의

// 11월 14일 목
// 실습01 - 1. select() 함수 사용

int main(int argc, char *argv[]){

    fd_set reads, temps; // 파일 디스크립터 집합 선언
    int result, str_len; // select 반환값 및 입력 길이를 저장할 변수
    char buf[BUF_SIZE]; // 콘솔로부터 읽은 데이터를 저장할 버퍼
    struct timeval timeout; // select 함수의 타임아웃 설정 구조체

    FD_ZERO(&reads); // 모든 파일 디스크립터 집합의 비트를 0으로 초기화
    FD_SET(0, &reads); // 표준 입력(파일 디스크립터 0)을 감시 대상으로 설정

    while(1){
        temps=reads; // 원본 `reads`를 복사하여 사용 (select 호출 시 원본이 변경됨)
        timeout.tv_sec=5; // timeval의 구조체 멤버인 tv_sec(초 단위 정보)를 5초로 지정

        if(result==-1){ // 오류 발생 시 -1 (select 함수 호출 실패 시)
            puts("select() error!");
            break;
        }else if(result==0){ // 타임 아웃에 의한 반환 시 0
            puts("time-out!");
        }else{  // 감시 중인 파일 디스크립터에서 이벤트가 발생한 경우
            str_len=read(0,buf,BUF_SIZE); // 표준 입력에서 데이터를 읽어 buf에 저장
            buf[str_len]=0; // 문자열의 끝에 NULL 문자 추가
            printf("message from console: %s", buf); // 읽은 데이터를 출력
        }
    }
    return 0;
}