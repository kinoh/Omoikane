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

	struct ws : star<one<' ', '\t', '\n', '\r'>> {};
	struct integer : plus<digit> {};
	struct real : seq<plus<digit>, one<'.'>, plus<digit>> {};
	struct literal : sor<real, integer> {};
	struct symbol : identifier {};
	struct list_sep : one<','> {};
	struct expr_list : seq<class expr, star<seq<ws, list_sep, ws, class expr>>> {};
	struct bracket_open : one<'['> {};
	struct bracket_close : one<']'> {};
	struct vector_expr : seq<bracket_open, ws, expr_list, ws, bracket_close> {};
	struct paren_open : one<'('> {};
	struct paren_close : one<')'> {};
	struct paren_expr : seq<paren_open, ws, class expr, ws, paren_close> {};
	struct primary_expr : sor<symbol, literal, paren_expr, vector_expr> {};
	struct pow_op : one<'^'> {};
	struct pow_expr : seq<primary_expr, star<seq<ws, pow_op, ws, primary_expr>>> {};
	struct mul_op : one<'*', '/'> {};
	struct mul_expr : seq<pow_expr, star<seq<ws, mul_op, ws, pow_expr>>> {};
	struct add_op : one<'+', '-'> {};
	struct add_expr : seq<mul_expr, star<seq<ws, add_op, ws, mul_expr>>> {};
	struct expr : sor<add_expr, vector_expr> {};
	struct grammar : must<expr, eof> {};

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
	DECLARE_ACTION(paren_open);
	DECLARE_ACTION(paren_close);
	DECLARE_ACTION(bracket_open);
	DECLARE_ACTION(bracket_close);
	DECLARE_ACTION(list_sep);
	DECLARE_ACTION(expr_list);
}

#endif
