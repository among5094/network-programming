#include <stdio.h>
#include <sys/uio.h>

// 11월 21일 목요일 - 실습02) Linux에서 writev 코드 작성 후 실행

int main(int argc, char *argv[]) {
    struct iovec vec[2]; // iovec 배열 생성: 여러 메모리 버퍼를 정의하는 구조체
    char buf1[] = "ABCDEFG"; // 첫 번째 버퍼에 문자열 초기화
    char buf2[] = "123456"; // 두 번째 버퍼에 문자열 초기화
    int str_len; // 쓰여진 바이트 수를 저장할 변수

    vec[0].iov_base = buf1; // 첫 번째 버퍼의 시작 주소
    vec[0].iov_len = 3; // 첫 번째 버퍼에서 3바이트만 전송

    vec[1].iov_base = buf2; // 두 번째 버퍼의 시작 주소
    vec[1].iov_len = 4; // 두 번째 버퍼에서 4바이트만 전송

    str_len = writev(1, vec, 2); // writev를 통해 여러 버퍼의 데이터를 한 번에 출력
    puts(""); // 출력 후 개행 추가
    printf("Write bytes: %d \n", str_len); // 쓰여진 바이트 수 출력

    return 0; // 프로그램 종료
}
