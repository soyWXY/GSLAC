#include <cstdint>
#include <cassert>
#include <iostream>
#include <dlfcn.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cuda.h>

#define HOST "127.0.0.1"
#define PORT 5566

CUresult cuInit(unsigned int Flags)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
  
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr(HOST);  
    serv_addr.sin_port = htons(PORT); 
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    char com[] = "0001";
    send(sock, com, strlen(com), 0);

    send(sock, &Flags, sizeof(unsigned int), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuInit in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuInit)(unsigned int);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);
 
    // cuInit = reinterpret_cast<CUresult(*)(unsigned int)>(dlsym(handle, "cuInit"));

    // CUresult err;
    // err = cuInit(Flags);

    // dlclose(handle);

    return err;
}

CUresult cuDeviceGet(CUdevice* device, int ordinal)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
  
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr(HOST);  
    serv_addr.sin_port = htons(PORT); 
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    char com[] = "0010";
    send(sock, com, strlen(com), 0);

    send(sock, &ordinal, sizeof(int), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);
    
    std::cout << "Hack cuDeviceGet in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuDeviceGet)(CUdevice*, int);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuDeviceGet = reinterpret_cast<CUresult(*)(CUdevice*, int)>(dlsym(handle, "cuDeviceGet"));
	
    // CUresult err;
    // err = cuDeviceGet(device, ordinal);

    // dlclose(handle);

    return err;
}

CUresult cuCtxCreate(CUcontext* pctx, unsigned int flags, CUdevice dev)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
  
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr(HOST);  
    serv_addr.sin_port = htons(PORT); 
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    char com[] = "0011";
    send(sock, com, strlen(com), 0);

    send(sock, &flags, sizeof(unsigned int), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuCtxCreate in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuCtxCreate)(CUcontext*, unsigned int, CUdevice);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuCtxCreate = reinterpret_cast<CUresult(*)(CUcontext*, unsigned int, CUdevice)>(dlsym(handle, "cuCtxCreate"));
	
    // CUresult err;
    // err = cuCtxCreate(pctx, flags, dev);

    // dlclose(handle);

    return err;
}

CUresult cuCtxGetApiVersion(CUcontext ctx, unsigned int* version)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
  
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr(HOST);  
    serv_addr.sin_port = htons(PORT); 
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    char com[] = "0100";
    send(sock, com, strlen(com), 0);

    recv(sock, version, sizeof(unsigned int), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuCtxGetApiVersion in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuCtxGetApiVersion)(CUcontext, unsigned int*);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuCtxGetApiVersion = reinterpret_cast<CUresult(*)(CUcontext, unsigned int*)>(dlsym(handle, "cuCtxGetApiVersion"));
	
    // CUresult err;
    // err = cuCtxGetApiVersion(ctx, version);

    // dlclose(handle);

    return err;
}

CUresult cuModuleLoad(CUmodule* module, const char* fname)
{
    std::cout << "Hack cuModuleLoad in libcuda.so" << std::endl;

    void *handle;
    CUresult (*cuModuleLoad)(CUmodule*, const char*);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuModuleLoad = reinterpret_cast<CUresult(*)(CUmodule*, const char*)>(dlsym(handle, "cuModuleLoad"));
	
    CUresult err;
    err = cuModuleLoad(module, fname);

    dlclose(handle);

    return err;
}

CUresult cuModuleGetFunction(CUfunction* hfunc, CUmodule hmod, const char* name)
{
    std::cout << "Hack cuModuleGetFunction in libcuda.so" << std::endl;

    void *handle;
    CUresult (*cuModuleGetFunction)(CUfunction*, CUmodule, const char*);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuModuleGetFunction = reinterpret_cast<CUresult(*)(CUfunction*, CUmodule, const char*)>(dlsym(handle, "cuModuleGetFunction"));
	
    CUresult err;
    err = cuModuleGetFunction(hfunc, hmod, name);

    dlclose(handle);

    return err;
}

