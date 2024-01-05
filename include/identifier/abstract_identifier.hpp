#pragma once

#include "architecture/vm_params.hpp"
#include "identifier/identifier_traits.hpp"

#include <string>



namespace jftt::identifier {

class abstract_identifier {
public:
    abstract_identifier() = default;

    abstract_identifier(const abstract_identifier&) = default;
    abstract_identifier(abstract_identifier&&) = default;

    abstract_identifier& operator=(const abstract_identifier&) = default;
    abstract_identifier& operator=(abstract_identifier&&) = default;

    virtual ~abstract_identifier() = default;

    [[nodiscard]] virtual type_discriminator discriminator() const {
        return this->_discriminator;
    }

    [[nodiscard]] virtual const std::string& name() const {
        return this->_name;
    }

    // TODO: move to abstract_lvalue_identifier, procedure
    [[nodiscard]] virtual bool is_used() const {
        return this->_used;
    }

    // TODO: move to abstract_lvalue_identifier, procedure
    virtual void set_used() {
        this->_used = true;
    }

protected:
    abstract_identifier(type_discriminator discriminator, const std::string& name)
    : _discriminator(discriminator), _name(name) {}

    type_discriminator _discriminator{type_discriminator::base};
    std::string _name;
    bool _used{false};
};

} // namespace jftt::identifier
