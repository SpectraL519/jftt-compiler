#pragma once

#include "abstract_identifier.hpp"
#include "identifier_cast.hpp"
#include "reference.hpp"
#include "../architecture/vm_params.hpp"

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>



namespace jftt::identifier {

class procedure : public abstract_identifier {
public:
    procedure(
        const std::string& name,
        const jftt::architecture::memory_address_type return_point_address);

    procedure(const procedure&) = default;
    procedure(procedure&&) = default;

    procedure& operator=(const procedure&) = default;
    procedure& operator=(procedure&&) = default;

    ~procedure() = default;

    [[nodiscard]] std::variant<std::string, std::shared_ptr<reference>> declare_parameter(
        const std::string& name, const type_discriminator discriminator);
    [[nodiscard]] std::shared_ptr<reference> get_next_parameter();
    std::optional<std::string> finish_parameter_passing();
    std::optional<std::string> declare_local_identifier(
        std::shared_ptr<identifier::abstract_lvalue_identifier> lvalue);

    [[nodiscard]] bool has_identifier(const std::string& name) const;
    [[nodiscard]] bool has_identifier(
        const std::string& name, const type_discriminator discriminator) const;
    [[nodiscard]] std::shared_ptr<identifier::abstract_lvalue_identifier> get_identifier(
        const std::string& name);

    void set_begin_label(const std::string& begin_label);
    [[nodiscard]] const std::string& begin_label() const;
    [[nodiscard]] const jftt::architecture::memory_address_type return_point_address() const;

private:
    void _assert_valid_param_discriminator(const type_discriminator discriminator) const;

    [[nodiscard]] std::vector<std::shared_ptr<abstract_lvalue_identifier>>::iterator _find_param(
        const std::string& name);
    [[nodiscard]] std::vector<std::shared_ptr<abstract_lvalue_identifier>>::const_iterator _cfind_param(
        const std::string& name) const;

    std::vector<std::shared_ptr<abstract_lvalue_identifier>> _local_identifiers;
    std::size_t _param_no{0u};
    std::size_t _call_param_idx{0u};

    std::string _begin_label;
    const jftt::architecture::memory_address_type _return_point_address;
};

} // namespace jftt::identifier
