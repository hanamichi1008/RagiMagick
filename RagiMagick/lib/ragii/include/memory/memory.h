#pragma once
#include <cstdlib>
#include <memory>

namespace ragii
{
    template<typename T>
    T* aligned_alloc(size_t size, size_t align)
    {
#if defined(_MSC_VER)
        return reinterpret_cast<T*>(_aligned_malloc(size, align));
#else
        return reinterpret_cast<T*>(std::aligned_alloc(size, align));
#endif
    }

    template<typename T>
    struct aligned_deleter
    {
        void operator()(T* ptr)
        {
            if (ptr) {
#if defined(_MSC_VER)
                _aligned_free(ptr);
#else
                free(ptr);
#endif
                ptr = nullptr;
            }
        }
    };

    template<typename T>
    struct aligned_allocator
    {
        static decltype(auto) make_unique(size_t size, int alignment)
        {
            return std::unique_ptr<T, aligned_deleter<T>>(aligned_alloc<T>(size, alignment));
        }

        static decltype(auto) make_shared(size_t size, int alignment)
        {
            return std::shared_ptr<T, aligned_deleter<T>>(aligned_alloc<T>(size, alignment));
        }
    };

}  // namespace ragii
