#include "grammar.h"
#include <functional>
#include <iostream>

using namespace pegtl;

static omoikane::ast::expression *foldl_binary(omoikane::parser_state &s, omoikane::op_group group)
{
	auto n = dynamic_cast<omoikane::ast::expression *>(s.pop_node());
	omoikane::op_kind op = s.pop_op(group);
	if (op == omoikane::op_kind::NONE)
		return n;
	auto expr = new omoikane::ast::binary_expression;
	expr->op = op;
	expr->left = foldl_binary(s, group);
	expr->right = n;
	return static_cast<omoikane::ast::expression *>(expr);
}
static omoikane::ast::expression *foldr_binary(omoikane::parser_state &s, omoikane::op_group group)
{
	auto n = dynamic_cast<omoikane::ast::expression *>(s.pop_node());
	omoikane::op_kind op;
	while ((op = s.pop_op(group)) != omoikane::op_kind::NONE)
	{
		auto expr = new omoikane::ast::binary_expression;
		expr->op = op;
		expr->left = dynamic_cast<omoikane::ast::expression *>(s.pop_node());
		expr->right = n;
		n = expr;
	}
	return n;
}

void omoikane::action<omoikane::integer>::apply(const action_input &in, omoikane::parser_state &s)
{
	auto lit = new ast::integer_literal;
	lit->value = std::atoi(in.string().c_str());
	s.push_node(lit);
}
void omoikane::action<omoikane::real>::apply(const action_input &in, omoikane::parser_state &s)
{
	auto lit = new ast::real_literal;
	lit->value = std::atof(in.string().c_str());
	s.push_node(lit);
}
void omoikane::action<omoikane::symbol>::apply(const action_input &in, omoikane::parser_state &s)
{
	auto symbol = new ast::symbol;
	symbol->name = in.string();
	s.push_node(symbol);
}
void omoikane::action<omoikane::pow_op>::apply(const action_input &in, omoikane::parser_state &s)
{
	if (in.string() == "^")
		s.push_op(op_kind::POW);
}
void omoikane::action<omoikane::pow_expr>::apply(const action_input &in, omoikane::parser_state &s)
{
	s.push_node(foldr_binary(s, op_group::POWER));
}
void omoikane::action<omoikane::mul_op>::apply(const action_input &in, omoikane::parser_state &s)
{
	if (in.string() == "*")
		s.push_op(op_kind::MUL);
	else if (in.string() == "/")
		s.push_op(op_kind::DIV);
}
void omoikane::action<omoikane::mul_expr>::apply(const action_input &in, omoikane::parser_state &s)
{
	s.push_node(foldl_binary(s, op_group::MULTIPLICATIVE));
}
void omoikane::action<omoikane::add_op>::apply(const action_input &in, omoikane::parser_state &s)
{
	if (in.string() == "+")
		s.push_op(op_kind::ADD);
	else if (in.string() == "-")
		s.push_op(op_kind::SUB);
}
void omoikane::action<omoikane::add_expr>::apply(const action_input &in, omoikane::parser_state &s)
{
	s.push_node(foldl_binary(s, op_group::ADDITIVE));
}
void omoikane::action<omoikane::paren_open>::apply(const action_input &in, omoikane::parser_state &s)
{
	s.mark(mark_type::PAREN);
}
void omoikane::action<omoikane::paren_close>::apply(const action_input &in, omoikane::parser_state &s)
{
	s.unmark(mark_type::PAREN);
}
