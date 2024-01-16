#pragma once

#include "abstract_lvalue_identifier.hpp"



namespace jftt::identifier {

class vararray : public abstract_lvalue_identifier {
public:
    // static size declaration
    vararray(const std::string& name, const architecture::memory_size_type size);

    // dynamic size declaration - for procedures
    vararray(const std::string& name);

    vararray(const vararray&) = default;
    vararray(vararray&&) = default;

    vararray& operator=(const vararray&) = default;
    vararray& operator=(vararray&&) = default;

    ~vararray() = default;

    [[nodiscard]] architecture::memory_size_type size() const override;

    [[nodiscard]] std::shared_ptr<identifier::abstract_identifier> indexer() const;
    void set_indexer(const std::shared_ptr<identifier::abstract_identifier>& indexer);

    [[nodiscard]] bool is_initialized() const override;
    void initialize() override;

private:
    architecture::memory_size_type _size{1u};
    static constexpr bool _initialized{true};
    std::shared_ptr<identifier::abstract_identifier> _indexer_identifier;
};

} // namespace jftt::identifier
