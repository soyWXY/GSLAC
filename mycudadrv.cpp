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
// #define HOST "127.0.0.1"
#define PORT 5566

CUresult cuInit(unsigned int Flags) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
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

    // void *handle;
    // CUresult (*cuInit)(unsigned int);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuInit = reinterpret_cast<CUresult(*)(unsigned int)>(dlsym(handle,
    // "cuInit"));

    // CUresult err;
    // err = cuInit(Flags);

    // dlclose(handle);

    return err;
}

CUresult cuDeviceGet(CUdevice *device, int ordinal) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
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

    // void *handle;
    // CUresult (*cuDeviceGet)(CUdevice*, int);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuDeviceGet = reinterpret_cast<CUresult(*)(CUdevice*, int)>(dlsym(handle,
    // "cuDeviceGet"));

    // CUresult err;
    // err = cuDeviceGet(device, ordinal);

    // dlclose(handle);

    return err;
}

CUresult cuCtxCreate(CUcontext *pctx, unsigned int flags, CUdevice dev) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
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

    // void *handle;
    // CUresult (*cuCtxCreate)(CUcontext*, unsigned int, CUdevice);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuCtxCreate = reinterpret_cast<CUresult(*)(CUcontext*, unsigned int,
    // CUdevice)>(dlsym(handle, "cuCtxCreate"));

    // CUresult err;
    // err = cuCtxCreate(pctx, flags, dev);

    // dlclose(handle);

    return err;
}

CUresult cuCtxGetApiVersion(CUcontext ctx, unsigned int *version) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
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

    // void *handle;
    // CUresult (*cuCtxGetApiVersion)(CUcontext, unsigned int*);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuCtxGetApiVersion = reinterpret_cast<CUresult(*)(CUcontext, unsigned
    // int*)>(dlsym(handle, "cuCtxGetApiVersion"));

    // CUresult err;
    // err = cuCtxGetApiVersion(ctx, version);

    // dlclose(handle);

    return err;
}

CUresult cuModuleLoad(CUmodule *module, const char *fname) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "0101";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuModuleLoad in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuModuleLoad)(CUmodule*, const char*);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuModuleLoad = reinterpret_cast<CUresult(*)(CUmodule*, const
    // char*)>(dlsym(handle, "cuModuleLoad"));

    // CUresult err;
    // err = cuModuleLoad(module, fname);

    // dlclose(handle);

    return err;
}

CUresult cuModuleGetFunction(CUfunction *hfunc, CUmodule hmod,
                             const char *name) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "0110";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuModuleGetFunction in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuModuleGetFunction)(CUfunction*, CUmodule, const char*);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuModuleGetFunction = reinterpret_cast<CUresult(*)(CUfunction*, CUmodule,
    // const char*)>(dlsym(handle, "cuModuleGetFunction"));

    // CUresult err;
    // err = cuModuleGetFunction(hfunc, hmod, name);

    // dlclose(handle);

    return err;
}

CUresult cuMemAlloc(CUdeviceptr *dptr, size_t bytesize) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
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

    // void *handle;
    // CUresult (*cuMemAlloc)(CUdeviceptr*, size_t);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuMemAlloc = reinterpret_cast<CUresult(*)(CUdeviceptr*,
    // size_t)>(dlsym(handle, "cuMemAlloc"));

    // CUresult err;
    // err = cuMemAlloc(dptr, bytesize);

    // dlclose(handle);

    return err;
}

