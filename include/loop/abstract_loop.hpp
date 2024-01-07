#pragma once

#include <string>



namespace jftt::loop {

enum class type_discriminator : bool {
    while_do, repeat_until
};


class abstract_loop {
public:
    abstract_loop(
        const type_discriminator discriminator,
        const std::string& begin_label,
        const std::string& end_label
    ) : _discriminator(discriminator), _begin_label(begin_label), _end_label(end_label) {}

    abstract_loop(const abstract_loop&) = default;
    abstract_loop(abstract_loop&&) = default;

    abstract_loop& operator=(const abstract_loop&) = default;
    abstract_loop& operator=(abstract_loop&&) = default;

    virtual ~abstract_loop() = default;

    [[nodiscard]] const type_discriminator discriminator() const {
        return this->_discriminator;
    }

    [[nodiscard]] const std::string& begin_label() const {
        return this->_begin_label;
    }

    [[nodiscard]] const std::string& end_label() const {
        return this->_end_label;
    }

    void set_begin_label(const std::string& begin_label) {
        this->_begin_label = begin_label;
    }

    void set_end_label(const std::string& end_label) {
        this->_end_label = end_label;
    }

private:
    type_discriminator _discriminator;
    std::string _begin_label;
    std::string _end_label;
};

} // namespace jftt::loop
