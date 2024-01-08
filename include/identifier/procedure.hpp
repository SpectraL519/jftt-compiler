#pragma once

#include "abstract_identifier.hpp"
#include "identifier_cast.hpp"

#include <map>
#include <memory>
#include <string>



namespace jftt::identifier {

struct procedure_param {
    type_discriminator declared_discriminator;
    std::shared_ptr<abstract_lvalue_identifier> reference;
};

// ? TODO: should derive from abstract_lvalue_identifier
//         |-> functions are lvalues
class procedure : public abstract_identifier {
public:
    procedure(const std::string& name)
    : abstract_identifier(type_discriminator::procedure, name) {}

    procedure(const procedure&) = default;
    procedure(procedure&&) = default;

    procedure& operator=(const procedure&) = default;
    procedure& operator=(procedure&&) = default;

    ~procedure() = default;

    void add_param(const std::string& name, const type_discriminator discriminator) {
        this->_assert_valid_param_discriminator(discriminator);
        this->_params[name] = procedure_param{discriminator, nullptr};
    }

    void set_param_reference(
        const std::string& name,
        const std::shared_ptr<abstract_lvalue_identifier>& reference
    ) {
        this->_params.at(name).reference = reference;
    }

    void set_begin_label(const std::string& begin_label) {
        this->_begin_label = begin_label;
    }

    void set_end_label(const std::string& end_label) {
        this->_end_label = end_label;
    }

    [[nodiscard]] bool has_param(const std::string& name) const {
        return this->_params.find(name) != this->_params.end();
    }

    [[nodiscard]] bool has_param(
        const std::string& name, const type_discriminator discriminator
    ) const {
        const auto& param = this->_params.find(name);
        return param != this->_params.end() &&
               param->second.declared_discriminator == discriminator;
    }

    [[nodiscard]] const std::shared_ptr<identifier::abstract_lvalue_identifier>& get_param_reference(
        const std::string& name
    ) {
        return this->_params.at(name).reference;
    }

    // ? not necessary
    template <type_discriminator IdentifierDiscriminator>
    [[nodiscard]] std::shared_ptr<identifier::type<IdentifierDiscriminator>> get_param_reference(
        const std::string& name
    ) const {
        return identifier::shared_ptr_cast<IdentifierDiscriminator>(
            this->_params.at(name).reference);
    }

    [[nodiscard]] const std::string& begin_label() const {
        return this->_begin_label;
    }

    [[nodiscard]] const std::string& end_label() const {
        return this->_end_label;
    }

private:
    void _assert_valid_param_discriminator(const type_discriminator discriminator) const {
        switch (discriminator) {
        case type_discriminator::variable:
            break; // valid
        case type_discriminator::vararray:
            break; // valid

        default:
            std::cerr << "[ERROR] : Invalid procedure param discriminator - must be an lvalue"
                      << std::endl;
            std::exit(1);
        }
    }

    std::map<std::string, procedure_param> _params;
    std::string _begin_label;
    std::string _end_label;
};

} // namespace jftt::identifier
