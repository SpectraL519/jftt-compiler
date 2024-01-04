#include "assembly/instructions.hpp"



namespace jftt::assembly::instructions {

std::string read() {
    static const std::string instruction{"READ"};
    return instruction;
}

std::string write() {
    static const std::string instruction{"WRITE"};
    return instruction;
}


std::string load(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"LOAD "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string store(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"STORE "};
    return instruction + jftt::architecture::as_string(reg);
}


std::string add(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"ADD "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string sub(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"SUB "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string get(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"GET "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string put(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"PUT "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string rst(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"RST "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string inc(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"INC "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string dec(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"DEC "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string shl(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"SHL "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string shr(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"SHR "};
    return instruction + jftt::architecture::as_string(reg);
}


std::string jump(
    const jftt::architecture::instruction_counter_type instr_no
) {
    static const std::string instruction{"JUMP "};
    return instruction + std::to_string(instr_no);
}

std::string jpos(
    const jftt::architecture::instruction_counter_type instr_no
) {
    static const std::string instruction{"JPOS "};
    return instruction + std::to_string(instr_no);
}

std::string jzero(
    const jftt::architecture::instruction_counter_type instr_no
) {
    static const std::string instruction{"JZERO "};
    return instruction + std::to_string(instr_no);
}


std::string strk(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"STRK "};
    return instruction + jftt::architecture::as_string(reg);
}

std::string jumpr(const jftt::architecture::vm_register& reg) {
    static const std::string instruction{"JUMPR "};
    return instruction + jftt::architecture::as_string(reg);
}


std::string halt() {
    static const std::string instruction{"HALT"};
    return instruction;
}

} // namespace jftt::assembly::instructions
