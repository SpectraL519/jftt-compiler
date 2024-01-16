#pragma once

#include "abstract_identifier.hpp"



namespace jftt::identifier {

class rvalue : public abstract_identifier {
public:
    rvalue() = delete;

    rvalue(const rvalue&) = default;
    rvalue(rvalue&&) = default;

    rvalue& operator=(const rvalue&) = default;
    rvalue& operator=(rvalue&&) = default;

    ~rvalue() = default;

    rvalue(const architecture::value_type value);

    [[nodiscard]] architecture::value_type value() const;

private:
    const architecture::value_type _value;
    inline static std::string _name{"rvalue"};
};

} // namespace jftt::identifier
