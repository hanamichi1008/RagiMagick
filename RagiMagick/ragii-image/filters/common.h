#pragma once

#if defined(_MSC_VER)
    #include <intrin.h>
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    #include <x86intrin.h>
#endif
