#pragma once

#include "abstract_identifier.hpp"
#include "identifier_cast.hpp"
#include "../architecture/vm_params.hpp"

#include <algorithm>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>



namespace jftt::identifier {

struct procedure_paramter {
    procedure_paramter() = delete;

    procedure_paramter(
        const type_discriminator declared_discriminator,
        const std::string local_name,
        const std::shared_ptr<abstract_lvalue_identifier>& reference
    ) : declared_discriminator(declared_discriminator),
        local_name(local_name),
        reference(reference)
    {}

    procedure_paramter(const procedure_paramter&) = default;
    procedure_paramter(procedure_paramter&&) = default;

    procedure_paramter& operator=(const procedure_paramter&) = default;
    procedure_paramter& operator=(procedure_paramter&&) = default;

    ~procedure_paramter() = default;

    type_discriminator declared_discriminator;
    std::string local_name;
    std::shared_ptr<abstract_lvalue_identifier> reference;
};



class procedure : public abstract_identifier {
public:
    procedure(
        const std::string& name,
        const jftt::architecture::memory_address_type return_point_address
    ) : abstract_identifier(type_discriminator::procedure, name),
        _return_point_address(return_point_address)
    {}

    procedure(const procedure&) = default;
    procedure(procedure&&) = default;

    procedure& operator=(const procedure&) = default;
    procedure& operator=(procedure&&) = default;

    ~procedure() = default;

    std::optional<std::string> declare_parameter(
        const std::string& name, const type_discriminator discriminator
    ) {
        if (this->has_identifier(name))
            return "Identifier `" + name + "` already defined in function `" + this->_name + "`";

        this->_assert_valid_param_discriminator(discriminator);
        this->_local_identifiers.emplace_back(discriminator, name, nullptr);
        this->_param_no++;
        return std::nullopt;
    }

    std::optional<std::string> set_parameter_reference(
        std::shared_ptr<abstract_lvalue_identifier> reference
    ) {
        // returns optional error msg
        this->_call_param_idx++;
        if (this->_call_param_idx > this->_param_no)
            return "To many function call parameters";

        auto& call_param{this->_local_identifiers.at(this->_call_param_idx - 1)};
        if (reference->discriminator() != call_param.declared_discriminator)
            return "Invalid parameter type. Expected: " +
                   as_string(call_param.declared_discriminator) +
                   ". Got: " + as_string(reference->discriminator());

        call_param.reference = reference;
        return std::nullopt;
    }

    std::optional<std::string> finish_parameter_passing() {
        // returns optional error msg
        if (this->_call_param_idx < this->_param_no)
            return "Not enough function call parameters";

        this->_call_param_idx = 0u;
        return std::nullopt;
    }

    std::optional<std::string> declare_lvalue_identifier(
        std::shared_ptr<identifier::abstract_lvalue_identifier> lvalue
    ) {
        if (this->has_identifier(lvalue->name()))
            return "Identifier `" + lvalue->name() +
                   "` already defined in function `" + this->_name + "`";

        this->_local_identifiers.emplace_back(
            lvalue->discriminator(), lvalue->name(), std::move(lvalue));
        return std::nullopt;
    }

    [[nodiscard]] bool has_identifier(const std::string& name) const {
        return this->_cfind_param(name) != this->_local_identifiers.end();
    }

    [[nodiscard]] bool has_identifier(
        const std::string& name, const type_discriminator discriminator
    ) const {
        auto param_it{this->_cfind_param(name)};
        return param_it != this->_local_identifiers.end() &&
               param_it->declared_discriminator == discriminator;
    }

    [[nodiscard]] const std::shared_ptr<identifier::abstract_lvalue_identifier>& get_identifier(
        const std::string& name
    ) {
        return this->_find_param(name)->reference;
    }

    // ? not necessary
    template <type_discriminator IdentifierDiscriminator>
    [[nodiscard]] std::shared_ptr<identifier::type<IdentifierDiscriminator>> get_identifier(
        const std::string& name
    ) {
        return identifier::shared_ptr_cast<IdentifierDiscriminator>(
            this->_find_param(name)->reference);
    }

    void set_begin_label(const std::string& begin_label) {
        this->_begin_label = begin_label;
    }

    [[nodiscard]] const std::string& begin_label() const {
        return this->_begin_label;
    }

    [[nodiscard]] const jftt::architecture::memory_address_type return_point_address() const {
        return this->_return_point_address;
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

    [[nodiscard]] std::vector<procedure_paramter>::iterator _find_param(const std::string& name) {
        return std::find_if(
            this->_local_identifiers.begin(), this->_local_identifiers.end(),
            [&name] (const auto& param) { return param.local_name == name; }
        );
    }

    [[nodiscard]] std::vector<procedure_paramter>::const_iterator _cfind_param(
        const std::string& name
    ) const {
        return std::find_if(
            this->_local_identifiers.cbegin(), this->_local_identifiers.cend(),
            [&name] (const auto& param) { return param.local_name == name; }
        );
    }

    std::vector<procedure_paramter> _local_identifiers;
    std::size_t _param_no{0u};
    std::size_t _call_param_idx{0u};

    std::string _begin_label;
    const jftt::architecture::memory_address_type _return_point_address;
};

} // namespace jftt::identifier
