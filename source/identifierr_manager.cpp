#include "identifier_manager.hpp"



namespace jftt {

void identifier_manager::add_variable(const std::string& name) {
    variable_identifier* variable = new variable_identifier(name);
    variable->set_address(this->_memory_manager.allocate(variable->size()));
    this->_identifiers[name] = std::make_unique<variable_identifier>(*variable);
}

void identifier_manager::add_vararray(
    const std::string& name, const architecture::memory_size_type size
) {
    vararray_identifier* vararray = new vararray_identifier(name, size);
    vararray->set_address(this->_memory_manager.allocate(vararray->size()));
    this->_identifiers[name] = std::make_unique<vararray_identifier>(*vararray);
}

void identifier_manager::add_procedure(const std::string& name) {
    this->_identifiers[name] = std::make_unique<procedure_identifier>(name);
}

bool identifier_manager::has(const std::string& name) const {
    return this->_identifiers.find(name) != this->_identifiers.cend();
}

bool identifier_manager::has(
    const std::string& name, const identifier_discriminator discriminator
) const {
    const auto& identifier = this->_identifiers.find(name);
    return identifier != this->_identifiers.end() &&
            identifier->second->discriminator() == discriminator;
}

const std::unique_ptr<abstract_identifier_base>& identifier_manager::get(
    const std::string& name
) const {
    return this->_identifiers.at(name);
}

void identifier_manager::remove(const std::string& name) {
    this->_identifiers.erase(name);
}

} // namespace jftt
