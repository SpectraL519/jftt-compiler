#pragma once

#include "abstract_identifier.hpp"
#include "identifier_traits.hpp"



namespace jftt::identifier {

namespace detail {

inline void throw_invalid_identifier_cast_error() {
    std::cerr << "[ERROR] Invalid identifier cast" << std::endl;
    std::exit(1);
}

// template <type_discriminator Discriminator>
// inline void assert_valid_identifier_cast(const type_discriminator discriminator) {
//     switch (Discriminator) {
//     case type_discriminator::base:
//         break; // valid casting

//     case type_discriminator::lvalue:
//         switch (discriminator) {
//         case type_discriminator::rvalue:
//             throw_invalid_identifier_cast_error();

//         case type_discriminator::procedure:
//             throw_invalid_identifier_cast_error();

//         default:
//             break; // valid casting
//         }
//         break;

//     default:
//         if (discriminator != Discriminator)
//             throw_invalid_identifier_cast_error();
//         // else: valid casting
//         break;
//     }
// }

} // namespace detail

// template<type_discriminator Discriminator>
// [[nodiscard]] type<Discriminator>* raw_ptr_cast(abstract_identifier* identifier) {
//     detail::assert_valid_identifier_cast<Discriminator>(identifier->discriminator());
//     return static_cast<type<Discriminator>*>(identifier);
// }

// template<type_discriminator Discriminator>
// [[nodiscard]] const type<Discriminator>* raw_ptr_cast(const abstract_identifier* identifier) {
//     detail::assert_valid_identifier_cast<Discriminator>(identifier->discriminator());
//     return static_cast<type<Discriminator>*>(identifier);
// }

// template<type_discriminator Discriminator>
// [[nodiscard]] std::shared_ptr<type<Discriminator>> shared_ptr_cast(
//     const std::shared_ptr<abstract_identifier>& identifier
// ) {
//     detail::assert_valid_identifier_cast<Discriminator>(identifier->discriminator());
//     return std::dynamic_pointer_cast<type<Discriminator>>(identifier);
// }


template<type_discriminator Discriminator>
[[nodiscard]] type<Discriminator>* raw_ptr_cast(abstract_identifier* identifier) {
    auto cast_identifeir{dynamic_cast<type<Discriminator>*>(identifier)};
    if (!cast_identifeir)
        detail::throw_invalid_identifier_cast_error();
    return cast_identifeir;
}

template<type_discriminator Discriminator>
[[nodiscard]] const type<Discriminator>* raw_ptr_cast(const abstract_identifier* identifier) {
    const auto cast_identifeir{dynamic_cast<type<Discriminator>*>(identifier)};
    if (!cast_identifeir)
        detail::throw_invalid_identifier_cast_error();
    return cast_identifeir;
    return dynamic_cast<type<Discriminator>*>(identifier);
}

template<type_discriminator Discriminator>
[[nodiscard]] std::shared_ptr<type<Discriminator>> shared_ptr_cast(
    const std::shared_ptr<abstract_identifier>& identifier
) {
    auto cast_identifeir{std::dynamic_pointer_cast<type<Discriminator>>(identifier)};
    if (!cast_identifeir)
        detail::throw_invalid_identifier_cast_error();
    return cast_identifeir;
}

template<type_discriminator Discriminator>
[[nodiscard]] std::shared_ptr<type<Discriminator>> shared_ptr_cast(
    abstract_identifier* identifier
) {
    auto cast_identifeir{raw_ptr_cast<Discriminator>(identifier)};
    return std::shared_ptr<type<Discriminator>>(cast_identifeir);
}

template<type_discriminator Discriminator>
[[nodiscard]] const std::shared_ptr<type<Discriminator>> shared_ptr_cast(
    const abstract_identifier* identifier
) {
    const auto cast_identifeir{raw_ptr_cast<Discriminator>(identifier)};
    return std::shared_ptr<type<Discriminator>>(cast_identifeir);
}

} // namespace jftt::identifier
