#include "io.hpp"

#include <iostream>



int main(int argc, char** argv) {
    const auto [in_file, out_file] = io::parse(argc, argv);

    const auto lines = io::read_lines(in_file);
    for (const auto& line : lines)
        std::cout << line << std::endl;

    const auto input = io::read(in_file);
    std::cout << input << std::endl;

    io::write(input, out_file);
    io::write_lines(lines, out_file, std::ios::app);

    return 0;
}
