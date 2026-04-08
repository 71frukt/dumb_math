#include <cstdint>
#include <cstring>
#include <limits>
#include <bit>
#include <concepts>

namespace dumb_math {
namespace common    {

template <std::floating_point T>
uint64_t GetMantissa(T x) 
{
    constexpr int digits = std::numeric_limits<T>::digits;

    // float
    if constexpr (sizeof(T) == 4 && digits == 24) 
    {
        uint32_t bits = std::bit_cast<uint32_t>(x);

        return bits & ((1U << (digits - 1)) - 1);
    }

    // double
    else if constexpr (sizeof(T) == 8 && digits == 53) 
    {
        uint64_t bits = std::bit_cast<uint64_t>(x);   

        return bits & ((1ULL << (digits - 1)) - 1);
    }

    // long double
    else if constexpr (digits == 64) 
    {
        // 63-й бит — явная 1 (explicit bit), биты 62..0 — дробная часть
        uint64_t mantissa = 0;
        std::memcpy(&mantissa, &x, sizeof(mantissa));
        
        constexpr int frac_bits = 63;
        
        return static_cast<uint64_t>(mantissa & ((1ULL << frac_bits) - 1));
    }

    else 
    {
        static_assert(digits == -1, "Unsupported floating-point format");
        return 0;
    }
}

template <std::floating_point T>
T GetNormalizedMantissa(T x)
{
    constexpr int digits = std::numeric_limits<T>::digits;

    // float
    if constexpr (digits == 24)
    {
        uint32_t bits = std::bit_cast<uint32_t>(x);
        uint32_t raw_exp = (bits >> 23) & 0xFF;
        uint32_t fraction = bits & 0x7FFFFF;

        if (raw_exp == 0)
        {
            if (fraction == 0) 
                return 1.0f;
            
            // 32 - 1 (sign) - 8 (exp) - 15 (leading zeros in 23-bit frac)
            int shift = std::countl_zero(fraction) - 8;
            
            fraction = (fraction << (shift + 1)) & 0x7FFFFF;
        }

        uint32_t result_bits = (0x7FU << 23) | fraction;
        return std::bit_cast<T>(result_bits);
    }

    // double
    else if constexpr (digits == 53)
    {
        uint64_t bits = std::bit_cast<uint64_t>(x);
        uint64_t raw_exp = (bits >> 52) & 0x7FF;
        uint64_t fraction = bits & 0xFFFFFFFFFFFFFULL;

        if (raw_exp == 0)
        {
            if (fraction == 0) return 1.0;
            int shift = std::countl_zero(fraction) - 11;
            fraction = (fraction << (shift + 1)) & 0xFFFFFFFFFFFFFULL;
        }

        uint64_t result_bits = (1023ULL << 52) | fraction;
        return std::bit_cast<T>(result_bits);
    }

    // x86 long double
    else if constexpr (digits == 64)
    {
        struct RawLD { uint64_t mantissa; uint16_t exp_sign; };
        RawLD parts;
        std::memcpy(&parts, &x, 10);

        uint16_t raw_exp = parts.exp_sign & 0x7FFF;

        if (raw_exp == 0)
        {
            if (parts.mantissa == 0)
                return 1.0L;
            
            int shift = std::countl_zero(parts.mantissa);
            parts.mantissa <<= shift;
        }

        parts.exp_sign = 0x3FFF;
        
        T result;
        std::memcpy(&result, &parts, 10);
        return result;
    }
}

template <std::floating_point T>
int32_t GetExponent(T x) 
{
    constexpr int digits = std::numeric_limits<T>::digits;

    if constexpr (sizeof(T) == 4 && digits == 24) 
    {
        // float: 1 бит знака | 8 бит экспоненты | 23 бита мантиссы
        uint32_t bits = std::bit_cast<uint32_t>(x);        
        uint32_t raw_exp = (bits >> 23) & 0xFF; 
        
        return static_cast<int32_t>(raw_exp) - 127;
    }

    else if constexpr (sizeof(T) == 8 && digits == 53) 
    {
        // double: 1 бит знака | 11 бит экспоненты | 52 бита мантиссы
        uint64_t bits = std::bit_cast<uint64_t>(x);
        uint32_t raw_exp = (bits >> 52) & 0x7FF; 
        
        return static_cast<int32_t>(raw_exp) - 1023;
    }

    else if constexpr (digits == 64) 
    {
        // Байты 0-7: мантисса (64 бита)
        // Байты 8-9: 15 бит экспоненты + 1 бит знака
        uint16_t exp_sign;
        
        std::memcpy(&exp_sign, reinterpret_cast<const char*>(&x) + 8, sizeof(exp_sign));
        
        // отсекает 15-й бит (бит знака числа)
        uint32_t raw_exp = exp_sign & 0x7FFF; 
        
        return static_cast<int32_t>(raw_exp) - 16383;
    }
    
    else 
    {
        static_assert(digits == -1, "Unsupported floating-point format");
        return 0;
    }
}


} // namespace common
} // namespace dumb_math