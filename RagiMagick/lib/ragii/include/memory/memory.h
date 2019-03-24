#pragma once
#include <memory>

namespace ragii
{
    template<typename T>
    struct aligned_deleter
    {
        void operator()(T* ptr)
        {
            if (ptr) {
                _aligned_free(ptr);
                ptr = nullptr;
            }
        }
    };

    template<typename T>
    struct aligned_allocator
    {
        static T* alloc(size_t size, int alignment)
        {
            return reinterpret_cast<T*>(_aligned_malloc(size, alignment));
        }

        static decltype(auto) make_unique(size_t size, int alignment)
        {
            return std::unique_ptr<T, aligned_deleter<T>>(alloc(size, alignment));
        }
    };
}  // namespace ragii
