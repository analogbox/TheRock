# TheRock for AMD Strix Halo

### ⚡ Out-of-the-Box ROCm & HIP Platform for AMD Ryzen™ AI MAX+ (`gfx1151` / RDNA 3.5) on Ubuntu 26.04 & GCC 15

[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit)](https://github.com/pre-commit/pre-commit) [![Multi-arch CI](https://github.com/ROCm/TheRock/actions/workflows/multi_arch_ci.yml/badge.svg?branch=main&event=push)](https://github.com/ROCm/TheRock/actions/workflows/multi_arch_ci.yml?query=branch%3Amain) [![Ubuntu 26.04](https://img.shields.io/badge/Ubuntu-26.04%20LTS-E95420?logo=ubuntu&logoColor=white)](https://ubuntu.com) [![GCC 15](https://img.shields.io/badge/GCC-15.2-blue?logo=gnu)](https://gcc.gnu.org) [![AMD Strix Halo](https://img.shields.io/badge/AMD%20Strix%20Halo-gfx1151-ED1C24?logo=amd&logoColor=white)](https://www.amd.com)

TheRock is a modular open-source build platform for HIP and ROCm. This fork provides verified, zero-friction support for **AMD Strix Halo APUs (`gfx1151` / Radeon 8060S / 8050S / Ryzen AI MAX+ 395)** on **Ubuntu 26.04 LTS (Resolute Raccoon)**, **GCC 15.2**, and **CMake 4.x**, featuring modular 30-minute builds and hermetic Python virtual environment isolation.

> [!IMPORTANT]
> **🚀 Purpose-Built & Validated for Ubuntu 26.04 LTS (Resolute Raccoon)**  
> Standard ROCm packages and upstream builds fail on Ubuntu 26.04 due to strict **GCC 15.2 ISO C++20 standard header migration**, **CMake 4.x deferred dependency providers**, and **Linux 7.0+ kernel driver ABIs**. This repository is specifically engineered, patched, and benchmarked to provide a **100% stable, out-of-the-box ROCm platform on Ubuntu 26.04 LTS**.

---

## 🖥️ Verified Testbed Platform

| Component | Specification |
| :--- | :--- |
| **System / Model** | **GMKtec NucBox EVO-X2** (SKU: EVO-X2-001 / BIOS: v1.12) |
| **APU / Processor** | **AMD Ryzen™ AI MAX+ 395** (16 Cores, 32 Threads, Strix Halo) |
| **Integrated Graphics** | **AMD Radeon™ 8060S Graphics** (40 Compute Units / 2560 SPs, RDNA 3.5, ISA: `gfx1151`) |
| **System Memory** | **128 GB LPDDR5X** Unified High-Speed Memory |
| **Operating System** | **Ubuntu 26.04 LTS (Resolute Raccoon)** / `Linux 7.0.0-29-generic` (x86_64) |
| **Host Toolchain** | GCC 15.2.0 / G++ 15.2.0, CMake 4.2.3, Ninja 1.12.1, Python 3.14, `uv` 0.x |

---

## 🚀 Quick Start

### 📋 Host Prerequisites (Ubuntu 26.04)
On a fresh Ubuntu machine, ensure essential build packages and development headers are installed:
```bash
sudo apt update && sudo apt install -y \
  build-essential gcc g++ gfortran git ninja-build cmake \
  pkg-config xxd automake libtool python3-dev libegl1-mesa-dev \
  libsqlite3-dev texinfo bison flex curl make ccache
```

### Option 1: One-Liner Zero-Install (No Prior Clone Required!)
On a fresh machine, run this single command to automatically install dependencies, clone the repo, provision Python 3.14 virtual environment, and build:

```bash
# Complete LLM Inference & Fine-Tuning Stack (~30 min build):
curl -fsSL https://raw.githubusercontent.com/analogbox/TheRock/main/bootstrap.sh | bash -s -- --preset llm --python 3.14
```

### Option 2: Download `bootstrap.sh` Separately & Run
If you prefer downloading `bootstrap.sh` first to inspect or run with custom flags:

```bash
# 1. Download bootstrap script
curl -O https://raw.githubusercontent.com/analogbox/TheRock/main/bootstrap.sh
chmod +x bootstrap.sh

# 2. Run automated build (e.g. for LLM Inference Stack)
./bootstrap.sh --preset llm --python 3.14
```

### Option 3: Local Repository Workflow (`therock-env`)
If you already cloned the repository locally:

```bash
# 1. Build desired preset (e.g. LLM Stack)
./therock-env build --preset llm --python 3.14

# 2. Activate environment & verify GPU
source ~/virtualenv/therock-7.14/py314-llm/.venv/bin/activate
rocminfo   # Shows AMD Radeon 8060S / gfx1151
```

---

## 🍕 Workload Presets (`--preset`)

Instead of waiting 5+ hours for 50+ unused components, select targeted packages:

| Workload Tier | Preset | Aliases | Included Components | Recommended For | Build Time |
| :--- | :--- | :--- | :--- | :--- | :---: |
| **Tier 1 (AI / LLM)** | **`llm`** | `lora`, `finetuning` | HIP + `rocBLAS` + `hipBLASLt` + `rocPRIM` + `hipTensor` + `AMD Mesa (RADV Vulkan)` | **vLLM, llama.cpp (HIP & Vulkan), Ollama, MLC-LLM, LoRA/QLoRA Fine-Tuning** | **~30 min** |
| **Tier 1 (AI / LLM)** | **`ai-full`** | `ai`, `training`, `pytorch` | `llm` stack + `MIOpen` (CK) + `RCCL` + `hipDNN` | Full PyTorch training from scratch, CNN/Vision, Stable Diffusion | ~4 hours |
| **Tier 2 (Media)** | **`vulkan-media`** | `vulkan`, `media` | AMD Mesa (RADV Vulkan) + `rocDecode` + `rocJPEG` | Vulkan graphics, 4K/8K video decode, llama.cpp (Vulkan) | **~25 min** |
| **Tier 3 (Engine)** | **`core-hip`** | `hip`, `minimal` | AMD Clang 23 + HIP Runtime + AMDSMI + `rocminfo` | Minimal C++/HIP kernel development & testing | **~20 min** |
| **Tier 3 (Math)** | **`math-hpc`** | `math`, `scientific` | `rocBLAS` + `rocFFT` + `rocSOLVER` + `rocSPARSE` + `rocALUTION` | FFT signal processing, matrix solvers, simulations | ~1.5 hours |
| **Tools** | **`profiler`** | `profiler` | `rocprofiler-sdk`, `rocprofiler-systems`, `rocgdb` | GPU performance tracing & interactive debugging | ~40 min |
| **Monolithic** | **`full`** | `full` | Complete ROCm stack (all 50+ libraries) | Monolithic distribution release build | ~5 hours |

---

## 🎨 Custom Component Builds (`--components` & `--with-*`)

### 1. Build a Custom Environment (`--components`)
Select precisely the components you want:

```bash
# Example: Build custom environment with BLAS, Vulkan, MIOpen, and Fast Fourier Transforms (rocFFT)
./bootstrap.sh --components blas,vulkan,miopen,fft --python 3.14

# Or via therock-env directly
./therock-env build --preset custom --components blas,vulkan,profiler --python 3.14
```

**Supported Component Tokens**:
* `blas` (`rocblas`, `hipblas`, `hipblaslt`): Matrix multiplication & GEMM kernels.
* `vulkan` (`mesa`, `radv`): AMD Mesa Vulkan runtime & shader compiler.
* `miopen` (`ck`): Deep learning convolutions & attention operators.
* `rccl`: Multi-GPU collective communications.
* `fft` (`rocfft`): Fast Fourier Transforms (1D, 2D, 3D).
* `solver` (`rocsolver`): Dense linear system & eigenvalue solvers.
* `sparse` (`rocsparse`): Sparse matrix operations.
* `media` (`rocdecode`, `rocjpeg`): 4K/8K hardware video decoding & JPEG codec.
* `profiler` (`rocgdb`): Profiler, tracer, and GDB debugger.

### 2. On-The-Fly Mix-and-Match Flags (`--with-*`)

You can customize any preset on-the-fly by adding or removing individual components:

```bash
# Example 1: Build LLM preset with GPU Profiler & GDB Debugger
./therock-env build --preset llm --python 3.14 --with-profiler

# Example 2: Build LLM preset with MIOpen for CNN/Vision hybrid tasks
./therock-env build --preset llm --python 3.14 --with-miopen

# Example 3: Build HIP foundation engine with Fast Fourier Transforms (rocFFT)
./therock-env build --preset core-hip --python 3.14 --with-fft

# Example 4: Build LLM preset with ccache compiler acceleration
./bootstrap.sh --preset llm --python 3.14 --with-ccache
```

**Supported Flags**:
* `--with-miopen`: Adds MIOpen & Composable Kernel.
* `--with-rccl`: Adds multi-GPU collective communications (RCCL).
* `--with-profiler`: Adds `rocprofv3`, `rocprofiler-sdk`, and `rocgdb`.
* `--with-fft`: Adds `rocFFT` math library.
* `--with-media` / `--with-vulkan`: Adds AMD Mesa, `rocDecode`, and `rocJPEG`.
* `--with-ccache`: Enables compiler caching (auto-installs `ccache` via `apt` if missing).
* `--without-blas`: Excludes BLAS math libraries.

---

## 🔍 Under the Hood: Virtual Environment Isolation

```
[1. Source Code Factory]
  ~/virtualenv/therock-7.14/TheRock/ (Shared Git Source & Build Scripts)
             │
             ▼ (Compiles targeted preset in ~25-35 min)
[2. Build Output Artifacts]
  ~/virtualenv/therock-7.14/py314-llm/build/dist/rocm/ (Compiled ROCm Libraries)
             │
             ▼ (Automatic Wrapper Injection & Path Linking)
[3. Hermetically Installed inside Virtualenv!]
  ~/virtualenv/therock-7.14/py314-llm/.venv/bin/
             ├── python3 & pip
             ├── rocminfo     ← (Auto-injected wrapper for this specific build!)
             ├── hipcc        ← (Auto-injected wrapper!)
             ├── amdclang     ← (Auto-injected wrapper!)
             └── rocm-smi     ← (Auto-injected wrapper!)
```

* **No System Pollution**: All 220+ ROCm executable wrappers and environment variables (`ROCM_PATH`, `HIP_DEVICE_LIB_PATH`) are installed directly inside the virtual environment (`.venv/bin/`). System `/opt/rocm` is 100% bypassed.
* **Side-by-Side Isolation**: Python 3.14 (`py314-llm`) and Python 3.13 (`py313-llm`) live in isolated sibling folders sharing the source repo without duplicating git history.
* **1-Second Switching**: Activating any `.venv` switches the entire ROCm toolchain instantly. Running `deactivate` reverts to clean Ubuntu.

---

## 📁 Managing Environments & Builds

```bash
# 1. List all detected Python virtual environments
./therock-env list-envs

# 2. List all completed ROCm build trees and disk usage
./therock-env list-builds

# 3. Hermetically install an existing build into a virtual environment
./therock-env install-to-venv --build-dir build_py314_llm --python 3.14
```

---

## 🧪 Post-Build Verification & GPU Testing

Once your environment is built, verify GPU discovery, compiler toolchain, and parallel kernel compute on your **AMD Strix Halo (Radeon 8060S / `gfx1151`)**:

### 1. Activate Environment & Check Hardware Discovery
```bash
source ~/virtualenv/therock-7.14/py314-llm/.venv/bin/activate

# 1. Verify GPU Agent & Topology
rocminfo | grep -E "Marketing Name|Name:|Compute Unit"

# 2. Check HIP Compiler Version
hipcc --version
```

### 2. Compile & Run Parallel GPU Compute Test (Zero-Warning Standard C++)
Create and execute a self-verifying vector addition kernel (1,000,000 elements across 256 threads/block):

```bash
cat << 'EOF' > test_gpu.cpp
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
EOF

# Compile with hipcc and execute:
hipcc test_gpu.cpp -o test_gpu
./test_gpu
```

---

## 📚 Technical Guides & References

* [Upstream Sync & Rebase Guide](docs/UPSTREAM_SYNC_AND_REBASE_GUIDE.md): Step-by-step workflow for upgrading your fork when AMD releases new upstream ROCm tags.
* [GCC 15 & Ubuntu 26.04 Technical Porting Guide](docs/GCC15_UBUNTU2604_PORTING_GUIDE.md): Deep-dive into GCC 15 `<version>` migration, CMake 4.x deferred dependency providers, and Meson symbol version scripts.
* [Development Guide](docs/development/development_guide.md): Developer architecture guide.
* [Supported GPUs](SUPPORTED_GPUS.md): AMD GPU architecture roadmap.
* [CONTRIBUTING.md](CONTRIBUTING.md): Contributing guidelines.
