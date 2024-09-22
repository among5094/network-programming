
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>
// 
int main(int argc, char* argv[]){

    // 
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;


    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,0), &wsaData);

    net_port=htons(host_port);
    net_addr=htonl(host_addr);

    printf("Host ordered port: %#x \n", host_port);
    printf("Network ordered port: %#x \n", net_port);
    printf("Host ordered port: %#x \n", host_addr);
    printf("Network ordered port: %#x \n", net_addr);

    return 0;
}