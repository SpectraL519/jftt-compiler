#pragma once

#include <cstdint>
#include <string>



namespace jftt::architecture {

enum class register_discriminator : uint8_t {
    a, b, c, d, e, f, g, h
};

[[nodiscard]] std::string as_string(const register_discriminator rd) {
    return std::to_string(static_cast<uint8_t>(rd));
}


using instruction_counter_type = std::size_t;
using memory_address_type = uint64_t;
using memory_size_type = uint64_t;
using value_type = uint64_t;


static constexpr memory_address_type VM_MEMORY_SIZE{0x4000000000000000ull}; // 2^62
static constexpr uint8_t VM_REGISTER_NO{
    static_cast<uint8_t>(register_discriminator::h) + 1};

} // namespace jftt::architecture
