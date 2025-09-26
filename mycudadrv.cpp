#include <arpa/inet.h>
#include <cuda.h>
#include <dlfcn.h>
#include <linux/vm_sockets.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstdint>
#include <iostream>

#define VMADDR_CID_GPU_SANDBOX 3
#define PORT 5566

CUresult cuInit(unsigned int Flags) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "0001";
    send(sock, com, sizeof(com), 0);

    send(sock, &Flags, sizeof(unsigned int), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuInit in libcuda.so" << std::endl;

    return err;
}

CUresult cuDeviceGet(CUdevice *device, int ordinal) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "0010";
    send(sock, com, sizeof(com), 0);

    send(sock, &ordinal, sizeof(int), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuDeviceGet in libcuda.so" << std::endl;

    return err;
}

CUresult cuCtxCreate(CUcontext *pctx, unsigned int flags, CUdevice dev) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "0011";
    send(sock, com, sizeof(com), 0);

    send(sock, &flags, sizeof(unsigned int), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuCtxCreate in libcuda.so" << std::endl;

    return err;
}

CUresult cuCtxGetApiVersion(CUcontext ctx, unsigned int *version) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "0100";
    send(sock, com, sizeof(com), 0);

    recv(sock, version, sizeof(unsigned int), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuCtxGetApiVersion in libcuda.so" << std::endl;

    return err;
}

CUresult cuModuleLoad(CUmodule *module, const char *fname) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "0101";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuModuleLoad in libcuda.so" << std::endl;

    return err;
}

CUresult cuModuleGetFunction(CUfunction *hfunc, CUmodule hmod,
                             const char *name) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "0110";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuModuleGetFunction in libcuda.so" << std::endl;

    return err;
}

CUresult cuMemAlloc(CUdeviceptr *dptr, size_t bytesize) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "0111";
    send(sock, com, sizeof(com), 0);

    send(sock, &bytesize, sizeof(size_t), 0);

    recv(sock, dptr, sizeof(CUdeviceptr), 0);
    std::cout << "dptr: " << *dptr << std::endl;

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuMemAlloc in libcuda.so" << std::endl;

    return err;
}

CUresult cuMemcpyHtoD(CUdeviceptr dstDevice, const void *srcHost,
                      size_t ByteCount) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1000";
    send(sock, com, sizeof(com), 0);

    send(sock, &dstDevice, sizeof(CUdeviceptr), 0);
    send(sock, &ByteCount, sizeof(size_t), 0);

    float *buf = (float *)malloc(ByteCount);

    memcpy(buf, srcHost, ByteCount);

    size_t chunk = 128;
    int count = ByteCount / chunk;
    for (int i = 0; i < count; i++) {
        send(sock, buf + i * 32, chunk, 0);
    }

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    free(buf);

    close(sock);

    std::cout << "Hack cuMemcpyHtoD in libcuda.so" << std::endl;

    return err;
}

CUresult cuLaunchKernel(CUfunction f, unsigned int gridDimX,
                        unsigned int gridDimY, unsigned int gridDimZ,
                        unsigned int blockDimX, unsigned int blockDimY,
                        unsigned int blockDimZ, unsigned int sharedMemBytes,
                        CUstream hStream, void **kernelParams, void **extra) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1001";
    send(sock, com, sizeof(com), 0);

    send(sock, &gridDimX, sizeof(unsigned int), 0);
    send(sock, &gridDimY, sizeof(unsigned int), 0);
    send(sock, &gridDimZ, sizeof(unsigned int), 0);
    send(sock, &blockDimX, sizeof(unsigned int), 0);
    send(sock, &blockDimY, sizeof(unsigned int), 0);
    send(sock, &blockDimZ, sizeof(unsigned int), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuLaunchKernel in libcuda.so" << std::endl;

    return err;
}

CUresult cuCtxSynchronize() {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1010";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuCtxSynchronize in libcuda.so" << std::endl;

    return err;
}

CUresult cuMemcpyDtoH(void *dstHost, CUdeviceptr srcDevice, size_t ByteCount) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1011";
    send(sock, com, sizeof(com), 0);

    send(sock, &ByteCount, sizeof(size_t), 0);

    float *buf = (float *)malloc(ByteCount);
    int rb = 0;
    size_t chunk = 128;
    int count = ByteCount / chunk;

    for (int i = 0; i < count; i++) {
        int temp = recv(sock, buf + i * 32, chunk, 0);
        rb += temp;
    }

    memcpy(dstHost, buf, ByteCount);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    free(buf);

    close(sock);

    std::cout << "Hack cuMemcpyDtoH in libcuda.so" << std::endl;

    return err;
}

CUresult cuMemFree(CUdeviceptr dptr) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1100";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuMemFree in libcuda.so" << std::endl;

    return err;
}

CUresult cuCtxDestroy(CUcontext ctx) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1101";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuCtxDestroy in libcuda.so" << std::endl;

    return err;
}
