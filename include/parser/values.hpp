#pragma once

#include "../architecture/vm_params.hpp"

#include <string>



namespace jftt::parser {

enum class value_discriminator : uint8_t {
    abstract, rvalue, identifier
};


class abstract_value_base {
public:
    abstract_value_base() = default;
    abstract_value_base(const abstract_value_base&) = default;
    abstract_value_base(abstract_value_base&&) = default;
    abstract_value_base& operator= (const abstract_value_base&) = default;
    abstract_value_base& operator= (abstract_value_base&&) = default;

    ~abstract_value_base() = default;

    abstract_value_base(const value_discriminator discriminator)
    : _discriminator(discriminator) {}

    [[nodiscard]] value_discriminator discriminator() const {
        return this->_discriminator;
    }

protected:
    const value_discriminator _discriminator{value_discriminator::abstract};
};


class rvalue : public abstract_value_base {
public:
    rvalue() = default;

    rvalue(const int value)
    : abstract_value_base(value_discriminator::rvalue), _value(value) {}

    [[nodiscard]] architecture::value_type value() const {
        return this->_value;
    }

private:
    const architecture::value_type _value;
};

[[nodiscard]] inline rvalue* as_rvalue(abstract_value_base* value_ptr) {
    return static_cast<rvalue*>(value_ptr);
}


class identifier : public abstract_value_base {
public:
    identifier() = default;

    identifier(const std::string& name)
    : abstract_value_base(value_discriminator::identifier), _name(name) {}

    [[nodiscard]] const std::string& name() const {
        return this->_name;
    }

private:
    const std::string _name;
};

[[nodiscard]] inline identifier* as_identifier(abstract_value_base* value_ptr) {
    return static_cast<identifier*>(value_ptr);
}

} // namespace jftt::parser
