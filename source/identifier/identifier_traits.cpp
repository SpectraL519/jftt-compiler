#include "identifier/identifier_traits.hpp"



namespace jftt::identifier {

const std::string& as_string(const type_discriminator discriminator) {
    switch (discriminator) {
    case type_discriminator::lvalue:
        return type_traits<type_discriminator::lvalue>::str;
    case type_discriminator::variable:
        return type_traits<type_discriminator::variable>::str;
    case type_discriminator::vararray:
        return type_traits<type_discriminator::vararray>::str;
    case type_discriminator::reference:
        return type_traits<type_discriminator::reference>::str;
    case type_discriminator::procedure:
        return type_traits<type_discriminator::procedure>::str;
    case type_discriminator::rvalue:
        return type_traits<type_discriminator::rvalue>::str;
    default:
        break;
    }
    // avoid reached end of non-void function warnings
    return type_traits<type_discriminator::base>::str;
}

bool is_lvalue(const type_discriminator discriminator) {
    switch (discriminator) {
    case type_discriminator::lvalue:
        return true;
    case type_discriminator::variable:
        return true;
    case type_discriminator::vararray:
        return true;
    case type_discriminator::reference:
        return true;
    case type_discriminator::procedure:
        return false;
    case type_discriminator::rvalue:
        return false;
    default:
        break;
    }
    // avoid reached end of non-void function warnings
    return false;
}

} // namespace jftt::identifier
