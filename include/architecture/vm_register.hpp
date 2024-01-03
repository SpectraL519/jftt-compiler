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

    [[nodiscard]] vm_register_discriminator discriminator() const noexcept {
        return this->_discriminator;
    }

    [[nodiscard]] bool is_free() const noexcept {
        return this->_free;
    }

    [[nodiscard]] bool acquire() noexcept {
        // maybe throw exception ?
        if (!this->_free)
            return false;

        this->_free = false;
        return true;
    }

    [[nodiscard]] bool release() noexcept {
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

} // namespace jftt::architecture
