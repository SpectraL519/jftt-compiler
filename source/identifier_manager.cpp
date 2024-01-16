#include "identifier_manager.hpp"



namespace jftt {

bool identifier_manager::has(const std::string& name) const {
    return this->_identifiers.find(name) != this->_identifiers.cend();
}

[[nodiscard]] bool identifier_manager::has(
    const std::string& name, const identifier_discriminator discriminator
) const {
    const auto& identifier = this->_identifiers.find(name);
    return identifier != this->_identifiers.end() &&
            identifier->second->discriminator() == discriminator;
}

void identifier_manager::initialize_lvalue_identifier(const std::string& name) {
    this->get<identifier_discriminator::lvalue>(name)->initialize();
}

const std::shared_ptr<identifier::abstract_identifier>& identifier_manager::get(
    const std::string& name
) {
    return this->_identifiers.at(name);
}

void identifier_manager::remove(const std::string& name) {
    this->_identifiers.erase(name);
}

} // namespace jftt
