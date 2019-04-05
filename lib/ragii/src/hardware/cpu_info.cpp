#include "hardware/cpu_info.h"

using namespace ragii::hardware;

std::string CpuInfo::m_VendorName = {};
CpuRegister CpuInfo::m_ID0 = {};
CpuRegister CpuInfo::m_ID1 = {};
CpuRegister CpuInfo::m_ID7 = {};

std::string CpuInfo::getVendorName()
{
    if (!m_VendorName.empty()) {
        return m_VendorName;
    }

    char buf[sizeof(uint32_t) + 1] = {};
    text::arithmetic_to_str(m_ID0.ebx, buf);
    m_VendorName += buf;
    text::arithmetic_to_str(m_ID0.edx, buf);
    m_VendorName += buf;
    text::arithmetic_to_str(m_ID0.ecx, buf);
    m_VendorName += buf;

    return m_VendorName;
}

void CpuInfo::load()
{
    m_ID0 = load(0);
    m_ID1 = load(1);
    m_ID7 = load(7);
}

CpuRegister CpuInfo::load(uint32_t param)
{
    uint32_t a, b, c, d = 0;

#if defined(_MSC_VER)
    __asm {
        mov eax, param
        mov ebx, 0
        mov ecx, 0
        mov edx, 0
        cpuid
        mov a, eax
        mov b, ebx
        mov c, ecx
        mov d, edx
    }
#else
    asm volatile(
        "movl %0, %%eax \n"
        "movl $0, %%ebx \n"
        "movl $0, %%ecx \n"
        "movl $0, %%edx \n"
        "cpuid \n"
        : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
        : "a"(param));
#endif

    return { a, b, c, d };
}
