#pragma once

#include "identifier/abstract_identifier.hpp"
#include "identifier/abstract_lvalue_identifier.hpp"
#include "identifier/identifier_cast.hpp"
#include "identifier/identifier_traits.hpp"
#include "identifier/procedure.hpp"
#include "identifier/rvalue.hpp"
#include "identifier/vararray.hpp"
#include "identifier/variable.hpp"
#include "identifier/reference.hpp"

namespace jftt {
using identifier_discriminator = identifier::type_discriminator;
} // namespace jftt
