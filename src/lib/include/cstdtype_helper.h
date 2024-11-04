#pragma once

#include <type_traits>
#include <cstdint>

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

// Convenience alias template
template <typename T>
using make_signed_custom_t = typename make_signed_custom<T>::type;

// // Example usage
// template <typename T>
// void printSignedType() {
//     using SignedType = make_signed_custom_t<T>;
//     std::cout << "Signed type for " << typeid(T).name() << " is " << typeid(SignedType).name() << std::endl;
// }

// int main() {
//     printSignedType<uint16_t>(); // Should print int16_t
//     printSignedType<uint32_t>(); // Should print int32_t
//     // Add more tests as needed
//     return 0;
// }

