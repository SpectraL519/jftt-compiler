#include "architecture/vm_memory_manager.hpp"

#include <iostream>



namespace jftt::architecture {

vm_memory_manager& vm_memory_manager::instance() {
    static vm_memory_manager instance;
    return instance;
}

vm_register& vm_memory_manager::get_accumulator() {
    return this->_registers[0u];
}

bool vm_memory_manager::has_free_registers() const {
    for (uint8_t i = 1; i < VM_REGISTER_NO; i++)
        if (this->_registers[i].is_free())
            return true;

    return false;
}

vm_register& vm_memory_manager::get_free_register() {
    for (uint8_t i = 1; i < VM_REGISTER_NO; i++)
        if (this->_registers[i].is_free())
            return this->_registers[i];

    std::cerr << "[ERROR] No free registers available" << std::endl;
    std::exit(1);
}

vm_register& vm_memory_manager::acquire_free_register() {
    for (uint8_t i = 1; i < VM_REGISTER_NO; i++) {
        if (this->_registers[i].is_free()) {
            this->_registers[i].acquire();
            return this->_registers[i];
        }
    }

    std::cerr << "[ERROR] No free registers available" << std::endl;
    std::exit(1);
}

memory_address_type vm_memory_manager::allocate(const memory_size_type chunk_size) {
    if (this->_first_free_address + chunk_size > VM_MEMORY_SIZE) {
        std::cerr << "[ERROR] Cannot allocate memory chunk of size "
                    << chunk_size << std::endl;
        std::exit(1);
    }

    const memory_address_type allocated_memory_address = this->_first_free_address;
    this->_first_free_address += chunk_size;
    return allocated_memory_address;
}

} // namespace jftt::architecture
