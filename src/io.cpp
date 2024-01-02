#include "io.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>



namespace jftt::io {

std::pair<std::string, std::string> parse(int argc, char** argv) {
    if (argc != detail::expected_argc) {
        std::cerr << "Compiler usage: " << std::endl 
                  << "./compiler <src-code-file> <asm-out-file>" << std::endl;
        std::exit(1); 
    }

    return std::make_pair(argv[detail::in_idx], argv[detail::out_idx]);
}


std::string read(const std::string& file_name) {
    return detail::join(read_lines(file_name), "\n");
}

std::vector<std::string> read_lines(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + file_name);

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        if (!line.empty())
            lines.push_back(line);

    return lines;
}


void write(const std::string& str, const std::string& file_name) {
    std::ofstream file(file_name);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + file_name);

    file << str;
}

void write_lines(
    const std::vector<std::string>& lines,
    const std::string& file_name,
    const std::ios::openmode openmode
) {
    std::ofstream file(file_name, openmode);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + file_name);

    for (const auto& line : lines)
        file << line << '\n';
}


namespace detail {

std::string join(const std::vector<std::string>& str_list, const std::string& delimiter) {
    if (str_list.empty())
        return std::string{};

    std::string joined = str_list.front();
    for (std::size_t i = 1; i < str_list.size(); i++)
        joined += delimiter + str_list.at(i);
    return joined;
}

} // namespace detail

} // namespace jftt::io
