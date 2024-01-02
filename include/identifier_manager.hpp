#pragma once

#include "identifiers.hpp"

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

    void add(abstract_identifier_base&& identifier) noexcept {
        this->_identifiers[identifier.name()] =
            std::make_unique<abstract_identifier_base>(std::move(identifier));
    }

    [[nodiscard]] bool has(const std::string& name) const noexcept {
        return this->_identifiers.find(name) != this->_identifiers.cend();
    }

    [[nodiscard]] bool has_variable(const std::string& name) const noexcept {
        const auto& identifier = this->_identifiers.find(name);
        return identifier != this->_identifiers.end() && identifier->second->is_variable();
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
};

} // namespace jftt
