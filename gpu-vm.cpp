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
float *h_A;
float *h_B;
float *h_C;
CUdeviceptr d_A = 0;
CUdeviceptr d_B = 0;
CUdeviceptr d_C = 0;

int main(){

    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    serv_addr.sin_port = htons(5566);

    // Start === Dynamic Loading CUDA Driver API
    void *handle;
    CUresult (*cuInit)(unsigned int);
    CUresult (*cuDeviceGet)(CUdevice*, int);
    CUresult (*cuCtxCreate)(CUcontext*, unsigned int, CUdevice);
    CUresult (*cuCtxGetApiVersion)(CUcontext, unsigned int*);
    CUresult (*cuModuleLoad)(CUmodule*, const char*);
    CUresult (*cuModuleGetFunction)(CUfunction*, CUmodule, const char*);
    CUresult (*cuMemAlloc)(CUdeviceptr*, size_t);
    CUresult (*cuMemcpyHtoD)(CUdeviceptr, const void*, size_t);
    CUresult (*cuLaunchKernel)(CUfunction, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, CUstream, void**, void**);
    CUresult (*cuCtxSynchronize)();
    CUresult (*cuMemcpyDtoH)(void*, CUdeviceptr, size_t);
    CUresult (*cuMemFree)(CUdeviceptr);
    CUresult (*cuCtxDestroy)(CUcontext);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);
    // End === Dynamic Loading CUDA Driver API

    // Start === Initialize local variable
    int N = 50000;
    size_t  size = N * sizeof(float);

    // allocate host vectors
    h_A = (float *)malloc(size);
    h_B = (float *)malloc(size);
    h_C = (float *)malloc(size);
    // End === Initialize local variable

    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 20);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    int clnt_sock;
    while((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size)) > 0) {
        char buf[5] = {-1};
        size_t msg_len = recv(clnt_sock, &buf, sizeof(buf), 0);
        // printf("Received %lu bytes: %.*s\n", msg_len, msg_len, buf);
        std::cout << "msg_len: " << msg_len << ", buf: " << buf << std::endl;

        if (strcmp(buf, "0001") == 0) {
            std::cout << "0001: cuInit" << std::endl;
            
            unsigned int flags = -1;
            recv(clnt_sock, &flags, sizeof(unsigned int), 0);
            std::cout << "Receive: Flags " << flags << std::endl;

            cuInit = reinterpret_cast<CUresult(*)(unsigned int)>(dlsym(handle, "cuInit"));
            CUresult err;
            err = cuInit(flags);

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "0010") == 0) {
            std::cout << "0010: cuDeviceGet" << std::endl;

            int devID = -1;
            recv(clnt_sock, &devID, sizeof(int), 0);
            std::cout << "Receive: devID " << devID << std::endl;

            cuDeviceGet = reinterpret_cast<CUresult(*)(CUdevice*, int)>(dlsym(handle, "cuDeviceGet"));
	
            CUresult err;
            err = cuDeviceGet(&cuDevice, devID);

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "0011") == 0) {
            std::cout << "0011: cuCtxCreate" << std::endl;

            unsigned int flags = -1;
            recv(clnt_sock, &flags, sizeof(unsigned int), 0);
            std::cout << "Receive: flags " << flags << std::endl;

            cuCtxCreate = reinterpret_cast<CUresult(*)(CUcontext*, unsigned int, CUdevice)>(dlsym(handle, "cuCtxCreate"));
	
            CUresult err;
            err = cuCtxCreate(&cuContext, flags, cuDevice);

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "0100") == 0) {
            std::cout << "0100: cuCtxGetApiVersion" << std::endl;

            unsigned int version = -1;

            cuCtxGetApiVersion = reinterpret_cast<CUresult(*)(CUcontext, unsigned int*)>(dlsym(handle, "cuCtxGetApiVersion"));
	
            CUresult err;
            err = cuCtxGetApiVersion(cuContext, &version);

            send(clnt_sock, &version, sizeof(unsigned int), 0);

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "0101") == 0) {
            std::cout << "0101: cuModuleLoad" << std::endl;

            cuModuleLoad = reinterpret_cast<CUresult(*)(CUmodule*, const char*)>(dlsym(handle, "cuModuleLoad"));
	
            char fname[25] = {0};

            std::cout << "Receive fname: " << fname << std::endl;
            
            CUresult err;
            err = cuModuleLoad(&cuModule, "vectorAdd_kernel.ptx");

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "0110") == 0) {
            std::cout << "0110: cuModuleGetFunction" << std::endl;

            cuModuleGetFunction = reinterpret_cast<CUresult(*)(CUfunction*, CUmodule, const char*)>(dlsym(handle, "cuModuleGetFunction"));
	
            CUresult err;
            err = cuModuleGetFunction(&vecAdd_kernel, cuModule, "VecAdd_kernel");

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "0111") == 0) {
            std::cout << "0111: cuMemAlloc" << std::endl;

            cuMemAlloc = reinterpret_cast<CUresult(*)(CUdeviceptr*, size_t)>(dlsym(handle, "cuMemAlloc"));

            size_t bytesize;
            recv(clnt_sock, &bytesize, sizeof(size_t), 0);

            CUresult err;
            if (!d_A) {
                err = cuMemAlloc(&d_A, bytesize);
                std::cout << "d_A: " << d_A << std::endl;
                send(clnt_sock, &d_A, sizeof(CUdeviceptr), 0);
            } else if (!d_B) {
                err = cuMemAlloc(&d_B, bytesize);
                std::cout << "d_B: " << d_B << std::endl;
                send(clnt_sock, &d_B, sizeof(CUdeviceptr), 0);
            } else if (!d_C) {
                err = cuMemAlloc(&d_C, bytesize);
                std::cout << "d_C: " << d_C << std::endl;
                send(clnt_sock, &d_C, sizeof(CUdeviceptr), 0);
            }

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "1000") == 0) {
            std::cout << "1000: cuMemcpyHtoD" << std::endl;

            cuMemcpyHtoD = reinterpret_cast<CUresult(*)(CUdeviceptr, const void*, size_t)>(dlsym(handle, "cuMemcpyHtoD"));

            CUdeviceptr dptr;
            size_t ByteCount;

            recv(clnt_sock, &dptr, sizeof(CUdeviceptr), 0);
            recv(clnt_sock, &ByteCount, sizeof(size_t), 0);

            CUresult err;

            if (dptr == d_A) {
                recv(clnt_sock, h_A, ByteCount, 0);
                err = cuMemcpyHtoD(d_A, h_A, ByteCount);
            } else if (dptr == d_B) {
                recv(clnt_sock, h_B, ByteCount, 0);
                err = cuMemcpyHtoD(d_B, h_B, ByteCount);
            }

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "1001") == 0) {
            std::cout << "1001: cuLaunchKernel" << std::endl;

            void *args[] = { &d_A, &d_B, &d_C, &N };

            cuLaunchKernel = reinterpret_cast<CUresult(*)(CUfunction, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, CUstream, void**, void**)>(dlsym(handle, "cuLaunchKernel"));

            unsigned int blocksPerGrid, threadsPerBlock;

            recv(clnt_sock, &blocksPerGrid, sizeof(unsigned int), 0);
            recv(clnt_sock, &threadsPerBlock, sizeof(unsigned int), 0);

            CUresult err;
            err = cuLaunchKernel(vecAdd_kernel, blocksPerGrid, 1, 1,
                                threadsPerBlock, 1, 1,
                                0,
                                NULL, args, NULL);
            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "1010") == 0) {
            std::cout << "1010: cuCtxSynchronize" << std::endl;

            cuCtxSynchronize = reinterpret_cast<CUresult(*)()>(dlsym(handle, "cuCtxSynchronize"));
	
            CUresult err;
            err = cuCtxSynchronize();
            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "1011") == 0) {
            std::cout << "1011: cuMemcpyDtoH" << std::endl;

            cuMemcpyDtoH = reinterpret_cast<CUresult(*)(void*, CUdeviceptr, size_t)>(dlsym(handle, "cuMemcpyDtoH"));

            size_t ByteCount;

            recv(clnt_sock, &ByteCount, sizeof(size_t), 0);

            CUresult err;
            err = cuMemcpyDtoH(h_C, d_C, ByteCount);

            send(clnt_sock, h_C, ByteCount, 0);

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "1100") == 0) {
            std::cout << "1100: cuMemFree" << std::endl;

            cuMemFree = reinterpret_cast<CUresult(*)(CUdeviceptr)>(dlsym(handle, "cuMemFree"));

            CUresult err;
            if (d_A) {
                err = cuMemFree(d_A);
                d_A = 0;
            } else if (d_B) {
                err = cuMemFree(d_B);
                d_B = 0;
            } else if (d_C) {
                err = cuMemFree(d_C);
                d_C = 0;
            }

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "1101") == 0) {
            std::cout << "1101: cuCtxDestroy" << std::endl;

            cuCtxDestroy = reinterpret_cast<CUresult(*)(CUcontext)>(dlsym(handle, "cuCtxDestroy"));
	
            CUresult err;
            err = cuCtxDestroy(cuContext);

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else {
            printf("Exception\n");
        }

        close(clnt_sock);
    }

    close(serv_sock);

    // Close CUDA Driver API Library
    dlclose(handle);

    // Start === Destroy local variable
    // allocate host vectors
    if (h_A)
    {
        free(h_A);
    }

    if (h_B)
    {
        free(h_B);
    }

    if (h_C)
    {
        free(h_C);
    }
    // End === Initialize local variable

    return 0;
}
