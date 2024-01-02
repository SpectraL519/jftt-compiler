#include "io.hpp"

#include <iostream>


// bison main function
int compile(const std::string& infile, const std::string& outfile);


int main(int argc, char** argv) {
    const auto [infile, outfile] = jftt::io::parse(argc, argv);

    compile(infile, outfile);

    return 0;
}
