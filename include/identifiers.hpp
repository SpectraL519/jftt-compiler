#pragma once

#include "architecture/vm_params.hpp"

#include <iostream>
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

    [[nodiscard]] identifier_discriminator discriminator() const noexcept {
        return this->_discriminator;
    }

    [[nodiscard]] const std::string& name() const noexcept {
        return this->_name;
    }

    [[nodiscard]] bool is_used() const noexcept {
        return this->_used;
    }

    void set_used() noexcept {
        this->_used = true;
    }

    virtual architecture::memory_address_type address() const noexcept = 0;
    virtual architecture::memory_size_type size() const noexcept = 0;

    virtual void set_address(const architecture::memory_address_type) noexcept = 0;

protected:
    abstract_identifier_base(identifier_discriminator discriminator, const std::string& name)
    : _discriminator(discriminator), _name(name) {}

    identifier_discriminator _discriminator;
    std::string _name;
    bool _used{false};
};


class variable_identifier : public abstract_identifier_base {
public:
    variable_identifier() = delete;

    variable_identifier(const variable_identifier&) = default;
    variable_identifier(variable_identifier&&) = default;

    variable_identifier& operator=(const variable_identifier&) = default;
    variable_identifier& operator=(variable_identifier&&) = default;

    ~variable_identifier() = default;

    variable_identifier(const std::string& name)
    : abstract_identifier_base(identifier_discriminator::variable, name) {}

    architecture::memory_address_type address() const noexcept override {
        return this->_address;
    }

    architecture::memory_size_type size() const noexcept override {
        return this->_size;
    }

    void set_address(const architecture::memory_address_type address) noexcept override {
        this->_address = address;
    }

private:
    architecture::memory_address_type _address;
    static constexpr architecture::memory_size_type _size{1u};
};


class vararray_identifier : public abstract_identifier_base {
public:
    vararray_identifier() = delete;

    vararray_identifier(const vararray_identifier&) = default;
    vararray_identifier(vararray_identifier&&) = default;

    vararray_identifier& operator=(const vararray_identifier&) = default;
    vararray_identifier& operator=(vararray_identifier&&) = default;

    ~vararray_identifier() = default;

    vararray_identifier(const std::string& name, const architecture::memory_size_type size)
    : abstract_identifier_base(identifier_discriminator::vararray, name), _size(size) {}

    architecture::memory_address_type address() const noexcept override {
        return this->_address;
    }

    architecture::memory_size_type size() const noexcept override {
        return this->_size;
    }

    void set_address(const architecture::memory_address_type address) noexcept override {
        this->_address = address;
    }

private:
    architecture::memory_address_type _address;
    const architecture::memory_size_type _size;
};


class procedure_identifier : public abstract_identifier_base {
public:
    procedure_identifier() = delete;

    procedure_identifier(const procedure_identifier&) = default;
    procedure_identifier(procedure_identifier&&) = default;

    procedure_identifier& operator=(const procedure_identifier&) = default;
    procedure_identifier& operator=(procedure_identifier&&) = default;

    ~procedure_identifier() = default;

    procedure_identifier(const std::string& name)
    : abstract_identifier_base(identifier_discriminator::procedure, name) {}

    architecture::memory_address_type address() const noexcept override {
        std::cerr << "[ERROR] Cannot get memory address of a procedure identifier: "
                  << this->_name << std::endl;
        std::exit(1);
    }

    architecture::memory_size_type size() const noexcept override {
        std::cerr << "[ERROR] Cannot assign get memory size of a procedure identifier: "
                  << this->_name << std::endl;
        std::exit(1);
    }

    void set_address(const architecture::memory_address_type) noexcept override {
        std::cerr << "[ERROR] Cannot set memory address of a procedure identifier: "
                  << this->_name << std::endl;
        std::exit(1);
    }
};

} // namespace jftt
