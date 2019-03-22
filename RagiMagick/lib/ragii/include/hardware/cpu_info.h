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
        CpuVendor(CpuRegister reg)
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

    private:
        std::string m_Name;
    };

    class CpuInfo
    {
    public:
        CpuRegister load(uint32_t param);
    };

}  // namespace ragii::hardware
