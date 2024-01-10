#pragma once

#include "abstract_lvalue_identifier.hpp"

#include <iostream>



namespace jftt::identifier {

class vararray : public abstract_lvalue_identifier {
public:
    // static size declaration
    vararray(const std::string& name, const architecture::memory_size_type size)
    : abstract_lvalue_identifier(type_discriminator::vararray, name), _size(size) {}

    // dynamic size declaration - for procedures
    vararray(const std::string& name)
    : abstract_lvalue_identifier(type_discriminator::vararray, name) {}

    vararray(const vararray&) = default;
    vararray(vararray&&) = default;

    vararray& operator=(const vararray&) = default;
    vararray& operator=(vararray&&) = default;

    ~vararray() = default;

    [[nodiscard]] architecture::memory_size_type size() const override {
        return this->_size;
    }

    [[nodiscard]] std::shared_ptr<identifier::abstract_identifier> indexer() const {
        return this->_indexer_identifier;
    }

    void set_indexer(const std::shared_ptr<identifier::abstract_identifier>& indexer) {
        switch (indexer->discriminator()) {
        case type_discriminator::rvalue:
            break; // valid indexer
        case type_discriminator::variable:
            break; // valid indexer
        default:
            std::cerr << "[ERROR] : Invalid array indexer type" << std::endl;
            std::exit(1);
        }

        this->_indexer_identifier = indexer;
    }

private:
    architecture::memory_size_type _size{1u};
    std::shared_ptr<identifier::abstract_identifier> _indexer_identifier;
};

} // namespace jftt::identifier
