#include <hip/hip_runtime.h>
#include <iostream>
#include <vector>

#define HIP_CHECK(call)                                                         \
    do {                                                                        \
        hipError_t err = (call);                                                \
        if (err != hipSuccess) {                                                \
            std::cerr << "HIP Error: " << hipGetErrorString(err)                \
                      << " (" << err << ") at line " << __LINE__ << std::endl;  \
            return 1;                                                           \
        }                                                                       \
    } while (0)

__global__ void vector_add_gpu(const float* a, const float* b, float* c, int n) {
    int idx = hipBlockDim_x * hipBlockIdx_x + hipThreadIdx_x;
    if (idx < n) {
        c[idx] = a[idx] + b[idx];
    }
}

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << " AMD Strix Halo (Radeon 8060S / gfx1151) HIP Test " << std::endl;
    std::cout << "==================================================" << std::endl;

    int deviceCount = 0;
    HIP_CHECK(hipGetDeviceCount(&deviceCount));
    std::cout << ">> Detected GPU Count: " << deviceCount << std::endl;

    hipDeviceProp_t prop;
    HIP_CHECK(hipGetDeviceProperties(&prop, 0));
    std::cout << ">> Device Name       : " << prop.name << std::endl;
    std::cout << ">> GPU Architecture  : " << prop.gcnArchName << std::endl;
    std::cout << ">> Compute Units     : " << prop.multiProcessorCount << " CUs" << std::endl;
    std::cout << ">> Total Global VRAM : " << (prop.totalGlobalMem / 1073741824.0) << " GB" << std::endl;

    const int N = 1000000;
    size_t size = N * sizeof(float);

    std::vector<float> h_a(N, 1.5f);
    std::vector<float> h_b(N, 2.5f);
    std::vector<float> h_c(N, 0.0f);

    float *d_a = nullptr, *d_b = nullptr, *d_c = nullptr;
    HIP_CHECK(hipMalloc(&d_a, size));
    HIP_CHECK(hipMalloc(&d_b, size));
    HIP_CHECK(hipMalloc(&d_c, size));

    HIP_CHECK(hipMemcpy(d_a, h_a.data(), size, hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(d_b, h_b.data(), size, hipMemcpyHostToDevice));

    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

    std::cout << ">> Launching GPU Kernel with " << blocksPerGrid << " blocks x " << threadsPerBlock << " threads..." << std::endl;
    vector_add_gpu<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, N);

    HIP_CHECK(hipGetLastError());
    HIP_CHECK(hipDeviceSynchronize());

    HIP_CHECK(hipMemcpy(h_c.data(), d_c, size, hipMemcpyDeviceToHost));

    bool success = true;
    for (int i = 0; i < N; ++i) {
        if (h_c[i] != 4.0f) {
            success = false;
            break;
        }
    }

    HIP_CHECK(hipFree(d_a));
    HIP_CHECK(hipFree(d_b));
    HIP_CHECK(hipFree(d_c));

    std::cout << "--------------------------------------------------" << std::endl;
    if (success) {
        std::cout << ">> [SUCCESS] 1,000,000 vector elements computed correctly on Radeon 8060S!" << std::endl;
    } else {
        std::cout << ">> [FAILED] Calculation mismatch!" << std::endl;
    }
    std::cout << "==================================================" << std::endl;

    return 0;
}
