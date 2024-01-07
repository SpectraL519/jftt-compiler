#pragma once

#include <string>



namespace jftt {

namespace condition {

enum class branch_discriminator : uint8_t {
    eq, neq, le, leq, ge, geq
};

struct branch {
    const std::string false_eval_label;
};

} // namespace condition

using condition_discriminator = condition::branch_discriminator;

} // namespace jftt
