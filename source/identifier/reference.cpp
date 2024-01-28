#include "identifier/reference.hpp"



namespace jftt::identifier {

reference::reference(
    const std::string& name,
    const type_discriminator reference_discriminator
) : abstract_lvalue_identifier(type_discriminator::reference, name),
    _reference_discriminator(reference_discriminator) {}

architecture::memory_size_type reference::size() const {
    return this->_size;
}

type_discriminator reference::reference_discriminator() const {
    return this->_reference_discriminator;
}

std::shared_ptr<identifier::abstract_identifier> reference::indexer() const {
    return this->_indexer_identifier;
}

bool reference::is_initialized() const {
    return this->_initialized;
}

const bool reference::initializes_underlying_resource() const {
    return this->_resouce_initialized;
}

void reference::set_indexer(const std::shared_ptr<identifier::abstract_identifier>& indexer) {
    if (this->_reference_discriminator != type_discriminator::vararray) {
        std::cerr << "[ERROR] : Cannot set an indexer for a non-vararray reference" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    switch (indexer->discriminator()) {
    case type_discriminator::rvalue:
        break; // valid indexer
    case type_discriminator::variable:
        break; // valid indexer
    case type_discriminator::reference:
        break; // valid indexer
    default:
        std::cerr << "[ERROR] : Invalid array indexer type" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    this->_indexer_identifier = indexer;
}

void reference::initialize() {}

void reference::initialize_resource() {
    this->_resouce_initialized = true;
}

} // namespace jftt::identifier
