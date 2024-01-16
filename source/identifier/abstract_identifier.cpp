#include "identifier/abstract_identifier.hpp"



namespace jftt::identifier {

abstract_identifier::abstract_identifier(
    type_discriminator discriminator, const std::string& name
) : _discriminator(discriminator), _name(name) {}

type_discriminator abstract_identifier::discriminator() const {
    return this->_discriminator;
}

const std::string& abstract_identifier::name() const {
    return this->_name;
}

} // namespace jftt::identifier
