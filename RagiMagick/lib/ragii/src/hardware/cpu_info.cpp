#include "hardware/cpu_info.h"

using namespace ragii::hardware;

CpuRegister CpuInfo::load(uint32_t param)
{
    uint32_t a, b, c, d = 0;

    asm volatile(
        "movl %0, %%eax \n"
        "cpuid \n"
        : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
        : "a"(param));

    return { a, b, c, d };
}
