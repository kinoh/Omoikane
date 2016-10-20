#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <cassert>
#include <cstdlib>
#include "pegtl.hh"

#include "ast.h"
#include "parser_state.h"
#include "code_dumper.h"

#define DECLARE_ACTION(T) \
	template<> \
	struct action<T> \
	{ \
		static void apply(const action_input &, parser_state &); \
	}


namespace omoikane
{
	using namespace pegtl;

	struct ws : one<' ', '\t', '\n', '\r'> {};
	struct integer : plus<digit> {};
	struct real : seq<plus<digit>, one<'.'>, plus<digit>> {};
	struct literal : sor<real, integer> {};
	struct symbol : identifier {};
	struct bracket_open : one<'('> {};
	struct bracket_close : one<')'> {};
	struct bracket_expr : seq<bracket_open, class add_expr, bracket_close> {};
	struct primary_expr : sor<symbol, literal, bracket_expr> {};
	struct pow_op : one<'^'> {};
	struct pow_expr : seq<primary_expr, star<seq<pow_op, primary_expr>>> {};
	struct mul_op : one<'*', '/'> {};
	struct mul_expr : seq<pow_expr, star<seq<mul_op, pow_expr>>> {};
	struct add_op : one<'+', '-'> {};
	struct add_expr : seq<mul_expr, star<seq<add_op, mul_expr>>> {};
	struct grammar : must<add_expr, eof> {};

	template<typename Rule>
	struct action
		: nothing<Rule> {};

	DECLARE_ACTION(integer);
	DECLARE_ACTION(real);
	DECLARE_ACTION(symbol);
	DECLARE_ACTION(pow_op);
	DECLARE_ACTION(pow_expr);
	DECLARE_ACTION(mul_op);
	DECLARE_ACTION(mul_expr);
	DECLARE_ACTION(add_op);
	DECLARE_ACTION(add_expr);
	DECLARE_ACTION(bracket_open);
	DECLARE_ACTION(bracket_close);
}

#endif
