#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <dlfcn.h>
#include <cuda.h>

CUdevice cuDevice;
CUcontext cuContext;
CUmodule cuModule;
CUfunction vecAdd_kernel;
CUdeviceptr d_A;
CUdeviceptr d_B;
CUdeviceptr d_C;

int main(){

    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    serv_addr.sin_port = htons(5566);

    // // Start === Dynamic Loading CUDA Driver API
    // void *handle;
    // CUresult (*cuInit)(unsigned int);
    // CUresult (*cuDeviceGet)(CUdevice*, int);
    // CUresult (*cuCtxCreate)(CUcontext*, unsigned int, CUdevice);
    // CUresult (*cuCtxGetApiVersion)(CUcontext, unsigned int*);
    // CUresult (*cuModuleLoad)(CUmodule*, const char*);
    // CUresult (*cuModuleGetFunction)(CUfunction*, CUmodule, const char*);
    // CUresult (*cuMemAlloc)(CUdeviceptr*, size_t);
    // CUresult (*cuMemcpyHtoD)(CUdeviceptr, const void*, size_t);
    // CUresult (*cuLaunchKernel)(CUfunction, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, CUstream, void**, void**);
    // CUresult (*cuCtxSynchronize)();
    // CUresult (*cuMemcpyDtoH)(void*, CUdeviceptr, size_t);
    // CUresult (*cuMemFree)(CUdeviceptr);
    // CUresult (*cuCtxDestroy)(CUcontext);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);
    // // End === Dynamic Loading CUDA Driver API

    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 20);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    int clnt_sock;
    while((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size)) > 0) {
        char buf[4];
        size_t msg_len = recv(clnt_sock, &buf, sizeof(buf), 0);
        // printf("Received %lu bytes: %.*s\n", msg_len, msg_len, buf);

        if (strcmp(buf, "0001") == 0) {
            std::cout << "0001: cuInit" << std::endl;
        } else if (strcmp(buf, "0010") == 0) {
            std::cout << "0010: cuDeviceGet" << std::endl;
        } else if (strcmp(buf, "0011") == 0) {
            std::cout << "0011: cuCtxCreate" << std::endl;
        } else if (strcmp(buf, "0100") == 0) {
            std::cout << "0100: cuCtxGetApiVersion" << std::endl;
        } else if (strcmp(buf, "0101") == 0) {
            std::cout << "0101: cuModuleLoad" << std::endl;
        } else if (strcmp(buf, "0110") == 0) {
            std::cout << "0110: cuModuleGetFunction" << std::endl;
        } else if (strcmp(buf, "0111") == 0) {
            std::cout << "0111: cuMemAlloc" << std::endl;
        } else if (strcmp(buf, "1000") == 0) {
            std::cout << "1000: cuMemcpyHtoD" << std::endl;
        } else if (strcmp(buf, "1001") == 0) {
            std::cout << "1001: cuLaunchKernel" << std::endl;
        } else if (strcmp(buf, "1010") == 0) {
            std::cout << "1010: cuCtxSynchronize" << std::endl;
        } else if (strcmp(buf, "1011") == 0) {
            std::cout << "1011: cuMemcpyDtoH" << std::endl;
        } else if (strcmp(buf, "1100") == 0) {
            std::cout << "1100: cuMemFree" << std::endl;
        } else if (strcmp(buf, "1101") == 0) {
            std::cout << "1101: cuCtxDestroy" << std::endl;
        } else {
            printf("Exception\n");
        }

        close(clnt_sock);
    }

    close(serv_sock);

    // // Close CUDA Driver API Library
    // dlclose(handle);

    return 0;
}
