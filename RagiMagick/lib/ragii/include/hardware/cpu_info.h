#pragma once
#include <cstdint>
#include <string>
#include "text/text.h"

namespace ragii::hardware
{

    struct CpuRegister
    {
        uint32_t eax;
        uint32_t ebx;
        uint32_t ecx;
        uint32_t edx;
    };

    class CpuVendor
    {
    public:
        CpuVendor(const CpuRegister& reg)
        {
            char buf[sizeof(uint32_t) + 1] = {};
            text::arithmetic_to_str(reg.ebx, buf);
            m_Name += buf;
            text::arithmetic_to_str(reg.edx, buf);
            m_Name += buf;
            text::arithmetic_to_str(reg.ecx, buf);
            m_Name += buf;
        }

        std::string getName() { return m_Name; }

        static constexpr bool isIntel(const CpuRegister& reg)
        {
            return text::str_to_number<uint32_t>("Genu") == reg.ebx &&
                   text::str_to_number<uint32_t>("ineI") == reg.edx &&
                   text::str_to_number<uint32_t>("ntel") == reg.ecx;
        }

    private:
        std::string m_Name;
    };

    class CpuAvailableFeatures
    {
    public:
        CpuAvailableFeatures(const CpuRegister& reg)
            : m_Register(reg)
        {
        }

        inline bool sse() { return (m_Register.edx & (1 << 25)) == (1 << 25); }
        inline bool sse2() { return (m_Register.edx & (1 << 26)) == (1 << 26); }
        inline bool sse3() { return (m_Register.ecx & 1) == 1; }
        inline bool ssse3() { return (m_Register.ecx & (1 << 9)) == (1 << 9); }
        inline bool sse41() { return (m_Register.ecx & (1 << 19)) == (1 << 19); }
        inline bool sse42() { return (m_Register.ecx & (1 << 20)) == (1 << 20); }
        inline bool avx() { return (m_Register.ecx & (1 << 28)) == (1 << 28); }
        inline bool avx2() { return (m_Register.ebx & (1 << 5)) == (1 << 5); }

    private:
        CpuRegister m_Register;
    };

    class CpuInfo
    {
    public:
        CpuRegister load(uint32_t param);
    };

}  // namespace ragii::hardware
