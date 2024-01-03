#pragma once

#include "architecture/vm_params.hpp"

#include <string>



namespace jftt {

enum class identifier_discriminator : uint8_t {
    variable,
    vararray,
    procedure
};

class abstract_identifier_base {
public:
    abstract_identifier_base() = delete;

    abstract_identifier_base(const abstract_identifier_base&) = default;
    abstract_identifier_base(abstract_identifier_base&&) = default;

    abstract_identifier_base& operator=(const abstract_identifier_base&) = default;
    abstract_identifier_base& operator=(abstract_identifier_base&&) = default;

    virtual ~abstract_identifier_base() = default;

    abstract_identifier_base(identifier_discriminator discriminator, const std::string& name)
        : _discriminator(discriminator), _name(name) {}


    [[nodiscard]] identifier_discriminator discriminator() const noexcept {
        return this->_discriminator;
    }

    [[nodiscard]] bool is_variable() const noexcept {
        return this->_discriminator != identifier_discriminator::procedure;
    }

    [[nodiscard]] const std::string& name() const noexcept {
        return this->_name;
    }

    [[nodiscard]] bool is_used() const noexcept {
        return this->_is_used;
    }

    void set_used() noexcept {
        this->_is_used = true;
    }

protected:
    identifier_discriminator _discriminator;
    std::string _name;
    bool _is_used{false};

    // TODO: size, optional? memory address
};


class variable_identifier : abstract_identifier_base {
public:
    variable_identifier() = delete;

    variable_identifier(const variable_identifier&) = default;
    variable_identifier(variable_identifier&&) = default;

    variable_identifier& operator=(const variable_identifier&) = default;
    variable_identifier& operator=(variable_identifier&&) = default;

    ~variable_identifier() = default;

    variable_identifier(const std::string& name)
        : abstract_identifier_base(identifier_discriminator::variable, name) {}
};

class vararray_identifier : abstract_identifier_base {
    vararray_identifier() = delete;

    vararray_identifier(const vararray_identifier&) = default;
    vararray_identifier(vararray_identifier&&) = default;

    vararray_identifier& operator=(const vararray_identifier&) = default;
    vararray_identifier& operator=(vararray_identifier&&) = default;

    ~vararray_identifier() = default;

    vararray_identifier(const std::string& name)
        : abstract_identifier_base(identifier_discriminator::vararray, name) {}
};

class procedure_identifier : abstract_identifier_base {
    procedure_identifier() = delete;

    procedure_identifier(const procedure_identifier&) = default;
    procedure_identifier(procedure_identifier&&) = default;

    procedure_identifier& operator=(const procedure_identifier&) = default;
    procedure_identifier& operator=(procedure_identifier&&) = default;

    ~procedure_identifier() = default;

    procedure_identifier(const std::string& name)
        : abstract_identifier_base(identifier_discriminator::procedure, name) {}
};

} // namespace jftt
