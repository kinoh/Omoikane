cmake_minimum_required(VERSION 2.8)
add_definitions("-Wall -std=c++11 -g -I../PEGTL")
add_executable(Main
	ast.cxx
	parser_state.cxx
	grammar.cxx
	code_dumper.cxx
	main.cxx
)
