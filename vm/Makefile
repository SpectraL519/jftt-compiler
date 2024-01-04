FLAGS = -W -pedantic -std=c++17 -O3

VM_OUT 	   = ../run-vm
CLN_VM_OUT = ../run-cln-vm

.PHONY = all clean cleanall

all: vm cln-vm

vm: lexer.o parser.o mw.o main.o
	@$(CXX) $^ -o $(VM_OUT)

cln-vm: lexer.o parser.o mw-cln.o main.o
	@$(CXX) $^ -o $(CLN_VM_OUT) -l cln

%.o: %.cc
	@$(CXX) $(FLAGS) -c $^

lexer.cc: lexer.l parser.hh
	@flex -o $@ $<

parser.cc parser.hh: parser.y
	@bison -Wall -d -o parser.cc $^

clean:
	@rm -f *.o parser.cc parser.hh lexer.cc

cleanall: clean
	@rm -f $(VM_OUT) $(CLN_VM_OUT)
