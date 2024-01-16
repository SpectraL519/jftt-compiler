#include "identifier/vararray.hpp"

#include <iostream>



namespace jftt::identifier {

vararray::vararray(const std::string& name, const architecture::memory_size_type size)
: abstract_lvalue_identifier(type_discriminator::vararray, name), _size(size) {}

vararray::vararray(const std::string& name)
: abstract_lvalue_identifier(type_discriminator::vararray, name) {}

architecture::memory_size_type vararray::size() const {
    return this->_size;
}

std::shared_ptr<identifier::abstract_identifier> vararray::indexer() const {
    return this->_indexer_identifier;
}

void vararray::set_indexer(const std::shared_ptr<identifier::abstract_identifier>& indexer) {
    switch (indexer->discriminator()) {
    case type_discriminator::rvalue:
        break; // valid indexer
    case type_discriminator::variable:
        break; // valid indexer
    case type_discriminator::reference:
        break; // valid indexer
    default:
        std::cerr << "[ERROR] : Invalid array indexer type" << std::endl;
        std::exit(1);
    }

    this->_indexer_identifier = indexer;
}

bool vararray::is_initialized() const {
    return this->_initialized;
}

void vararray::initialize() {}

} // namespace jftt::identifier
