#pragma once

#include <type_traits>
#include <cstdint>
#include <iostream>

namespace ns_type_helper {
// The <stdfloat> header, which provides support for fixed-width floating-point types, is part of the C++23 standard. Here's the support status for different compilers:
// GCC: Support for <stdfloat> is available starting from GCC 13 when using the -std=c++23 option.
// Clang: Clang has support for <stdfloat> starting from Clang 14.
// MSVC: Microsoft Visual Studio (MSVC) supports <stdfloat> from MSVC 19.32.
// Define a constexpr function to check compiler and version
constexpr bool checkCompilerHasStdFloat() {
#if _HAS_CXX23
    #ifdef __clang__
        return (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__) >= 140000;
    #elif defined(__GNUC__) || defined(__GNUG__)
        return (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= 130000;
    #elif defined(_MSC_VER)
        return _MSC_VER >= 1932;
    #else
        return false;
    #endif
#else
    return false;
#endif
}

// Define a template function with no parameters to act as macro avoiding multiple function body implementation
template <typename T = void>
void printCompilerInfo() {
    #ifdef __clang__
        std::cout << "Compiler: Clang\n";
        std::cout << "Version: " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__ << "\n";
    #elif defined(__GNUC__) || defined(__GNUG__)
        std::cout << "Compiler: GCC\n";
        std::cout << "Version: " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << "\n";
    #elif defined(_MSC_VER)
        std::cout << "Compiler: MSVC\n";
        std::cout << "Version: " << _MSC_VER << "\n";
    #else
        std::cout << "Compiler: Unknown\n";
    #endif
}

} // ns_type_helper

#if _HAS_CXX23
    #ifdef __clang__
        #if (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__) >= 140000
        #define COMPILER_HAS_STD_FLOAT
        #endif
    #elif defined(__GNUC__) || defined(__GNUG__)
        #if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) >= 130000
        #define COMPILER_HAS_STD_FLOAT
        #endif
    #elif defined(_MSC_VER)
        #if _MSC_VER >= 1932
        #define COMPILER_HAS_STD_FLOAT
        #endif
    #endif
#endif

#ifdef COMPILER_HAS_STD_FLOAT
namespace std {
    using float16_t = std::uint16_t;    // 16 bit float need half_float or Eigen support
    using float32_t = float;
    using float64_t = double;
    using bfloat16_t = std::uint16_t;
}
#else
#include <stdfloat>
#endif
#include <cmath>

namespace ns_type_helper {

// Primary template - not defined on purpose
template <typename T>
struct make_signed_custom;

// Template specialization for unsigned types
template <>
struct make_signed_custom<uint8_t> {
    using type = int8_t;
};

template <>
struct make_signed_custom<int8_t> {
    using type = int8_t;
};

template <>
struct make_signed_custom<uint16_t> {
    using type = int16_t;
};

template <>
struct make_signed_custom<int16_t> {
    using type = int16_t;
};

template <>
struct make_signed_custom<uint32_t> {
    using type = int32_t;
};

template <>
struct make_signed_custom<int32_t> {
    using type = int32_t;
};

template <>
struct make_signed_custom<uint64_t> {
    using type = int64_t;
};

template <>
struct make_signed_custom<int64_t> {
    using type = int64_t;
};

#ifdef COMPILER_HAS_STD_FLOAT
template <>
struct make_signed_custom<float> {
    using type = float;
};
template <>
struct make_signed_custom<double> {
    using type = double;
};
template <>
struct make_signed_custom<long double> {
    using type = long double;
};
#else
template <>
struct make_signed_custom<std::float16_t> {
    using type = std::float16_t;
};

template <>
struct make_signed_custom<std::float32_t> {
    using type = std::float32_t;
};

template <>
struct make_signed_custom<std::float64_t> {
    using type = std::float64_t;
};

template <>
struct make_signed_custom<std::float128_t> {
    using type = std::float128_t;
};

template <>
struct make_signed_custom<std::bfloat16_t> {
    using type = std::bfloat16_t;
};
#endif

// Helper template to select the floating-point type
template <int Bits>
struct FloatType;

template <>
struct FloatType<16> {
    using type = std::float16_t;
};
template <>
struct FloatType<32> {
    using type = std::float32_t;
};
template <>
struct FloatType<64> {
    using type = std::float64_t;
};
#ifdef is_float128_stable
template <>
struct FloatType<128> {
    using type = std::float128_t;
};
#endif

// Helper function to perform static casting
template <int Bits, typename U>
auto scf(U value) -> typename FloatType<Bits>::type {
    return static_cast<typename FloatType<Bits>::type>(value);
}

// int main() {
//     float value = 1.0f;

//     // Perform the static cast using the helper function with 16 bits
//     auto casted_value_16 = scf<16>(value);

//     // Perform the static cast using the helper function with 32 bits
//     auto casted_value_32 = scf<32>(value);

//     // Print the casted values (cast back to float for printing)
//     std::cout << "Original value: " << value << std::endl;
//     std::cout << "Casted value (16 bits): " << static_cast<float>(casted_value_16) << std::endl;
//     std::cout << "Casted value (32 bits): " << casted_value_32 << std::endl;

//     return 0;
// }

// Convenience alias template
template <typename T>
using make_signed_custom_t = typename make_signed_custom<T>::type;

// compile time check variable type
template <typename T, typename U>
constexpr bool check_eq_typeof() {
    return std::is_same<T, U>::value;
}


// // Example usage
// static_assert(check_eq_typeof<decltype(v), float>());

// // Example usage
// template <typename T>
// void printSignedType() {
//     using SignedType = make_signed_custom_t<T>;
//     std::cout << "Signed type for " << typeid(T).name() << " is " << typeid(SignedType).name() << std::endl;
// }
//
//
// int main() {
//     printSignedType<uint16_t>(); // Should print int16_t
//     printSignedType<uint32_t>(); // Should print int32_t
//     // Add more tests as needed
//     return 0;
// }

// compile time choose % or std::fmod
// Template function to perform modulus operation
template<typename T>
constexpr auto std_div_exact(T a, make_signed_custom_t<T> b, T &q) -> bool {
    if constexpr (std::is_integral_v<T>) {
        q = a / b;
        return 0 == a % b; // Use % for integer types
    } else if constexpr (std::is_floating_point_v<T>) {
        q = static_cast<int>(std::floor(a / b));
        // std::abs not available for std::float128_t
        //return 100 * std::abs(std::fmod(a, b)) < 1; // Use std::fmod for floating-point types
        return -1 < (10000 * std::fmod(a, b))
            && (std::fmod(a, b) * 10000) < 1;
    } else {
        static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "Unsupported type");
    }
}

// Only unsigned char, signed char need special care
constexpr auto std_div_exact(uint8_t a, int8_t b, uint8_t &q) -> bool {
    q = a / b;
    return 0 == a % b;
}

} // ns_type_helper;