CUresult cuMemAlloc(CUdeviceptr* dptr, size_t bytesize)
{
    std::cout << "Hack cuMemAlloc in libcuda.so" << std::endl;

    void *handle;
    CUresult (*cuMemAlloc)(CUdeviceptr*, size_t);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuMemAlloc = reinterpret_cast<CUresult(*)(CUdeviceptr*, size_t)>(dlsym(handle, "cuMemAlloc"));
	
    CUresult err;
    err = cuMemAlloc(dptr, bytesize);

    dlclose(handle);

    return err;
}

CUresult cuMemcpyHtoD(CUdeviceptr dstDevice, const void* srcHost, size_t ByteCount)
{
    std::cout << "Hack cuMemcpyHtoD in libcuda.so" << std::endl;

    void *handle;
    CUresult (*cuMemcpyHtoD)(CUdeviceptr, const void*, size_t);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuMemcpyHtoD = reinterpret_cast<CUresult(*)(CUdeviceptr, const void*, size_t)>(dlsym(handle, "cuMemcpyHtoD"));
	
    CUresult err;
    err = cuMemcpyHtoD(dstDevice, srcHost, ByteCount);

    dlclose(handle);

    return err;
}

CUresult cuLaunchKernel(CUfunction f, unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ,
                        unsigned int  blockDimX, unsigned int blockDimY, unsigned int blockDimZ,
                        unsigned int  sharedMemBytes, CUstream hStream, void** kernelParams, void** extra)
{
    std::cout << "Hack cuMemcpyHtoD in libcuda.so" << std::endl;

    void *handle;
    CUresult (*cuLaunchKernel)(CUfunction, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, CUstream, void**, void**);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuLaunchKernel = reinterpret_cast<CUresult(*)(CUfunction, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, CUstream, void**, void**)>(dlsym(handle, "cuLaunchKernel"));
	
    CUresult err;
    err = cuLaunchKernel(f, gridDimX, gridDimY, gridDimZ,
                        blockDimX, blockDimY, blockDimZ,
                        sharedMemBytes, hStream, kernelParams, extra);

    dlclose(handle);

    return err;
}

CUresult cuCtxSynchronize()
{
    std::cout << "Hack cuCtxSynchronize in libcuda.so" << std::endl;

    void *handle;
    CUresult (*cuCtxSynchronize)();

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuCtxSynchronize = reinterpret_cast<CUresult(*)()>(dlsym(handle, "cuCtxSynchronize"));
	
    CUresult err;
    err = cuCtxSynchronize();

    dlclose(handle);

    return err;
}

CUresult cuMemcpyDtoH(void* dstHost, CUdeviceptr srcDevice, size_t ByteCount)
{
    std::cout << "Hack cuMemcpyDtoH in libcuda.so" << std::endl;

    void *handle;
    CUresult (*cuMemcpyDtoH)(void*, CUdeviceptr, size_t);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuMemcpyDtoH = reinterpret_cast<CUresult(*)(void*, CUdeviceptr, size_t)>(dlsym(handle, "cuMemcpyDtoH"));
	
    CUresult err;
    err = cuMemcpyDtoH(dstHost, srcDevice, ByteCount);

    dlclose(handle);

    return err;
}

CUresult cuMemFree(CUdeviceptr dptr)
{
    std::cout << "Hack cuMemFree in libcuda.so" << std::endl;

    void *handle;
    CUresult (*cuMemFree)(CUdeviceptr);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuMemFree = reinterpret_cast<CUresult(*)(CUdeviceptr)>(dlsym(handle, "cuMemFree"));
	
    CUresult err;
    err = cuMemFree(dptr);

    dlclose(handle);

    return err;
}

CUresult cuCtxDestroy(CUcontext ctx)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
  
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr(HOST);  
    serv_addr.sin_port = htons(PORT); 
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    char com[] = "1101";
    send(sock, com, strlen(com), 0);

    CUresult err;
    recv(sock, &err, sizeof(CUresult), 0);

    close(sock);

    std::cout << "Hack cuCtxDestroy in libcuda.so" << std::endl;

    // void *handle;
    // CUresult (*cuCtxDestroy)(CUcontext);

    // handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    // cuCtxDestroy = reinterpret_cast<CUresult(*)(CUcontext)>(dlsym(handle, "cuCtxDestroy"));
	
    // CUresult err;
    // err = cuCtxDestroy(ctx);

    // dlclose(handle);

    return err;
}