#pragma once

#include <map>
#include <string>
#include <vector>


namespace jftt::assembly {

class code_builder;

class jump_manager {
public:
    jump_manager(code_builder& asm_builder);

    ~jump_manager() = default;

    [[nodiscard]] std::string new_label(const std::string& name);
    [[nodiscard]] std::string new_label();
    void insert_label(const std::string& label_name);
    void jump_to_label(const std::string& jump_intsruction, const std::string& label_name);
    void fill_labels();

private:
    struct jump_label {
        std::size_t jump_to;
        std::vector<std::size_t> jump_from_list;
    };

    code_builder& _asm_builder;
    std::size_t _label_no{0u};

    static constexpr std::size_t _invalid_jump_to{0u};
    inline static const std::string _default_label_name{"label"};

    // TODO: extract pair to struct
    std::map<std::string, jump_label> _labels;
};

} // namespace jftt::assembly
