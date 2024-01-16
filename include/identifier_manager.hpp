#pragma once

#include "identifier.hpp"
#include "architecture/vm_memory_manager.hpp"

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

    template <identifier_discriminator IdentifierDiscriminator>
    void add(std::shared_ptr<identifier::type<IdentifierDiscriminator>> identifier) {
        this->_identifiers[identifier->name()] = std::move(identifier);
    }

    [[nodiscard]] bool has(const std::string& name) const;
    [[nodiscard]] bool has(
        const std::string& name, const identifier_discriminator discriminator) const;

    void initialize_lvalue_identifier(const std::string& name);

    [[nodiscard]] const std::shared_ptr<identifier::abstract_identifier>& get(const std::string& name);

    template <identifier_discriminator IdentifierDiscriminator>
    [[nodiscard]] std::shared_ptr<identifier::type<IdentifierDiscriminator>> get(
        const std::string& name
    ) const {
        return identifier::shared_ptr_cast<IdentifierDiscriminator>(this->_identifiers.at(name));
    }

    void remove(const std::string& name);

private:
    std::map<std::string, std::shared_ptr<identifier::abstract_identifier>> _identifiers;
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
};

} // namespace jftt
