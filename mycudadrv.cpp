#include <cstdint>
#include <cassert>
#include <iostream>
#include <dlfcn.h>
#include <string.h>

#include <cuda.h>

#define HOST "localhost"
#define PORT 3000

CUresult cuInit(unsigned int Flags)
{

    // struct sockaddr_in server_addr;
    // memset(&server_addr, 0, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_port = htons(PORT);

    // int server = socket(AF_INET, SOCK_STREAM, 0);
    // if (server < 0) {
    //     perror("socket");
    //     exit(-1);
    // }

    // // connect to server
    // if (connect(server, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    //     perror("connect");
    //     exit(-1);
    // }

    // send(server, 0b0001, sizeof(0b0001), 0);
    
    // char buf[1024] = {0};

    printf("Hack cuInit in libcuda.so\n");

    void *handle;
    CUresult (*cuInit)(unsigned int);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);
 
    cuInit = reinterpret_cast<CUresult(*)(unsigned int)>(dlsym(handle, "cuInit"));

    CUresult err;
    err = cuInit(Flags);

    dlclose(handle);

    return err;
}

CUresult cuDeviceGet(CUdevice* device, int ordinal)
{
    printf("Hack cuDeviceGet in libcuda.so\n");

    void *handle;
    CUresult (*cuDeviceGet)(CUdevice*, int);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuDeviceGet = reinterpret_cast<CUresult(*)(CUdevice*, int)>(dlsym(handle, "cuDeviceGet"));
	
    CUresult err;
    err = cuDeviceGet(device, ordinal);

    dlclose(handle);

    return err;
}

CUresult cuCtxCreate(CUcontext* pctx, unsigned int flags, CUdevice dev)
{
    printf("Hack cuCtxCreate in libcuda.so\n");

    void *handle;
    CUresult (*cuCtxCreate)(CUcontext*, unsigned int, CUdevice);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuCtxCreate = reinterpret_cast<CUresult(*)(CUcontext*, unsigned int, CUdevice)>(dlsym(handle, "cuCtxCreate"));
	
    CUresult err;
    err = cuCtxCreate(pctx, flags, dev);

    dlclose(handle);

    return err;
}

CUresult cuCtxGetApiVersion(CUcontext ctx, unsigned int* version)
{
    printf("Hack cuCtxGetApiVersion in libcuda.so\n");

    void *handle;
    CUresult (*cuCtxGetApiVersion)(CUcontext, unsigned int*);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);

    cuCtxGetApiVersion = reinterpret_cast<CUresult(*)(CUcontext, unsigned int*)>(dlsym(handle, "cuCtxGetApiVersion"));
	
    CUresult err;
    err = cuCtxGetApiVersion(ctx, version);

    dlclose(handle);

    return err;
}