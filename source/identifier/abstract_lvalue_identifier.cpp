#include "identifier/abstract_lvalue_identifier.hpp"



namespace jftt::identifier {

abstract_lvalue_identifier::abstract_lvalue_identifier(
    const type_discriminator discriminator, const std::string& name
) : abstract_identifier(discriminator, name) {}

architecture::memory_address_type abstract_lvalue_identifier::address() const {
    return this->_address;
}

void abstract_lvalue_identifier::set_address(
    const architecture::memory_address_type address
) {
    this->_address = address;
}

} // namespace jftt::identifier
