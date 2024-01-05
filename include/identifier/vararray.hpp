#pragma once

#include "abstract_lvalue_identifier.hpp"



namespace jftt::identifier {

class vararray : public abstract_lvalue_identifier {
public:
    vararray(const std::string& name, const architecture::memory_size_type size)
    : abstract_lvalue_identifier(type_discriminator::vararray, name), _size(size) {}

    vararray(const vararray&) = default;
    vararray(vararray&&) = default;

    vararray& operator=(const vararray&) = default;
    vararray& operator=(vararray&&) = default;

    ~vararray() = default;

    [[nodiscard]] architecture::memory_size_type size() const override {
        return this->_size;
    }

private:
    const architecture::memory_size_type _size{1u};
};

} // namespace jftt::identifier
