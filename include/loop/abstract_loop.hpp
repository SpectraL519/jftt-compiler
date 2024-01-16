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
        const std::string& end_label);

    abstract_loop(const abstract_loop&) = default;
    abstract_loop(abstract_loop&&) = default;

    abstract_loop& operator=(const abstract_loop&) = default;
    abstract_loop& operator=(abstract_loop&&) = default;

    virtual ~abstract_loop() = default;

    [[nodiscard]] const type_discriminator discriminator() const;

    [[nodiscard]] const std::string& begin_label() const;
    [[nodiscard]] const std::string& end_label() const;

    void set_begin_label(const std::string& begin_label);
    void set_end_label(const std::string& end_label);

private:
    type_discriminator _discriminator;
    std::string _begin_label;
    std::string _end_label;
};

} // namespace jftt::loop
