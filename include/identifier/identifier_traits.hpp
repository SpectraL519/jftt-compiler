#pragma once

#include <cstdint>
#include <iostream>
#include <memory>



namespace jftt::identifier {

enum class type_discriminator : uint8_t {
    base, rvalue, lvalue, variable, vararray, procedure
};



class abstract_identifier;
class abstract_lvalue_identifier;
class variable;
class vararray;
class rvalue;
class procedure;



template <type_discriminator TypeDiscriminator>
struct type_traits {
    using type = abstract_identifier;
};

template <>
struct type_traits<type_discriminator::lvalue> {
    using type = abstract_lvalue_identifier;
};

template <>
struct type_traits<type_discriminator::variable> {
    using type = variable;
};

template <>
struct type_traits<type_discriminator::vararray> {
    using type = vararray;
};

template <>
struct type_traits<type_discriminator::rvalue> {
    using type = rvalue;
};

template <>
struct type_traits<type_discriminator::procedure> {
    using type = procedure;
};

template <type_discriminator TypeDiscriminator>
using type = typename type_traits<TypeDiscriminator>::type;

} // namespace jftt::identifier
