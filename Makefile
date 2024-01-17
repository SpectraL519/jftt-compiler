# Structure
APP_DIR 	= ./app
INCLUDE_DIR = ./include
SOURCE_DIR  = ./source
EXEC 		= compiler
PARSER_SRC  = $(SOURCE_DIR)/parser.cpp
PARSER_INC  = $(INCLUDE_DIR)/parser.hpp
LEXER_SRC   = $(SOURCE_DIR)/lexer.cpp

# Generators
FF := flex
BB := bison

# Compiler
CXX := g++
CXX_STD := c++17
CXX_FLAGS += -std=$(CXX_STD) -O3 -Wall -Wno-unused-function -g

# CMD
RM = rm -rf


# Rules
.PHONY: all compiler parser lexer clean vm cleanvm cleanall

all: cleanall compiler vm


compiler: clean parser lexer
	@echo Building the compiler
	@$(CXX) $(CXX_FLAGS) -o $(EXEC) -I $(INCLUDE_DIR) \
		$(wildcard $(APP_DIR)/*.cpp) $(wildcard $(SOURCE_DIR)/*.cpp) $(wildcard $(SOURCE_DIR)/**/*.cpp)
	@echo Build successful!


lexer:
	@echo Generating lexer
	@$(FF) -o $(LEXER_SRC) $(SOURCE_DIR)/lexer.l


parser:
	@echo Generating parser
	@$(BB) -o $(PARSER_SRC) --defines=$(PARSER_INC) $(SOURCE_DIR)/parser.y


clean:
	@echo Removing compiler generated files
	@$(RM) $(EXEC)
	@$(RM) $(LEXER_SRC)
	@$(RM) $(PARSER_SRC) $(PARSER_INC)
	@$(RM) out/*
	@echo


vm: cleanvm
	@echo Building the virtual machine
	@cd vm && make all CXX=$(CXX) && cd ..
	@echo Build successful!


cleanvm:
	@echo Removing vm generated files
	@cd vm && make cleanall && cd ..
	@echo


cleanall: clean cleanvm
	@echo
