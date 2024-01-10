#pragma once

#include "abstract_identifier.hpp"
#include "identifier_traits.hpp"



namespace jftt::identifier {

namespace detail {

inline void throw_invalid_identifier_cast_error(
    const identifier::type_discriminator from, const identifier::type_discriminator to
) {
    std::cerr << "[ERROR] Invalid identifier cast from " << identifier::as_string(from)
              << " to " << identifier::as_string(to) << std::endl;
    std::exit(1);
}

} // namespace detail

template<type_discriminator Discriminator = type_discriminator::base>
[[nodiscard]] type<Discriminator>* raw_ptr_cast(abstract_identifier* identifier) {
    auto cast_identifeir{dynamic_cast<type<Discriminator>*>(identifier)};
    if (!cast_identifeir)
        detail::throw_invalid_identifier_cast_error(identifier->discriminator(), Discriminator);
    return cast_identifeir;
}

template<type_discriminator Discriminator = type_discriminator::base>
[[nodiscard]] const type<Discriminator>* raw_ptr_cast(const abstract_identifier* identifier) {
    const auto cast_identifeir{dynamic_cast<type<Discriminator>*>(identifier)};
    if (!cast_identifeir)
        detail::throw_invalid_identifier_cast_error(identifier->discriminator(), Discriminator);
    return cast_identifeir;
    return dynamic_cast<type<Discriminator>*>(identifier);
}

template<type_discriminator Discriminator = type_discriminator::base>
[[nodiscard]] std::shared_ptr<type<Discriminator>> shared_ptr_cast(
    const std::shared_ptr<abstract_identifier>& identifier
) {
    auto cast_identifeir{std::dynamic_pointer_cast<type<Discriminator>>(identifier)};
    if (!cast_identifeir)
        detail::throw_invalid_identifier_cast_error(identifier->discriminator(), Discriminator);
    return cast_identifeir;
}

template<type_discriminator Discriminator = type_discriminator::base>
[[nodiscard]] std::shared_ptr<type<Discriminator>> shared_ptr_cast(
    abstract_identifier* identifier
) {
    auto cast_identifeir{raw_ptr_cast<Discriminator>(identifier)};
    return std::shared_ptr<type<Discriminator>>(cast_identifeir);
}

template<type_discriminator Discriminator = type_discriminator::base>
[[nodiscard]] std::shared_ptr<type<Discriminator>> shared_ptr_cast(
    const abstract_identifier* identifier
) {
    const auto cast_identifeir{raw_ptr_cast<Discriminator>(identifier)};
    return std::shared_ptr<type<Discriminator>>(cast_identifeir);
}

} // namespace jftt::identifier
