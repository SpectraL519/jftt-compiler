#pragma once

#include "../architecture/vm_params.hpp"
#include "../architecture/vm_register.hpp"

#include <string>



namespace jftt::assembly::instructions {

[[nodiscard]] std::string read();
[[nodiscard]] std::string write();

[[nodiscard]] std::string load(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string store(const jftt::architecture::vm_register& reg);

[[nodiscard]] std::string add(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string sub(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string get(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string put(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string rst(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string inc(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string dec(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string shl(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string shr(const jftt::architecture::vm_register& reg);

[[nodiscard]] std::string strk(const jftt::architecture::vm_register& reg);
[[nodiscard]] std::string jumpr(const jftt::architecture::vm_register& reg);

[[nodiscard]] std::string halt();


// label jump instructions
const std::string jump_label{"JUMP "};
const std::string jpos_label{"JPOS "};
const std::string jzero_label{"JZERO "};

} // namespace jftt::assembly::instructions
