#pragma once

// include
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
#include <x86intrin.h>
#endif

// warning
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-const-variable"
#elif defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wunused-const-variable"
#endif
