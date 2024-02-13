#pragma once

#include <cstdint>
#include <limits>



namespace jftt::architecture {

using instruction_counter_type = std::size_t;
using memory_address_type = uint64_t;
using memory_size_type = uint64_t;
using value_type = uint64_t;

enum class vm_register_discriminator : uint8_t {
    a, b, c, d, e, f, g, h
};

constexpr memory_size_type VM_MEMORY_SIZE{1ull << 62};
constexpr uint8_t VM_REGISTER_NO{
    static_cast<uint8_t>(vm_register_discriminator::h) + 1};
constexpr int VM_VALUE_BITSIZE{std::numeric_limits<value_type>::digits};

} // namespace jftt::architecture
