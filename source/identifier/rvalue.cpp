#include "identifier/rvalue.hpp"



namespace jftt::identifier {

rvalue::rvalue(const architecture::value_type value)
: abstract_identifier(type_discriminator::rvalue, _name), _value(value) {}

architecture::value_type rvalue::value() const {
    return this->_value;
}

} // namespace jftt::identifie
