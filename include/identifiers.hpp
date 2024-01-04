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

    [[nodiscard]] identifier_discriminator discriminator() const;
    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] bool is_used() const;
    void set_used();

    virtual architecture::memory_address_type address() const = 0;
    virtual architecture::memory_size_type size() const = 0;

    virtual void set_address(const architecture::memory_address_type) = 0;

protected:
    abstract_identifier_base(identifier_discriminator discriminator, const std::string& name);

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

    variable_identifier(const std::string& name);

    [[nodiscard]] architecture::memory_address_type address() const override;
    [[nodiscard]] architecture::memory_size_type size() const override;

    void set_address(const architecture::memory_address_type address) override;

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

    vararray_identifier(const std::string& name, const architecture::memory_size_type size);

    [[nodiscard]] architecture::memory_address_type address() const override;
    [[nodiscard]] architecture::memory_size_type size() const override;

    void set_address(const architecture::memory_address_type address) override;

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

    procedure_identifier(const std::string& name);

    architecture::memory_address_type address() const override;
    architecture::memory_size_type size() const override;

    void set_address(const architecture::memory_address_type) override;
};

} // namespace jftt
