#include "identifier/variable.hpp"



namespace jftt::identifier {

variable::variable(const std::string& name)
: abstract_lvalue_identifier(type_discriminator::variable, name) {}

architecture::memory_size_type variable::size() const {
    return this->_size;
}

bool variable::is_initialized() const {
    return this->_initialized;
}

void variable::initialize() {
    this->_initialized = true;
}

} // namespace jftt::identifier
