#include "loop/abstract_loop.hpp"



namespace jftt::loop {

abstract_loop::abstract_loop(
    const type_discriminator discriminator,
    const std::string& begin_label,
    const std::string& end_label
) : _discriminator(discriminator), _begin_label(begin_label), _end_label(end_label) {}

const type_discriminator abstract_loop::discriminator() const {
    return this->_discriminator;
}

const std::string& abstract_loop::begin_label() const {
    return this->_begin_label;
}

const std::string& abstract_loop::end_label() const {
    return this->_end_label;
}

void abstract_loop::set_begin_label(const std::string& begin_label) {
    this->_begin_label = begin_label;
}

void abstract_loop::set_end_label(const std::string& end_label) {
    this->_end_label = end_label;
}

} // namespace jftt::loop
