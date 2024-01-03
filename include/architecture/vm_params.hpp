#pragma once

#include <cstdint>



namespace jftt::architecture {

using memory_address_type = uint64_t;

enum class vm_register_discriminator : uint8_t {
    a, b, c, d, e, f, g, h
};

static constexpr memory_address_type VM_MEMORY_SIZE{0x4000000000000000ull}; // 2^62
static constexpr uint8_t VM_REGISTER_NO{
    static_cast<uint8_t>(vm_register_discriminator::h) + 1};

} // namespace jftt::architecture
