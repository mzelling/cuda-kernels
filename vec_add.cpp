#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

__global__ void vec_add_kernel(float* a, float* b, float* c, size_t n) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    printf("Hello from thread %d!\n", threadIdx.x);

    if (i < n) {
        c[i] = a[i] + b[i];
    }
};

float* vec_add(float* a, float* b, float* c, size_t n) {
    // Step 1: allocate memory on the GPU
    size_t size = sizeof(float) * n;
    float* gpu_a;
    float* gpu_b; 
    float* gpu_c;
    cudaError_t err_a = cudaMalloc((void **) &gpu_a, size);
    cudaError_t err_b = cudaMalloc((void **) &gpu_b, size);
    cudaError_t err_c = cudaMalloc((void **) &gpu_c, size);

    if ((err_a != cudaSuccess) || (err_b != cudaSuccess) || (err_c != cudaSuccess)) {
        printf("%s in %s at line %d\n", cudaGetErrorString(err_a), __FILE__, __LINE__);
        printf("%s in %s at line %d\n", cudaGetErrorString(err_b), __FILE__, __LINE__);
        printf("%s in %s at line %d\n", cudaGetErrorString(err_c), __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    // Step 2: copy memory onto the GPU
    cudaMemcpy(gpu_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(gpu_b, b, size, cudaMemcpyHostToDevice);

    // Step 3: launch the CUDA kernel to compute
    vec_add_kernel<<<1, 4>>>(gpu_a, gpu_b, gpu_c, n);

    // Step 4: Read the result back from memory
    cudaMemcpy(c, gpu_c, size, cudaMemcpyDeviceToHost);

    // Step 5: Free the GPU memory
    cudaFree(gpu_a); 
    cudaFree(gpu_b); 
    cudaFree(gpu_c);

    return c;
};

int main() {
    float a[] = {1.0, 2.5, 3.6};
    float b[] = {2.3, 1.5, 1.4};
    float* c = new float[3];
    c[0] = 1.0;
    vec_add((float *) a, (float* ) b, c, 3);
    printf("a = [ %f %f %f ]\n", a[0], a[1], a[2]);
    printf("b = [ %f %f %f ]\n", b[0], b[1], b[2]);
    printf("a + b = [ %f %f %f ]\n", c[0], c[1], c[2]);
};