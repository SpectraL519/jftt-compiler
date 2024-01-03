#pragma once

#include "vm_params.hpp"
#include "vm_register.hpp"

#include <array>
#include <iostream>



namespace jftt::architecture {

// (simple) singleton class
class vm_memory_manager {
public:
    vm_memory_manager(const vm_memory_manager&) = delete;
    vm_memory_manager(vm_memory_manager&&) = delete;

    vm_memory_manager& operator=(const vm_memory_manager&) = delete;
    vm_memory_manager& operator=(vm_memory_manager&&) = delete;

    ~vm_memory_manager() = default;

    [[nodiscard]] static vm_memory_manager& instance() noexcept {
        static vm_memory_manager instance;
        return instance;
    }

    [[nodiscard]] vm_register& get_accumulator() noexcept {
        return this->_registers[0u];
    }

    [[nodiscard]] bool has_free_registers() const noexcept {
        for (uint8_t i = 1; i < VM_REGISTER_NO; i++)
            if (this->_registers[i].is_free())
                return true;

        return false;
    }

    [[nodiscard]] vm_register& get_free_register() noexcept {
        for (uint8_t i = 1; i < VM_REGISTER_NO; i++)
            if (this->_registers[i].is_free())
                return this->_registers[i];

        std::cerr << "[ERROR] No free registers available" << std::endl;
        std::exit(1);
    }

    [[nodiscard]] memory_address_type allocate(memory_address_type chunk_size) noexcept {
        if (this->_first_free_address + chunk_size > VM_MEMORY_SIZE) {
            std::cerr << "[ERROR] Cannot allocate memory chunk of size "
                      << chunk_size << std::endl;
            std::exit(1);
        }

        const memory_address_type allocated_memory_address = this->_first_free_address;
        this->_first_free_address += chunk_size;
        return allocated_memory_address;
    }

private:
    vm_memory_manager() = default;

    std::array<vm_register, VM_REGISTER_NO> _registers{
        vm_register(vm_register_discriminator::a),
        vm_register(vm_register_discriminator::b),
        vm_register(vm_register_discriminator::c),
        vm_register(vm_register_discriminator::d),
        vm_register(vm_register_discriminator::e),
        vm_register(vm_register_discriminator::f),
        vm_register(vm_register_discriminator::g),
        vm_register(vm_register_discriminator::h)
    };

    memory_address_type _first_free_address{0ull};
};

} // namespace jftt::architecture
