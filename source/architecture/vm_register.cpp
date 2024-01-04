#include "architecture/vm_register.hpp"



namespace jftt::architecture {

vm_register::vm_register(vm_register_discriminator discriminator)
: _discriminator(discriminator) {}

vm_register_discriminator vm_register::discriminator() const {
    return this->_discriminator;
}

bool vm_register::is_free() const {
    return this->_free;
}

bool vm_register::acquire() {
    // maybe throw exception ?
    if (!this->_free)
        return false;

    this->_free = false;
    return true;
}

bool vm_register::release() {
    // maybe throw exception
    if (this->_free)
        return false;

    this->_free = true;
    return true;
}

bool is_accumulator(const vm_register& reg) {
    return reg.discriminator() == vm_register_discriminator::a;
}

std::string as_string(const vm_register& reg) {
    return std::string(1, 'a' + static_cast<uint8_t>(reg.discriminator()));
}

} // namespace jftt::architecture
