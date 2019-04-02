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

    class CpuInfo
    {
    public:
        static void load();
        static std::string getVendorName();

        static inline bool isIntel()
        {
            return text::str_to_number<uint32_t>("Genu") == m_ID0.ebx &&
                   text::str_to_number<uint32_t>("ineI") == m_ID0.edx &&
                   text::str_to_number<uint32_t>("ntel") == m_ID0.ecx;
        }

        static inline bool isAMD()
        {
            return text::str_to_number<uint32_t>("Auth") == m_ID0.ebx &&
                   text::str_to_number<uint32_t>("enti") == m_ID0.edx &&
                   text::str_to_number<uint32_t>("cAMD") == m_ID0.ecx;
        }

        static inline bool mmx() { return (m_ID1.edx & (1 << 23)) != 0; }
        static inline bool sse() { return (m_ID1.edx & (1 << 25)) != 0; }
        static inline bool sse2() { return (m_ID1.edx & (1 << 26)) != 0; }
        static inline bool sse3() { return (m_ID1.ecx & 1) != 0; }
        static inline bool ssse3() { return (m_ID1.ecx & (1 << 9)) != 0; }
        static inline bool fma() { return (m_ID1.ecx & (1 << 12)) != 0; }
        static inline bool sse41() { return (m_ID1.ecx & (1 << 19)) != 0; }
        static inline bool sse42() { return (m_ID1.ecx & (1 << 20)) != 0; }
        static inline bool avx() { return (m_ID1.ecx & (1 << 28)) != 0; }
        static inline bool avx2() { return (m_ID7.ebx & (1 << 5)) != 0; }
        static inline bool avx512() { return avx512f(); }
        static inline bool avx512f() { return (m_ID7.ebx & (1 << 16)) != 0; }
        static inline bool avx512dq() { return (m_ID7.ebx & (1 << 17)) != 0; }
        static inline bool avx512ifma52() { return (m_ID7.ebx & (1 << 21)) != 0; }
        static inline bool avx512pf() { return (m_ID7.ebx & (1 << 26)) != 0; }
        static inline bool avx512er() { return (m_ID7.ebx & (1 << 27)) != 0; }
        static inline bool avx512cd() { return (m_ID7.ebx & (1 << 28)) != 0; }
        static inline bool avx512bw() { return (m_ID7.ebx & (1 << 30)) != 0; }
        static inline bool avx512vl() { return (m_ID7.ebx & (1 << 31)) != 0; }
        static inline bool avx512vbmi() { return (m_ID7.ecx & (1 << 1)) != 0; }

        static inline bool htt() { return (m_ID1.edx & (1 << 26)) != 0; }

    private:
        static CpuRegister load(uint32_t param);

        static CpuRegister m_ID0;
        static CpuRegister m_ID1;
        static CpuRegister m_ID7;

        static std::string m_VendorName;
    };

}  // namespace ragii::hardware
