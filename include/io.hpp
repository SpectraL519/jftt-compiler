#pragma once

#include <ios>
#include <string>
#include <vector>



namespace jftt::io {

std::pair<std::string, std::string> parse(int, char**);

std::string read(const std::string&);
std::vector<std::string> read_lines(const std::string&);

void write(const std::string&, const std::string&);
void write_lines(
    const std::vector<std::string>&,
    const std::string&,
    const std::ios::openmode = std::ios::in | std::ios::out);


namespace detail {

constexpr int expected_argc{3};
constexpr int in_idx{1}, out_idx{2};

std::string join(const std::vector<std::string>&, const std::string& = " ");

} // namespace detail

} // namespace jftt::io
