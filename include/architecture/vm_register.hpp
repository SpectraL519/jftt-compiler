#pragma once

#include "vm_params.hpp"

#include <string>



namespace jftt::architecture {

class vm_register {
public:
    vm_register(const vm_register&) = delete;
    vm_register(vm_register&&) = delete;

    vm_register& operator=(const vm_register&) = delete;
    vm_register& operator=(vm_register&&) = delete;

    vm_register(vm_register_discriminator);

    [[nodiscard]] vm_register_discriminator discriminator() const;
    [[nodiscard]] bool is_free() const;
    bool acquire();
    bool release();

private:
    vm_register_discriminator _discriminator;
    bool _free;
};

[[nodiscard]] bool is_accumulator(const vm_register&);
[[nodiscard]] std::string as_string(const vm_register&);

} // namespace jftt::architecture
