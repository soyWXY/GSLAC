// Device code
extern "C" __global__ void MatMul_kernel(const float *A, const float *B, float *C, int N, int M)
{
    int row = blockDim.y * blockIdx.y + threadIdx.y;
    int col = blockDim.x * blockIdx.x + threadIdx.x;

    float temp = 0.0f;
    if (row < N && col < N) {
        
        for (int i = 0; i < N; i++) {
                temp += A[row * N + i] * B[i * N + col];
        }
    }
    C[row * N + col] = temp;
}