#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>

namespace dumb_math::matrix {
namespace detail            {

template <typename T, std::size_t Alignment>
struct AlignedAllocator
{
    using value_type = T;

    template <typename U>
    struct rebind
    {
        using other = AlignedAllocator<U, Alignment>;
    };

    AlignedAllocator() noexcept = default;
    
    template <typename U> 
    AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n)
    {
        if (n == 0) return nullptr;
        
        std::size_t bytes = n * sizeof(T);
        void* ptr = nullptr;

#ifdef _MSC_VER
        // Windows (MSVC)
        ptr = _aligned_malloc(bytes, Alignment);
#else
        // размер должен быть кратен выравниванию по стандарту C++17,
        // поэтому мы округляем размер запрошенной памяти вверх до кратного Alignment.
        std::size_t aligned_bytes = (bytes + Alignment - 1) & ~(Alignment - 1);
        ptr = std::aligned_alloc(Alignment, aligned_bytes);
#endif

        if (!ptr)
        {
            throw std::bad_alloc();
        }
        
        return static_cast<T*>(ptr);
    }

    void deallocate(T* ptr, std::size_t /* n */) noexcept {
#ifdef _MSC_VER
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }

    // Аллокаторы без состояния всегда равны
    bool operator==(const AlignedAllocator&) const noexcept { return true; }
    bool operator!=(const AlignedAllocator&) const noexcept { return false; }
};

} // namespace detail
} // namespace dumb_math::matrix