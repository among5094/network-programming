#include <stdio.h>
#include <sys/uio.h>
#define BUF_SIZE 100

// 11월 21일 목요일 - 실습03) Linux에서 readv 코드 작성 후 실행

int main(int argc, char *argv[]) {
    struct iovec vec[2]; // iovec 배열 생성: 여러 메모리 버퍼를 정의하는 구조체
    char buf1[BUF_SIZE] = {0,}; // 첫 번째 버퍼 초기화
    char buf2[BUF_SIZE] = {0,}; // 두 번째 버퍼 초기화
    int str_len; // 읽어들인 바이트 수를 저장할 변수

    vec[0].iov_base = buf1; // 첫 번째 버퍼의 시작 주소
    vec[0].iov_len = 5; // 첫 번째 버퍼의 크기 (5바이트 읽기)

    vec[1].iov_base = buf2; // 두 번째 버퍼의 시작 주소
    vec[1].iov_len = BUF_SIZE; // 두 번째 버퍼의 크기 (최대 BUF_SIZE 바이트 읽기)

    str_len = readv(0, vec, 2); // readv를 통해 여러 버퍼로 한 번에 읽기
    printf("Read bytes: %d \n", str_len); // 읽어들인 바이트 수 출력
    printf("First message: %s \n", buf1); // 첫 번째 버퍼의 내용 출력
    printf("Second message: %s \n", buf2); // 두 번째 버퍼의 내용 출력

    return 0; // 프로그램 종료
}
