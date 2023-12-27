#pragma once

#include <ios>
#include <string>
#include <vector>



namespace io {

std::pair<std::string, std::string> parse(int, char**);

std::string read(const std::string&);
std::vector<std::string> read_lines(const std::string&);

void write(const std::string&, const std::string&);
void write_lines(
    const std::vector<std::string>&,
    const std::string&,
    const std::ios::openmode = std::ios::in | std::ios::out);


namespace detail {

std::string join(const std::vector<std::string>&, const std::string& = " ");

} // namespace detail

} // namespace io
