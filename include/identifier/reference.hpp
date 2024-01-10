#pragma once

#include "abstract_lvalue_identifier.hpp"



namespace jftt::identifier {

class reference : public abstract_lvalue_identifier {
public:
    reference(
        const std::string& name,
        const type_discriminator reference_discriminator
    ) : abstract_lvalue_identifier(type_discriminator::reference, name),
        _reference_discriminator(reference_discriminator) {}

    reference(const reference&) = default;
    reference(reference&&) = default;

    reference& operator=(const reference&) = default;
    reference& operator=(reference&&) = default;

    ~reference() = default;

    [[nodiscard]] architecture::memory_size_type size() const override {
        return this->_size;
    }

    [[nodiscard]] type_discriminator reference_discriminator() const {
        return this->_reference_discriminator;
    }

    [[nodiscard]] std::shared_ptr<identifier::abstract_identifier> indexer() const {
        return this->_indexer_identifier;
    }

    [[nodiscard]] const bool initializes_underlying_resource() const {
        return this->_resouce_initialized;
    }

    void set_indexer(const std::shared_ptr<identifier::abstract_identifier>& indexer) {
        if (this->_reference_discriminator != type_discriminator::vararray) {
            std::cerr << "[ERROR] : Cannot set an indexer for a non-vararray reference" << std::endl;
            std::exit(1);
        }

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

    void initialize_resource() {
        this->_resouce_initialized = true;
    }

private:
    static constexpr architecture::memory_size_type _size{1u};
    type_discriminator _reference_discriminator;
    std::shared_ptr<identifier::abstract_identifier> _indexer_identifier;
    bool _resouce_initialized{false};
};

} // namespace jftt::identifier
