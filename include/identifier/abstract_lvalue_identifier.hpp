#pragma once

#include "abstract_identifier.hpp"



namespace jftt::identifier {

class abstract_lvalue_identifier : public abstract_identifier {
public:
    abstract_lvalue_identifier()
    : abstract_identifier(type_discriminator::lvalue, _abstract_name) {}

    abstract_lvalue_identifier(const abstract_lvalue_identifier&) = default;
    abstract_lvalue_identifier(abstract_lvalue_identifier&&) = default;

    abstract_lvalue_identifier& operator=(const abstract_lvalue_identifier&) = default;
    abstract_lvalue_identifier& operator=(abstract_lvalue_identifier&&) = default;

    ~abstract_lvalue_identifier() = default;

    [[nodiscard]] architecture::memory_address_type address() const;
    void set_address(const architecture::memory_address_type address);

    virtual architecture::memory_size_type size() const = 0;
    virtual bool is_initialized() const = 0;
    virtual void initialize() = 0;

protected:
    abstract_lvalue_identifier(
        const type_discriminator discriminator, const std::string& name);

    architecture::memory_address_type _address;
    bool _initialized{false};

private:
    inline static const std::string _abstract_name{"lvalue"};
};

} // namespace jftt::identifier
