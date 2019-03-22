#include "hardware/cpu_info.h"

using namespace ragii::hardware;

CpuRegister CpuInfo::load(uint32_t param)
{
    uint32_t a, b, c, d = 0;

#ifdef _MSC_VER
#    ifdef _M_X64
#        error 非対応アーキテクチャ！
#    endif
    __asm
    {
        mov eax, param
        cpuid
        mov a, eax
        mov b, ebx
        mov c, ecx
        mov d, edx
    }
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    asm volatile(
        "movl %0, %%eax \n"
        "cpuid \n"
        : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
        : "a"(param));
#endif

    return { a, b, c, d };
}
