#pragma once

#include "abstract_lvalue_identifier.hpp"



namespace jftt::identifier {

class variable : public abstract_lvalue_identifier {
public:
    variable(const std::string& name)
    : abstract_lvalue_identifier(type_discriminator::variable, name) {}

    variable(const variable&) = default;
    variable(variable&&) = default;

    variable& operator=(const variable&) = default;
    variable& operator=(variable&&) = default;

    ~variable() = default;

    [[nodiscard]] architecture::memory_size_type size() const override {
        return this->_size;
    }

private:
    static constexpr architecture::memory_size_type _size{1u};
};

} // namespace jftt::identifier