CUresult cuMemcpyHtoD(CUdeviceptr dstDevice, const void *srcHost,
                      size_t ByteCount) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1000";
    send(sock, com, sizeof(com), 0);

    send(sock, &dstDevice, sizeof(CUdeviceptr), 0);
    send(sock, &ByteCount, sizeof(size_t), 0);
    // send(sock, srcHost, ByteCount, 0);

    float *buf = (float *)malloc(ByteCount);

    memcpy(buf, srcHost, ByteCount);

    size_t chunk = 128;
    int count = ByteCount / chunk;
    for (int i = 0; i < count; i++) {
        send(sock, buf + i * 32, chunk, 0);
    }

    // std::cout << "ByteCount: " << ByteCount << std::endl;

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    free(buf);

    close(sock);

    std::cout << "Hack cuMemcpyHtoD in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuMemcpyHtoD)(CUdeviceptr, const void*, size_t);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuMemcpyHtoD = reinterpret_cast<CUresult(*)(CUdeviceptr, const void*,
    // size_t)>(dlsym(handle, "cuMemcpyHtoD"));

    // CUresult err;
    // err = cuMemcpyHtoD(dstDevice, srcHost, ByteCount);

    // dlclose(handle);

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
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
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

    // void *handle;
    // CUresult (*cuLaunchKernel)(CUfunction, unsigned int, unsigned int,
    // unsigned int, unsigned int, unsigned int, unsigned int, unsigned int,
    // CUstream, void**, void**);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuLaunchKernel = reinterpret_cast<CUresult(*)(CUfunction, unsigned int,
    // unsigned int, unsigned int, unsigned int, unsigned int, unsigned int,
    // unsigned int, CUstream, void**, void**)>(dlsym(handle,
    // "cuLaunchKernel"));

    // CUresult err;
    // err = cuLaunchKernel(f, gridDimX, gridDimY, gridDimZ,
    //                     blockDimX, blockDimY, blockDimZ,
    //                     sharedMemBytes, hStream, kernelParams, extra);

    // dlclose(handle);

    return err;
}

CUresult cuCtxSynchronize() {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1010";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuCtxSynchronize in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuCtxSynchronize)();

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuCtxSynchronize = reinterpret_cast<CUresult(*)()>(dlsym(handle,
    // "cuCtxSynchronize"));

    // CUresult err;
    // err = cuCtxSynchronize();

    // dlclose(handle);

    return err;
}

CUresult cuMemcpyDtoH(void *dstHost, CUdeviceptr srcDevice, size_t ByteCount) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1011";
    send(sock, com, sizeof(com), 0);

    send(sock, &ByteCount, sizeof(size_t), 0);

    // CUresult err;
    // recv(sock, &err, sizeof(CUresult), 0);

    /*
    int rb = recv(sock, dstHost, ByteCount, 0);
    if (rb < 0)
    {
        perror("recv() error");
    close(sock);
    exit(-1);
    }
    std::cout << "ByteCount: " << ByteCount << std::endl;
    std::cout << "Receive " << rb << " bytes" << std::endl;
    */
    float *buf = (float *)malloc(ByteCount);
    int rb = 0;
    size_t chunk = 128;
    int count = ByteCount / chunk;

    for (int i = 0; i < count; i++) {
        int temp = recv(sock, buf + i * 32, chunk, 0);
        rb += temp;
    }
    // std::cout << "Final: Receive " << rb << " bytes" << std::endl;

    memcpy(dstHost, buf, ByteCount);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    free(buf);

    close(sock);

    std::cout << "Hack cuMemcpyDtoH in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuMemcpyDtoH)(void*, CUdeviceptr, size_t);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuMemcpyDtoH = reinterpret_cast<CUresult(*)(void*, CUdeviceptr,
    // size_t)>(dlsym(handle, "cuMemcpyDtoH"));

    // CUresult err;
    // err = cuMemcpyDtoH(dstHost, srcDevice, ByteCount);

    // dlclose(handle);

    return err;
}

CUresult cuMemFree(CUdeviceptr dptr) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1100";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuMemFree in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuMemFree)(CUdeviceptr);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuMemFree = reinterpret_cast<CUresult(*)(CUdeviceptr)>(dlsym(handle,
    // "cuMemFree"));

    // CUresult err;
    // err = cuMemFree(dptr);

    // dlclose(handle);

    return err;
}

CUresult cuCtxDestroy(CUcontext ctx) {
    int sock = socket(AF_VSOCK, SOCK_STREAM, 0);

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;
    // serv_addr.sin_addr.s_addr = inet_addr(HOST);
    serv_addr.svm_port = htons(PORT);
    serv_addr.svm_cid = VMADDR_CID_GPU_SANDBOX;
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char com[5] = "1101";
    send(sock, com, sizeof(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuCtxDestroy in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuCtxDestroy)(CUcontext);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuCtxDestroy = reinterpret_cast<CUresult(*)(CUcontext)>(dlsym(handle,
    // "cuCtxDestroy"));

    // CUresult err;
    // err = cuCtxDestroy(ctx);

    // dlclose(handle);

    return err;
}
