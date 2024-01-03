#pragma once

#include "../architecture/vm_params.hpp"

#include <string>



namespace jftt::assembly {

[[nodiscard]] std::string read() noexcept {
    static const std::string instruction{"READ"};
    return instruction;
}

[[nodiscard]] std::string write() noexcept {
    static const std::string instruction{"WRITE"};
    return instruction;
}


[[nodiscard]] std::string load(const jftt::architecture::memory_address_type addr) noexcept {
    static const std::string instruction{"LOAD "};
    return instruction + std::to_string(addr);
}

[[nodiscard]] std::string store(const jftt::architecture::memory_address_type addr) noexcept {
    static const std::string instruction{"STORE "};
    return instruction + std::to_string(addr);
}


[[nodiscard]] std::string add(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"ADD "};
    return instruction + jftt::architecture::as_string(reg);
}

[[nodiscard]] std::string sub(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"SUB "};
    return instruction + jftt::architecture::as_string(reg);
}

[[nodiscard]] std::string get(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"GET "};
    return instruction + jftt::architecture::as_string(reg);
}

[[nodiscard]] std::string put(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"PUT "};
    return instruction + jftt::architecture::as_string(reg);
}

[[nodiscard]] std::string rst(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"RST "};
    return instruction + jftt::architecture::as_string(reg);
}

[[nodiscard]] std::string inc(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"INC "};
    return instruction + jftt::architecture::as_string(reg);
}

[[nodiscard]] std::string dec(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"DEC "};
    return instruction + jftt::architecture::as_string(reg);
}

[[nodiscard]] std::string shl(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"SHL "};
    return instruction + jftt::architecture::as_string(reg);
}

[[nodiscard]] std::string shr(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"SHR "};
    return instruction + jftt::architecture::as_string(reg);
}


[[nodiscard]] std::string jump(
    const jftt::architecture::instruction_counter_type instr_no
) noexcept {
    static const std::string instruction{"JUMP "};
    return instruction + std::to_string(instr_no);
}

[[nodiscard]] std::string jpos(
    const jftt::architecture::instruction_counter_type instr_no
) noexcept {
    static const std::string instruction{"JPOS "};
    return instruction + std::to_string(instr_no);
}

[[nodiscard]] std::string jzero(
    const jftt::architecture::instruction_counter_type instr_no
) noexcept {
    static const std::string instruction{"JZERO "};
    return instruction + std::to_string(instr_no);
}


[[nodiscard]] std::string strk(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"STRK "};
    return instruction + jftt::architecture::as_string(reg);
}

[[nodiscard]] std::string jumpr(const jftt::architecture::vm_register_discriminator reg) noexcept {
    static const std::string instruction{"JUMPR "};
    return instruction + jftt::architecture::as_string(reg);
}


[[nodiscard]] std::string halt() noexcept {
    static const std::string instruction{"HALT"};
    return instruction;
}

} // namespace jftt::assembly
