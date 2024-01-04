#include "architecture/vm_register.hpp"

#include <iostream>



namespace jftt::architecture {

vm_register::vm_register(vm_register_discriminator discriminator)
: _discriminator(discriminator) {}

vm_register_discriminator vm_register::discriminator() const {
    return this->_discriminator;
}

bool vm_register::is_free() const {
    return this->_free;
}

void vm_register::acquire() {
    if (!this->_free) {
        std::cerr << "[ERROR] Register already acquired" << std::endl;
        std::exit(1);
    }
    this->_free = false;
}

void vm_register::release() {
    if (this->_free) {
        std::cerr << "[ERROR] Register already released" << std::endl;
        std::exit(1);
    }
    this->_free = true;
}

bool is_accumulator(const vm_register& reg) {
    return reg.discriminator() == vm_register_discriminator::a;
}

std::string as_string(const vm_register& reg) {
    return std::string(1, 'a' + static_cast<uint8_t>(reg.discriminator()));
}

} // namespace jftt::architecture
