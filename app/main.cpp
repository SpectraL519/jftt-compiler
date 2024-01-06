#include "io.hpp"

#include <iostream>


// include all headers to check whether the code compiles
#include "architecture/vm_params.hpp"
#include "architecture/vm_memory_manager.hpp"
#include "architecture/vm_register.hpp"
#include "assembly/code_builder.hpp"
#include "assembly/instructions.hpp"
#include "compiler.hpp"
#include "identifier_manager.hpp"
#include "identifier.hpp"
#include "parser/token.hpp"


// bison main function
int compile(const std::string& infile, const std::string& outfile);


int main(int argc, char** argv) {
    std::cout << std::boolalpha;

    const auto [infile, outfile] = jftt::io::parse(argc, argv);
    compile(infile, outfile);

    return 0;
}
