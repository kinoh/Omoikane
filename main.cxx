#include <iostream>
#include "pegtl.hh"
#include "ast.h"
#include "parser_state.h"
#include "code_dumper.h"
#include "grammar.h"

int main(int argc, char *argv[])
{
	omoikane::parser_state state;
	pegtl::parse_arg<omoikane::grammar, omoikane::action>(1, argv, state);

	auto ast = state.finish();
	omoikane::code_dumper dumper(std::cout);
	dumper.dump(ast);
}
