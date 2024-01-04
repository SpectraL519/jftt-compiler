#include "identifiers.hpp"

#include "architecture/vm_params.hpp"

#include <iostream>



namespace jftt {

identifier_discriminator abstract_identifier_base::discriminator() const {
    return this->_discriminator;
}

const std::string& abstract_identifier_base::name() const {
    return this->_name;
}

bool abstract_identifier_base::is_used() const {
    return this->_used;
}

void abstract_identifier_base::set_used() {
    this->_used = true;
}

abstract_identifier_base::abstract_identifier_base(
    identifier_discriminator discriminator, const std::string& name
) : _discriminator(discriminator), _name(name) {}


variable_identifier::variable_identifier(const std::string& name)
: abstract_identifier_base(identifier_discriminator::variable, name) {}

architecture::memory_address_type variable_identifier::address() const {
    return this->_address;
}

architecture::memory_size_type variable_identifier::size() const {
    return this->_size;
}

void variable_identifier::set_address(const architecture::memory_address_type address) {
    this->_address = address;
}


vararray_identifier::vararray_identifier(
    const std::string& name, const architecture::memory_size_type size
) : abstract_identifier_base(identifier_discriminator::vararray, name), _size(size) {}

architecture::memory_address_type vararray_identifier::address() const {
    return this->_address;
}

architecture::memory_size_type vararray_identifier::size() const {
    return this->_size;
}

void vararray_identifier::set_address(const architecture::memory_address_type address) {
    this->_address = address;
}


procedure_identifier::procedure_identifier(const std::string& name)
: abstract_identifier_base(identifier_discriminator::procedure, name) {}

architecture::memory_address_type procedure_identifier::address() const {
    std::cerr << "[ERROR] Cannot get memory address of a procedure identifier: "
                << this->_name << std::endl;
    std::exit(1);
}

architecture::memory_size_type procedure_identifier::size() const {
    std::cerr << "[ERROR] Cannot assign get memory size of a procedure identifier: "
                << this->_name << std::endl;
    std::exit(1);
}

void procedure_identifier::set_address(const architecture::memory_address_type) {
    std::cerr << "[ERROR] Cannot set memory address of a procedure identifier: "
                << this->_name << std::endl;
    std::exit(1);
}

} // namespace jftt
