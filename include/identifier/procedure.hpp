#pragma once

#include "abstract_identifier.hpp"



namespace jftt::identifier {

class procedure_identifier : public abstract_identifier {
public:
    procedure_identifier(const std::string& name)
    : abstract_identifier(type_discriminator::procedure, name) {}

    procedure_identifier(const procedure_identifier&) = default;
    procedure_identifier(procedure_identifier&&) = default;

    procedure_identifier& operator=(const procedure_identifier&) = default;
    procedure_identifier& operator=(procedure_identifier&&) = default;

    ~procedure_identifier() = default;

    // TODO: specialize
};

} // namespace jftt::identifier
