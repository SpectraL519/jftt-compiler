#pragma once

#include "vm_params.hpp"



namespace jftt::architecture {

class vm_register {
public:
    vm_register(const vm_register&) = delete;
    vm_register(vm_register&&) = delete;

    vm_register& operator=(const vm_register&) = delete;
    vm_register& operator=(vm_register&&) = delete;

    vm_register(vm_register_discriminator discriminator)
    : _discriminator(discriminator) {}

    [[nodiscard]] vm_register_discriminator discriminator() const {
        return this->_discriminator;
    }

    // TODO: name

    [[nodiscard]] bool is_free() const {
        return this->_free;
    }

    [[nodiscard]] bool acquire() {
        // maybe throw exception ?
        if (!this->_free)
            return false;

        this->_free = false;
        return true;
    }

    [[nodiscard]] bool release() {
        // maybe throw exception
        if (this->_free)
            return false;

        this->_free = true;
        return true;
    }

private:
    vm_register_discriminator _discriminator;
    bool _free;
};


[[nodiscard]] bool is_accumulator(const vm_register& reg) {
    return reg.discriminator() == vm_register_discriminator::a;
}

} // namespace jftt::architecture
