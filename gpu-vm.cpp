#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <netinet/in.h>

#include <math.h>

#include <chrono>
#include <ctime>

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

std::chrono::high_resolution_clock::time_point init_start, init_end, sync_start, sync_end, zeroing_start, zeroing_end, transfer_start, transfer_end, total_start, total_end;
std::chrono::duration<double> init_elapsed_seconds, sync_elapsed_seconds, zeroing_elapsed_seconds, transfer_elapsed_seconds, total_elapsed_seconds;

int main(int argc, char **argv)
{
    int N;
    if (argc < 3) {
        N = 50000;
    } else {
        N = atoi(argv[2]);
    }

    
    int rc, rb = -1;
    int serv_sock, max_sd, clnt_sock;
    struct timeval timeout;
    // struct fd_set master_set, working_set;

    serv_sock = socket(AF_VSOCK, SOCK_STREAM, 0);
    if (serv_sock < 0)
    {
      perror("socket() failed");
      exit(-1);
    }

    struct sockaddr_vm serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.svm_family = AF_VSOCK;  
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    serv_addr.svm_port = htons(5566);
    serv_addr.svm_cid = VMADDR_CID_ANY;

    // Start === Dynamic Loading CUDA Driver API
    void *handle;
    CUresult (*cuInit)(unsigned int);
    CUresult (*cuDeviceGet)(CUdevice*, int);
    CUresult (*cuCtxCreate)(CUcontext*, unsigned int, CUdevice);
    CUresult (*cuCtxGetApiVersion)(CUcontext, unsigned int*);
    CUresult (*cuModuleLoad)(CUmodule*, const char*);
    CUresult (*cuModuleGetFunction)(CUfunction*, CUmodule, const char*);
    CUresult (*cuMemAlloc)(CUdeviceptr*, size_t);
    CUresult (*cuMemsetD32)(CUdeviceptr*, unsigned int, size_t);
    CUresult (*cuMemcpyHtoD)(CUdeviceptr, const void*, size_t);
    CUresult (*cuLaunchKernel)(CUfunction, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, CUstream, void**, void**);
    CUresult (*cuCtxSynchronize)();
    CUresult (*cuMemcpyDtoH)(void*, CUdeviceptr, size_t);
    CUresult (*cuMemFree)(CUdeviceptr);
    CUresult (*cuCtxDestroy)(CUcontext);

    handle = dlopen("/usr/lib/x86_64-linux-gnu/libcuda.so", RTLD_LAZY);
    // End === Dynamic Loading CUDA Driver API

    // Start === Initialize local variable
    size_t  size = N * sizeof(float);

    int width = sqrt(N);
    // allocate host vectors
    h_A = (float *)malloc(size);
    h_B = (float *)malloc(size);
    h_C = (float *)malloc(size);
    memset(h_A, 0, N*sizeof(float));
    memset(h_B, 0, N*sizeof(float));
    memset(h_C, 0, N*sizeof(float));
    // End === Initialize local variable

    rc = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (rc < 0)
    {
      perror("bind() failed");
      close(serv_sock);
      exit(-1);
    }

    
    rc = listen(serv_sock, 20);
    if (rc < 0)
    {
      perror("listen() failed");
      close(serv_sock);
      exit(-1);
    }

    /*
    FD_ZERO(&master_set);
    max_sd = serv_sock;
    FD_SET(serv_sock, &master_set);
    */

    timeout.tv_sec  = 60;
    timeout.tv_usec = 0;

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    while((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size)) > 0) {
        char buf[5] = {-1};
        size_t msg_len = recv(clnt_sock, &buf, sizeof(buf), 0);
        // printf("Received %lu bytes: %.*s\n", msg_len, msg_len, buf);
        std::cout << "msg_len: " << msg_len << ", buf: " << buf << std::endl;

        if (strcmp(buf, "0001") == 0) {
            std::cout << "0001: cuInit" << std::endl;
	        init_start = std::chrono::system_clock::now();
            
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
            if (atoi(argv[1]) == 1)
                err = cuModuleLoad(&cuModule, "vectorAdd_kernel.ptx");
            
            if (atoi(argv[1]) == 2)
                err = cuModuleLoad(&cuModule, "matrixMul_kernel.ptx");

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "0110") == 0) {
            std::cout << "0110: cuModuleGetFunction" << std::endl;

            cuModuleGetFunction = reinterpret_cast<CUresult(*)(CUfunction*, CUmodule, const char*)>(dlsym(handle, "cuModuleGetFunction"));
	
            CUresult err;
            if (atoi(argv[1]) == 1)
                err = cuModuleGetFunction(&vecAdd_kernel, cuModule, "VecAdd_kernel");
            
            if (atoi(argv[1]) == 2)
                err = cuModuleGetFunction(&vecAdd_kernel, cuModule, "MatMul_kernel");

	        init_end = std::chrono::system_clock::now();
            init_elapsed_seconds = init_end-init_start;
            std::cout << "init elapsed time: " << init_elapsed_seconds.count() << "s" << std::endl;

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "0111") == 0) {
            std::cout << "0111: cuMemAlloc" << std::endl;

            cuMemAlloc = reinterpret_cast<CUresult(*)(CUdeviceptr*, size_t)>(dlsym(handle, "cuMemAlloc"));
            cuMemsetD32 = reinterpret_cast<CUresult(*)(CUdeviceptr*, unsigned int, size_t)>(dlsym(handle, "cuMemsetD32"));

            size_t bytesize;
            recv(clnt_sock, &bytesize, sizeof(size_t), 0);

            CUresult err, err2;
            if (!d_A) {
                err = cuMemAlloc(&d_A, bytesize);
                std::cout << "d_A: " << d_A << std::endl;
                send(clnt_sock, &d_A, sizeof(CUdeviceptr), 0);

                zeroing_start = std::chrono::system_clock::now();
                zeroing_elapsed_seconds = std::chrono::nanoseconds::zero();
                err2 = cuMemsetD32(&d_A, 0, bytesize);
                zeroing_end = std::chrono::system_clock::now();
                zeroing_elapsed_seconds += zeroing_end - zeroing_start;

            } else if (!d_B) {
                err = cuMemAlloc(&d_B, bytesize);
                std::cout << "d_B: " << d_B << std::endl;
                send(clnt_sock, &d_B, sizeof(CUdeviceptr), 0);

                zeroing_start = std::chrono::system_clock::now();
                err2 = cuMemsetD32(&d_B, 0, bytesize);
                zeroing_end = std::chrono::system_clock::now();
                zeroing_elapsed_seconds += zeroing_end - zeroing_start;

            } else if (!d_C) {
                err = cuMemAlloc(&d_C, bytesize);
                std::cout << "d_C: " << d_C << std::endl;
                send(clnt_sock, &d_C, sizeof(CUdeviceptr), 0);

                zeroing_start = std::chrono::system_clock::now();
                err2 = cuMemsetD32(&d_C, 0, bytesize);
                zeroing_end = std::chrono::system_clock::now();
                zeroing_elapsed_seconds += zeroing_end - zeroing_start;
                std::cout << "zeroing elapsed time: " << zeroing_elapsed_seconds.count() << "s" << std::endl;

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

                transfer_start = std::chrono::system_clock::now();
                transfer_elapsed_seconds = std::chrono::nanoseconds::zero();
                
		rb = 0;
    		size_t chunk = 128;
    		int count = ByteCount / chunk;

    		for (int i = 0; i < count; i++) {
        	    rb += recv(clnt_sock, h_A+(i*32), chunk, 0);
    		}
    		// std::cout << "Receive " << rb << " bytes" << std::endl;
		
		/*
		rb = recv(clnt_sock, h_A, ByteCount, 0);
		if (rb < 0) {
                    perror("recv() failed");
		    exit(-1);
		}
		*/
                transfer_end = std::chrono::system_clock::now();
                transfer_elapsed_seconds += transfer_end - transfer_start;
                
                err = cuMemcpyHtoD(d_A, h_A, ByteCount);

		/*
		for (int i = 0; i < 5; i++) {
            		for (int j = 0; j < 5; j++) {
                    		printf("%f ", h_A[i*width+j]);
            		}
            		printf("\n");
    		}
		*/

		printf("ByteCount: %zu\n", ByteCount);
		printf("Receive %d bytes\n", rb);

		for (int i = 0; i < 5; i++) {
                    printf("%f ", h_A[i]);
                }
                printf("\n");
                for (int i = N-1; i >= N-5; i--) {
                    printf("%f ", h_A[i]);
                }
                printf("\n");

            } else if (dptr == d_B) {

                transfer_start = std::chrono::system_clock::now();
                
		rb = 0;
                size_t chunk = 128;
                int count = ByteCount / chunk;

                for (int i = 0; i < count; i++) {
                    rb += recv(clnt_sock, h_B+(i*32), chunk, 0);
                }
                // std::cout << "Receive " << rb << " bytes" << std::endl;

		/*
		rb = recv(clnt_sock, h_B, ByteCount, 0);
		if (rb < 0) {
                    perror("recv() failed");
                    exit(-1);
                }
		*/
                transfer_end = std::chrono::system_clock::now();
                transfer_elapsed_seconds += transfer_end - transfer_start;

                err = cuMemcpyHtoD(d_B, h_B, ByteCount);
           
	       	/*
	    	for (int i = 0; i < 5; i++) {
                        for (int j = 0; j < 5; j++) {
                                printf("%f ", h_B[i*width+j]);
                        }
                        printf("\n");
                }
	        */	

		printf("ByteCount: %zu\n", ByteCount);
		printf("Receive %d bytes\n", rb);
                
		for (int i = 0; i < 5; i++) {
                    printf("%f ", h_B[i]);
                }
                printf("\n");
		for (int i = N-1; i >= N-5; i--) {
                    printf("%f ", h_B[i]);
                }
                printf("\n");
	        
	    }

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "1001") == 0) {
            std::cout << "1001: cuLaunchKernel" << std::endl;

	    
	    void **args;
            void *args1[] = { &d_A, &d_B, &d_C, &N };
            void *args2[] = { &d_A, &d_B, &d_C, &width, &width };

	    if (atoi(argv[1]) == 1)
	        args = args1;

	    if (atoi(argv[1]) == 2)
	        args = args2;

	    
	    // void *args[] = { &d_A, &d_B, &d_C, &N };
	    // void *args[] = { &d_A, &d_B, &d_C, &width, &width };


            cuLaunchKernel = reinterpret_cast<CUresult(*)(CUfunction, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, CUstream, void**, void**)>(dlsym(handle, "cuLaunchKernel"));

            unsigned int gridDimX, gridDimY, gridDimZ, blockDimX, blockDimY, blockDimZ;

            recv(clnt_sock, &gridDimX, sizeof(unsigned int), 0);
            recv(clnt_sock, &gridDimY, sizeof(unsigned int), 0);
            recv(clnt_sock, &gridDimZ, sizeof(unsigned int), 0);
            recv(clnt_sock, &blockDimX, sizeof(unsigned int), 0);
            recv(clnt_sock, &blockDimY, sizeof(unsigned int), 0);
            recv(clnt_sock, &blockDimZ, sizeof(unsigned int), 0);

            CUresult err;
            err = cuLaunchKernel(vecAdd_kernel, gridDimX, gridDimY, gridDimZ,
                                blockDimX, blockDimY, blockDimZ,
                                0,
                                NULL, args, NULL);
            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "1010") == 0) {
            std::cout << "1010: cuCtxSynchronize" << std::endl;

            cuCtxSynchronize = reinterpret_cast<CUresult(*)()>(dlsym(handle, "cuCtxSynchronize"));
	
            CUresult err;

	        sync_start = std::chrono::system_clock::now();

            err = cuCtxSynchronize();

	        sync_end = std::chrono::system_clock::now();
            sync_elapsed_seconds = sync_end - sync_start;
            std::cout << "sync elapsed time: " << sync_elapsed_seconds.count() << "s" << std::endl;

            send(clnt_sock, &err, sizeof(CUresult), 0);

        } else if (strcmp(buf, "1011") == 0) {
            std::cout << "1011: cuMemcpyDtoH" << std::endl;

            cuMemcpyDtoH = reinterpret_cast<CUresult(*)(void*, CUdeviceptr, size_t)>(dlsym(handle, "cuMemcpyDtoH"));

            size_t ByteCount;

            recv(clnt_sock, &ByteCount, sizeof(size_t), 0);

	    printf("ByteCount: %zu\n", ByteCount);

            CUresult err;
            err = cuMemcpyDtoH(h_C, d_C, ByteCount);

	    printf("err: %u (Before send)\n", err);

	    // send(clnt_sock, &err, sizeof(CUresult), 0);
	    
            transfer_start = std::chrono::system_clock::now();
            
	    size_t chunk = 128;
	    int count = ByteCount / chunk;
	    // send(clnt_sock, h_C, ByteCount, 0);
	    for(int i = 0; i < count; i++) {
		send(clnt_sock, h_C+(i*32), chunk, 0);
	    }
	    transfer_end = std::chrono::system_clock::now();
            transfer_elapsed_seconds += transfer_end - transfer_start;
            std::cout << "transfer elapsed time: " << transfer_elapsed_seconds.count() << "s" << std::endl;
           
	    printf("err: %u (After send)\n", err);
	    
	    
	    for (int i = 0; i < 5; i++) {
                printf("%f ", h_C[i]);
            }
            printf("\n");
	    
	    for (int i = N-1; i >= N-5; i--) {
                printf("%f ", h_C[i]);
            }
            printf("\n");
	   
		
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
	    memset(h_A, 0, N*sizeof(float));
	    memset(h_B, 0, N*sizeof(float));
	    memset(h_C, 0, N*sizeof(float));

            send(clnt_sock, &err, sizeof(CUresult), 0);

	    total_end = std::chrono::system_clock::now();
	    total_elapsed_seconds = total_end-total_start;

	    std::cout << "total elapsed time: " << total_elapsed_seconds.count() << "s" << std::endl;
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
