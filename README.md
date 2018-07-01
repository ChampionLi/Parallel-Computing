# Parallel Computing Library Test

Parallel computing can speed up our programme. From data parallel to process parallel, there are many parallel tool and way that we can use. The project include The Parallel Patterns Library (PPL), Open Multi-Processing (OpenMP) and Open Computing Language (OpenCL) test examples. More details can be found in [my blog](https://championli.github.io/2018/07/01/parallel-computing/)

To use the cpp files, you only need to establish the different Visual Studio projects and compile them respectively. The configurations are sa follows:

## PPL

- Add header `#include <ppl.h>`

## OpenMP

- Add header `#include<omp.h>`
- Turn on the OpenMP in configuration `Property~C/C++~Language~OpenMP Support~Yes`

## OpenCL

- Add header `#include <CL/cl.h>`
- The header files and library files can be found in CUDA tool kit, so add the additional include directory as follows(my configuration, you can look for your tool kit path):
    - `Property~C/C++~Regular~additional include directory~D:\NVIDIA\CUDA\CUDAToolkit\include`
    - `Property~Linker~Regular~additional library directory~D:\NVIDIA\CUDA\CUDAToolkit\lib\x64`
    - `Property~Linker~Import~additional dependencies~OpenCL.lib`
