// Includes
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath> 
#include <cstring>
#include <cuda.h>

// includes, CUDA
// #include <builtin_types.h>

using namespace std;

// #ifndef checkCudaErrors
// #define checkCudaErrors(err) __checkCudaErrors(err, __FILE__, __LINE__)
// // These are the inline versions for all of the SDK helper functions
// inline void __checkCudaErrors(CUresult err, const char *file, const int line) {
//   if (CUDA_SUCCESS != err) {
//     const char *errorStr = NULL;
//     cuGetErrorString(err, &errorStr);
//     fprintf(stderr,
//             "checkCudaErrors() Driver API error = %04d \"%s\" from file <%s>, "
//             "line %i.\n",
//             err, errorStr, file, line);
//     exit(EXIT_FAILURE);
//   }
// }
// #endif

#ifndef checkCudaErrors
#define checkCudaErrors(err) __checkCudaErrors(err, __FILE__, __LINE__)
inline void __checkCudaErrors(CUresult err, const char *file, const int line) {
if (0 != err) {
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

//define input ptx file
#ifndef PTX_FILE
#define PTX_FILE "vectorAdd_kernel.ptx"
#endif

//define kernel function file
#ifndef PTX_FILE
#define PTX_FILE "vectorAdd_kernel.ptx"
#endif

int main(int argc, char **argv)
{
    printf("Vector Addition (Driver API)\n");
    int N = 50000;
    size_t  size = N * sizeof(float);
    unsigned int api_version;

    // create CUDA device & context, and load the kernel
    checkCudaErrors(cuInit(0));
    checkCudaErrors(cuDeviceGet(&cuDevice, 0)); // pick first device
    checkCudaErrors(cuCtxCreate(&cuContext, 0, cuDevice));
    checkCudaErrors(cuCtxGetApiVersion(cuContext, &api_version));
    printf("Client: API version: %u\n", api_version);
    checkCudaErrors(cuModuleLoad(&cuModule, PTX_FILE));
    checkCudaErrors(cuModuleGetFunction(&vecAdd_kernel, cuModule, "VecAdd_kernel"));

    // allocate host vectors
    h_A = (float *)malloc(size);
    h_B = (float *)malloc(size);
    h_C = (float *)malloc(size);

    // Initialize input vectors
    RandomInit(h_A, N);
    RandomInit(h_B, N);
    // Allocate vectors in device memory
    checkCudaErrors(cuMemAlloc(&d_A, size));
    checkCudaErrors(cuMemAlloc(&d_B, size));
    checkCudaErrors(cuMemAlloc(&d_C, size));
    // Copy vectors from host memory to device memory
    checkCudaErrors(cuMemcpyHtoD(d_A, h_A, size));
    checkCudaErrors(cuMemcpyHtoD(d_B, h_B, size));

    int threadsPerBlock = 256;
    int blocksPerGrid   = (N + threadsPerBlock - 1) / threadsPerBlock;

    void *args[] = { &d_A, &d_B, &d_C, &N };
    
    // Launch the CUDA kernel
        checkCudaErrors(cuLaunchKernel(vecAdd_kernel,  blocksPerGrid, 1, 1,
                               threadsPerBlock, 1, 1,
                               0,
                               NULL, args, NULL));

    checkCudaErrors(cuCtxSynchronize());

    // copy the result from device back to host
    checkCudaErrors(cuMemcpyDtoH(h_C, d_C, size));

    // Verify result
    int i;

    for (i = 0; i < 10; ++i)
    {
        float sum = h_A[i] + h_B[i];
        std::cout << "h_A: " << h_A[i] << " + h_b: " << h_B[i] << " = sum: " << sum << " (h_C = " << h_C[i] << ")" << std::endl;
    }

    return CleanupNoFailure();
}

int CleanupNoFailure()
{
    // Free device memory
    checkCudaErrors(cuMemFree(d_A));
    checkCudaErrors(cuMemFree(d_B));
    checkCudaErrors(cuMemFree(d_C));

    // Free host memory
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

    // checkCudaErrors(cuCtxDestroy(cuContext));

    return EXIT_SUCCESS;
}

// Allocates an array with random float entries.
void RandomInit(float *data, int n)
{
    for (int i = 0; i < n; ++i)
    {
        data[i] = rand() / (float)RAND_MAX;
    }
}
