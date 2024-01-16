#pragma once

#include "abstract_lvalue_identifier.hpp"



namespace jftt::identifier {

class reference : public abstract_lvalue_identifier {
public:
    reference(
        const std::string& name,
        const type_discriminator reference_discriminator);

    reference(const reference&) = default;
    reference(reference&&) = default;

    reference& operator=(const reference&) = default;
    reference& operator=(reference&&) = default;

    ~reference() = default;

    [[nodiscard]] architecture::memory_size_type size() const override;
    [[nodiscard]] type_discriminator reference_discriminator() const;
    [[nodiscard]] std::shared_ptr<identifier::abstract_identifier> indexer() const;
    [[nodiscard]] bool is_initialized() const override;
    [[nodiscard]] const bool initializes_underlying_resource() const;

    void set_indexer(const std::shared_ptr<identifier::abstract_identifier>& indexer);
    void initialize() override;
    void initialize_resource();

private:
    static constexpr architecture::memory_size_type _size{1u};
    type_discriminator _reference_discriminator;
    std::shared_ptr<identifier::abstract_identifier> _indexer_identifier;
    static constexpr bool _initialized{true};
    bool _resouce_initialized{false};
};

} // namespace jftt::identifier
