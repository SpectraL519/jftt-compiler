#pragma once

#include "identifiers.hpp"
#include "architecture/vm_memory_manager.hpp"

#include <memory>
#include <map>



namespace jftt {

class identifier_manager {
public:
    identifier_manager(const identifier_manager&) = delete;
    identifier_manager(identifier_manager&&) = delete;

    identifier_manager& operator=(const identifier_manager&) = delete;
    identifier_manager& operator=(identifier_manager&&) = delete;

    identifier_manager() = default;
    ~identifier_manager() = default;

    void add_variable(const std::string& name);
    void add_vararray(const std::string& name, const architecture::memory_size_type size);
    void add_procedure(const std::string& name);

    [[nodiscard]] bool has(const std::string& name) const;
    [[nodiscard]] bool has(
        const std::string& name, const identifier_discriminator discriminator) const;
    [[nodiscard]] const std::unique_ptr<abstract_identifier_base>& get(
        const std::string& name) const;
    void remove(const std::string& name);

private:
    std::map<std::string, std::unique_ptr<abstract_identifier_base>> _identifiers;
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
};

} // namespace jftt
