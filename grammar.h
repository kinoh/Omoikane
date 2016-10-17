#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <cassert>
#include <cstdlib>
#include "pegtl.hh"

#include "ast.h"
#include "parser_state.h"
#include "code_dumper.h"

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
	struct mul_op : one<'*', '/'> {};
	struct mul_expr : seq<primary_expr, star<seq<mul_op, primary_expr>>> {};
	struct add_op : one<'+', '-'> {};
	struct add_expr : seq<mul_expr, star<seq<add_op, mul_expr>>> {};
	struct grammar : must<add_expr, eof> {};

	template<typename Rule>
	struct action
		: nothing<Rule> {};

	template<>
	struct action<integer>
	{
		static void apply(const action_input &, parser_state &);
	};
	template<>
	struct action<real>
	{
		static void apply(const action_input &, parser_state &);
	};
	template<>
	struct action<symbol>
	{
		static void apply(const action_input &, parser_state &);
	};
	template<>
	struct action<mul_op>
	{
		static void apply(const action_input &, parser_state &);
	};
	template<>
	struct action<mul_expr>
	{
		static void apply(const action_input &, parser_state &);
	};
	template<>
	struct action<add_op>
	{
		static void apply(const action_input &, parser_state &);
	};
	template<>
	struct action<add_expr>
	{
		static void apply(const action_input &, parser_state &);
	};
	template<>
	struct action<bracket_open>
	{
		static void apply(const action_input &, parser_state &);
	};
	template<>
	struct action<bracket_close>
	{
		static void apply(const action_input &, parser_state &);
	};
}

#endif
