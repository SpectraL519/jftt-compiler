# Structure
APP_DIR 	= ./app
INCLUDE_DIR = ./include
SOURCE_DIR  = ./source
EXEC 		= compiler

# Source files
SOURCE_FILES 	 := $(wildcard $(SOURCE_DIR)/*.cpp) $(wildcard $(SOURCE_DIR)/**/*.cpp)
APP_SOURCE_FILES := $(wildcard $(APP_DIR)/*.cpp)

# Generators
FF := flex
BB := bison

# Compiler
CXX := g++
CXX_STD := c++17
CXX_FLAGS += -std=$(CXX_STD) -O3 -Wall

# CMD
RM = rm -rf


# Rules
# TODO: vm
.PHONY: all compiler parser lexer clean

all: compiler


compiler: parser lexer
	@echo Building the compiler
	@$(CXX) -o $(EXEC) -I $(INCLUDE_DIR) $(CXX_FLAGS) \
		$(APP_SOURCE_FILES) $(SOURCE_FILES) $(SOURCE_DIR)/parser.cpp $(SOURCE_DIR)/lexer.cpp
	@echo Build successful!


lexer:
	@echo Generating lexer
	@$(FF) -o $(SOURCE_DIR)/lexer.cpp $(SOURCE_DIR)/lexer.l


parser:
	@echo Generating parser
	@$(BB) -o $(SOURCE_DIR)/parser.cpp --defines=$(INCLUDE_DIR)/parser.hpp $(SOURCE_DIR)/parser.y


clean:
	@echo Removing all generated files
	@$(RM) $(EXEC)
	@$(RM) $(SOURCE_DIR)/lexer.cpp
	@$(RM) $(SOURCE_DIR)/parser.cpp $(INCLUDE_DIR)/parser.hpp
