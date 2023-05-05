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

// Matrix dimensions
// (chosen as multiples of the thread block size for simplicity)
#define WA (4 * block_size)  // Matrix A width
#define HA (6 * block_size)  // Matrix A height
#define WB (4 * block_size)  // Matrix B width
#define HB WA                // Matrix B height
#define WC WB                // Matrix C width
#define HC HA                // Matrix C height

// Variables
CUdevice cuDevice;
CUcontext cuContext;
CUmodule cuModule;
CUfunction cuFunction;
float *h_A;
float *h_B;
float *h_C;
CUdeviceptr d_A;
CUdeviceptr d_B;
CUdeviceptr d_C;

int CleanupNoFailure();
void RandomInit(float *, int);
void constantInit(float *data, int size, float val);

//define input ptx file
#ifndef PTX_FILE
#define PTX_FILE "matrixMul_kernel.ptx"
#endif

int main(int argc, char **argv)
{
    int block_size = 32;
    printf("Matrix Multiplication (Driver API)\n");

    // create CUDA device & context, and load the kernel
    checkCudaErrors(cuInit(0));
    checkCudaErrors(cuDeviceGet(&cuDevice, 0)); // pick first device
    checkCudaErrors(cuCtxCreate(&cuContext, 0, cuDevice));
    // checkCudaErrors(cuCtxGetApiVersion(cuContext, &api_version));
    // printf("Client: API version: %u\n", api_version);
    checkCudaErrors(cuModuleLoad(&cuModule, PTX_FILE));
    checkCudaErrors(cuModuleGetFunction(&cuFunction, cuModule, "matrixMul_bs32_64bit"));

    // allocate host memory for matrices A and B
    unsigned int size_A = WA * HA;
    unsigned int mem_size_A = sizeof(float) * size_A;
    h_A = reinterpret_cast<float *>(malloc(mem_size_A));
    unsigned int size_B = WB * HB;
    unsigned int mem_size_B = sizeof(float) * size_B;
    h_B = reinterpret_cast<float *>(malloc(mem_size_B));

    // initialize host memory
    const float valB = 0.01f;
    constantInit(h_A, size_A, 1.0f);
    constantInit(h_B, size_B, valB);

    // allocate device memory for result
    size_t size_C = WC * HC;
    size_t mem_size_C = sizeof(float) * size_C;
    // allocate mem for the result on host side
    h_C = reinterpret_cast<float *>(malloc(mem_size_C));

    // allocate device memory
    checkCudaErrors(cuMemAlloc(&d_A, mem_size_A));
    checkCudaErrors(cuMemAlloc(&d_B, mem_size_B));
    checkCudaErrors(cuMemAlloc(&d_C, mem_size_C));

    // Copy matrix from host memory to device memory
    checkCudaErrors(cuMemcpyHtoD(d_A, h_A, mem_size_A));
    checkCudaErrors(cuMemcpyHtoD(d_B, h_B, mem_size_B));

    size_t Matrix_Width_A = (size_t)WA;
    size_t Matrix_Width_B = (size_t)WB;

    void *args[5] = {&d_C, &d_A, &d_B, &Matrix_Width_A, &Matrix_Width_B};
    
    // Launch the CUDA kernel
    checkCudaErrors(cuLaunchKernel(cuFunction, (WC / block_size), (HC / block_size), 1,
                            block_size, block_size, 1,
                            2 * block_size * block_size * sizeof(float),
                            NULL, args, NULL));

    checkCudaErrors(cuCtxSynchronize());

    // copy the result from device back to host
    checkCudaErrors(cuMemcpyDtoH(h_C, d_C, mem_size_C));

    // Verify result
    printf("Checking computed result for correctness: ");
    bool correct = true;

    for (int i = 0; i < static_cast<int>(WC * HC); i++) {
        if (fabs(h_C[i] - (WA * valB)) > 1e-5) {
        printf("Error! Matrix[%05d]=%.8f, ref=%.8f error term is > 1e-5\n", i,
                h_C[i], WA * valB);
        correct = false;
        }
    }

    printf("%s\n", correct ? "Result = PASS" : "Result = FAIL");

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

    checkCudaErrors(cuCtxDestroy(cuContext));

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

void constantInit(float *data, int size, float val) {
  for (int i = 0; i < size; ++i) {
    data[i] = val;
  }
}