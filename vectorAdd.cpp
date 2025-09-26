#include <cuda.h>
#include <stdio.h>
#include <string.h>

#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

#ifndef checkCudaErrors
#define checkCudaErrors(err) __checkCudaErrors(err, __FILE__, __LINE__)
inline void __checkCudaErrors(CUresult err, const char *file, const int line) {
    if (CUDA_SUCCESS != err) {
        fprintf(stderr,
                "checkCudaErrors() Driver API error = %04d from file <%s>, "
                "line %i.\n",
                err, file, line);
        exit(EXIT_FAILURE);
    }
}
#endif

// Variables
CUdevice cuDevice;
CUcontext cuContext;
CUmodule cuModule;
CUfunction vecAdd_kernel;
float *h_A;
float *h_B;
float *h_C;
CUdeviceptr d_A;
CUdeviceptr d_B;
CUdeviceptr d_C;

int CleanupNoFailure();
void RandomInit(float *, int);

std::chrono::high_resolution_clock::time_point init_start, init_end,
    zeroing_start, zeroing_end, transfer_start, transfer_end, sync_start,
    sync_end, total_start, total_end;
std::chrono::duration<double> init_elapsed_seconds, zeroing_elapsed_seconds,
    transfer_elapsed_seconds, sync_elapsed_seconds, total_elapsed_seconds;

// define input ptx file
#ifndef PTX_FILE
#define PTX_FILE "vectorAdd_kernel.ptx"
#endif

int main(int argc, char **argv) {
    init_start = total_start = std::chrono::system_clock::now();
    int N;
    if (argc < 2) {
        N = 500;
    } else {
        N = atoi(argv[1]);
    }

    printf("Vector Addition (Driver API)\n");

    size_t size = N * sizeof(float);
    unsigned int api_version;

    // create CUDA device & context, and load the kernel
    checkCudaErrors(cuInit(0));
    checkCudaErrors(cuDeviceGet(&cuDevice, 0));  // pick first device
    checkCudaErrors(cuCtxCreate(&cuContext, 0, cuDevice));
#ifdef DEBUG
    checkCudaErrors(cuCtxGetApiVersion(cuContext, &api_version));
    printf("Client: API version: %u\n", api_version);
#endif
    checkCudaErrors(cuModuleLoad(&cuModule, PTX_FILE));
    checkCudaErrors(
        cuModuleGetFunction(&vecAdd_kernel, cuModule, "VecAdd_kernel"));

    init_end = std::chrono::system_clock::now();
    init_elapsed_seconds = init_end - init_start;
    std::cout << "init elapsed time: " << init_elapsed_seconds.count() << "s"
              << std::endl;

    // allocate host vectors
    h_A = (float *)malloc(size);
    h_B = (float *)malloc(size);
    h_C = (float *)malloc(size);

    // Initialize input vectors
    RandomInit(h_A, N);
    RandomInit(h_B, N);
    // Allocate vectors in device memory
    zeroing_start = std::chrono::system_clock::now();
    checkCudaErrors(cuMemAlloc(&d_A, size));
    checkCudaErrors(cuMemAlloc(&d_B, size));
    checkCudaErrors(cuMemAlloc(&d_C, size));
    zeroing_elapsed_seconds = zeroing_end - zeroing_start;
    std::cout << "zeroing elapsed time: " << zeroing_elapsed_seconds.count()
              << "s" << std::endl;

    // Copy vectors from host memory to device memory
    transfer_elapsed_seconds = std::chrono::nanoseconds::zero();
    transfer_start = std::chrono::system_clock::now();
    checkCudaErrors(cuMemcpyHtoD(d_A, h_A, size));
    checkCudaErrors(cuMemcpyHtoD(d_B, h_B, size));
    transfer_end = std::chrono::system_clock::now();
    transfer_elapsed_seconds += transfer_end - transfer_start;

    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

    void *args[] = {&d_A, &d_B, &d_C, &N};

    // Launch the CUDA kernel
    checkCudaErrors(cuLaunchKernel(vecAdd_kernel, blocksPerGrid, 1, 1,
                                   threadsPerBlock, 1, 1, 0, NULL, args, NULL));

    sync_start = std::chrono::system_clock::now();
    checkCudaErrors(cuCtxSynchronize());
    sync_end = std::chrono::system_clock::now();
    sync_elapsed_seconds = sync_end - sync_start;
    std::cout << "sync elapsed time: " << sync_elapsed_seconds.count() << "s"
              << std::endl;

    // copy the result from device back to host
    transfer_start = std::chrono::system_clock::now();
    checkCudaErrors(cuMemcpyDtoH(h_C, d_C, size));
    transfer_end = std::chrono::system_clock::now();
    transfer_elapsed_seconds += transfer_end - transfer_start;
    std::cout << "transfer elapsed time: " << transfer_elapsed_seconds.count()
              << "s" << std::endl;

    // Verify result
    int i;
    int success = true;

    for (i = 0; i < N; ++i) {
        float sum = h_A[i] + h_B[i];
        if (fabs(h_C[i] - sum) > 1e-5) {
            success = false;
            printf("h_C[%d]: %f, sum: %f\n", i, h_C[i], sum);
        }
    }

    std::cout << (success ? "Pass!" : "Fail") << std::endl;

    return CleanupNoFailure();
}

int CleanupNoFailure() {
    checkCudaErrors(cuMemFree(d_A));
    checkCudaErrors(cuMemFree(d_B));
    checkCudaErrors(cuMemFree(d_C));

    // Free host memory
    if (h_A) {
        free(h_A);
    }

    if (h_B) {
        free(h_B);
    }

    if (h_C) {
        free(h_C);
    }

    checkCudaErrors(cuCtxDestroy(cuContext));

    total_end = std::chrono::system_clock::now();
    total_elapsed_seconds = total_end - total_start;
    std::cout << "total elapsed time: " << total_elapsed_seconds.count() << "s"
              << std::endl;

    return EXIT_SUCCESS;
}

// Allocates an array with random float entries.
void RandomInit(float *data, int n) {
    for (int i = 0; i < n; ++i) {
        data[i] = rand() / (float)RAND_MAX;
    }
}
