#pragma once

#include "identifiers.hpp"
#include "architecture/vm_memory_manager.hpp"

#include <iostream>
#include <memory>
#include <map>



namespace jftt {

class identifier_manager {
public:
    identifier_manager(const identifier_manager&) = delete;
    identifier_manager(identifier_manager&&) = delete;

    identifier_manager& operator=(const identifier_manager&) = delete;
    identifier_manager& operator=(identifier_manager&&) = delete;

    identifier_manager() = default;
    ~identifier_manager() = default;

    void add_variable(const std::string& name) noexcept {
        variable_identifier* variable = new variable_identifier(name);
        variable->set_address(this->_memory_manager.allocate(variable->size()));
        this->_identifiers[name] = std::make_unique<variable_identifier>(*variable);
    }

    void add_vararray(const std::string& name, const architecture::memory_size_type size) noexcept {
        vararray_identifier* vararray = new vararray_identifier(name, size);
        vararray->set_address(this->_memory_manager.allocate(size));
        this->_identifiers[name] = std::make_unique<vararray_identifier>(*vararray);
    }

    void add_procedure(const std::string& name) noexcept {
        this->_identifiers[name] = std::make_unique<procedure_identifier>(name);
    }

    [[nodiscard]] bool has(const std::string& name) const noexcept {
        return this->_identifiers.find(name) != this->_identifiers.cend();
    }

    [[nodiscard]] bool has(
        const std::string& name, const identifier_discriminator discriminator
    ) const noexcept {
        const auto& identifier = this->_identifiers.find(name);
        return identifier != this->_identifiers.end() &&
               identifier->second->discriminator() == discriminator;
    }

    [[nodiscard]] const std::unique_ptr<abstract_identifier_base>& get(
        const std::string& name
    ) const {
        return this->_identifiers.at(name);
    }

    void remove(const std::string& name) noexcept {
        this->_identifiers.erase(name);
    }

private:
    std::map<std::string, std::unique_ptr<abstract_identifier_base>> _identifiers;
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
};

} // namespace jftt
