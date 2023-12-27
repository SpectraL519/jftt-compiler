# Structure
APP_DIR 	= ./app
INCLUDE_DIR = ./include
SOURCE_DIR  = ./src
OUT 		= compiler

# Source files
SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
APP_SOURCE_FILES := $(wildcard $(APP_DIR)/*.cpp) $(SRC)


# Generators
FF := flex
BB := bison

# Compiler
CXX := g++
CXX_STD := c++17
CXX_FLAGS += -std=$(CXX_STD)


# Rules
.PHONY: all parser lexer clean

# TODO: <lexer-out-file> <parser-out-file>
all: parser lexer
	@echo Building the compiler
	@$(CXX) -o $(OUT) $(APP_SOURCE_FILES) $(SOURCE_FILES) -I $(INCLUDE_DIR) $(CXX_FLAGS)
	@echo Build successful!


# TODO: $(BB) -d -o <parser-out-file> <bison-yacc-file>
parser:
	@echo Generating grammar parser [TODO]


# TODO: $(FF) -o <lexer-out-file> <flex-file>
lexer:
	@echo Generating lexer [TODO]


# TODO: <lexer-out-file> <parser-out-header-file> <parser-out-file>
clean:
	@echo Removing all generated files
	@rm $(OUT)
