#include "identifier/procedure.hpp"

#include <algorithm>



namespace jftt::identifier {

procedure::procedure(
    const std::string& name,
    const jftt::architecture::memory_address_type return_point_address
) : abstract_identifier(type_discriminator::procedure, name),
    _return_point_address(return_point_address)
{}

std::variant<std::string, std::shared_ptr<reference>> procedure::declare_parameter(
    const std::string& name, const type_discriminator discriminator
) {
    // returns either reference identifier ptr or a string containing an error msg
    if (this->has_identifier(name))
        return "Identifier `" + name + "` already defined in procedure `" + this->_name + "`";

    this->_assert_valid_param_discriminator(discriminator);

    auto ref{std::make_shared<reference>(name, discriminator)};
    this->_local_identifiers.push_back(ref);
    this->_param_no++;

    return ref;
}

std::shared_ptr<reference> procedure::get_next_parameter() {
    // returns either reference identifier ptr or a nullptr if all parameters have been set
    this->_call_param_idx++;
    if (this->_call_param_idx > this->_param_no)
        return nullptr;

    return shared_ptr_cast<type_discriminator::reference>(
        this->_local_identifiers.at(this->_call_param_idx - 1));
}

std::optional<std::string> procedure::finish_parameter_passing() {
    // returns optional error msg
    if (this->_call_param_idx < this->_param_no)
        return "Not enough function call parameters";

    this->_call_param_idx = 0u;
    return std::nullopt;
}

std::optional<std::string> procedure::declare_local_identifier(
    std::shared_ptr<identifier::abstract_lvalue_identifier> lvalue
) {
    if (this->has_identifier(lvalue->name()))
        return "Identifier `" + lvalue->name() +
                "` already defined in function `" + this->_name + "`";

    this->_local_identifiers.push_back(std::move(lvalue));
    return std::nullopt;
}

bool procedure::has_identifier(const std::string& name) const {
    return this->_cfind_param(name) != this->_local_identifiers.end();
}

bool procedure::has_identifier(
    const std::string& name, const type_discriminator discriminator
) const {
    auto identifier_it{this->_cfind_param(name)};
    if (identifier_it == this->_local_identifiers.end())
        return false;

    const auto& identifier{*identifier_it};
    if (identifier->discriminator() == type_discriminator::reference) {
        const auto reference{shared_ptr_cast<type_discriminator::reference>(identifier)};
        return discriminator == reference->reference_discriminator();
    }

    return discriminator == identifier->discriminator();
}

std::shared_ptr<identifier::abstract_lvalue_identifier> procedure::get_identifier(
    const std::string& name
) {
    auto identifier_it{this->_find_param(name)};
    if (identifier_it == this->_local_identifiers.end()) {
        std::cerr << "[ERROR] : Procedure `" << this->_name << "` has no identifier `"
                    << name << "`" << std::endl;
        std::exit(1);
    }

    return *identifier_it;
}

void procedure::set_begin_label(const std::string& begin_label) {
    this->_begin_label = begin_label;
}

const std::string& procedure::begin_label() const {
    return this->_begin_label;
}

const jftt::architecture::memory_address_type procedure::return_point_address() const {
    return this->_return_point_address;
}

void procedure::_assert_valid_param_discriminator(const type_discriminator discriminator) const {
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

std::vector<std::shared_ptr<abstract_lvalue_identifier>>::iterator procedure::_find_param(
    const std::string& name
) {
    return std::find_if(
        this->_local_identifiers.begin(), this->_local_identifiers.end(),
        [&name] (const auto& identifier) { return identifier->name() == name; }
    );
}

std::vector<std::shared_ptr<abstract_lvalue_identifier>>::const_iterator procedure::_cfind_param(
    const std::string& name
) const {
    return std::find_if(
        this->_local_identifiers.cbegin(), this->_local_identifiers.cend(),
        [&name] (const auto& identifier) { return identifier->name() == name; }
    );
}

} // namespace jftt::identifier
