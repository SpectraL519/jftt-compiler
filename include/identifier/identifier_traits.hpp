#pragma once

#include <cstdint>
#include <iostream>
#include <memory>



namespace jftt::identifier {

enum class type_discriminator : uint8_t {
    base, rvalue, lvalue, variable, vararray, reference, procedure
};



class abstract_identifier;
class abstract_lvalue_identifier;
class variable;
class vararray;
class rererence;
class rvalue;
class procedure;



template <type_discriminator TypeDiscriminator>
struct type_traits {
    using type = abstract_identifier;
    inline static const std::string str{"base"};
};

template <>
struct type_traits<type_discriminator::lvalue> {
    using type = abstract_lvalue_identifier;
    inline static const std::string str{"lvalue"};
};

template <>
struct type_traits<type_discriminator::variable> {
    using type = variable;
    inline static const std::string str{"variable"};
};

template <>
struct type_traits<type_discriminator::vararray> {
    using type = vararray;
    inline static const std::string str{"vararray"};
};

template <>
struct type_traits<type_discriminator::reference> {
    using type = reference;
    inline static const std::string str{"reference"};
};

template <>
struct type_traits<type_discriminator::rvalue> {
    using type = rvalue;
    inline static const std::string str{"rvalue"};
};

template <>
struct type_traits<type_discriminator::procedure> {
    using type = procedure;
    inline static const std::string str{"procedure"};
};

template <type_discriminator TypeDiscriminator>
using type = typename type_traits<TypeDiscriminator>::type;

[[nodiscard]] const std::string& as_string(const type_discriminator discriminator);

} // namespace jftt::identifier
