#pragma once

#include "vm_params.hpp"
#include "vm_register.hpp"

#include <array>



namespace jftt::architecture {

// (simple) singleton class
class vm_memory_manager {
public:
    vm_memory_manager(const vm_memory_manager&) = delete;
    vm_memory_manager(vm_memory_manager&&) = delete;

    vm_memory_manager& operator=(const vm_memory_manager&) = delete;
    vm_memory_manager& operator=(vm_memory_manager&&) = delete;

    ~vm_memory_manager() = default;

    [[nodiscard]] static vm_memory_manager& instance();
    [[nodiscard]] vm_register& get_accumulator();
    [[nodiscard]] bool has_free_registers() const;
    [[nodiscard]] vm_register& get_free_register();
    [[nodiscard]] memory_address_type allocate(const memory_size_type chunk_size);

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
