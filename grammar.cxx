#include "grammar.h"

using namespace pegtl;

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
void omoikane::action<omoikane::mul_op>::apply(const action_input &in, omoikane::parser_state &s)
{
	if (in.string() == "*")
		s.push_op(op_kind::MUL);
	else if (in.string() == "/")
		s.push_op(op_kind::DIV);
}
void omoikane::action<omoikane::mul_expr>::apply(const action_input &in, omoikane::parser_state &s)
{
	auto j = s.node_begin();
	auto left = dynamic_cast<ast::expression *>(*j);
	++j;
	for (auto i = s.op_begin(); i != s.op_end(); ++i, ++j)
	{
		auto mul = new ast::binary_expression;
		mul->op = *i;
		mul->left = left;
		mul->right = dynamic_cast<ast::expression *>(*j);
		left = mul;
	}
	s.clear();
	s.push_node(left);
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
	auto j = s.node_begin();
	auto left = dynamic_cast<ast::expression *>(*j);
	++j;
	for (auto i = s.op_begin(); i != s.op_end(); ++i, ++j)
	{
		auto mul = new ast::binary_expression;
		mul->op = *i;
		mul->left = left;
		mul->right = dynamic_cast<ast::expression *>(*j);
		left = mul;
	}
	s.clear();
	s.push_node(left);
}
void omoikane::action<omoikane::bracket_open>::apply(const action_input &in, omoikane::parser_state &s)
{
	s.mark();
}
void omoikane::action<omoikane::bracket_close>::apply(const action_input &in, omoikane::parser_state &s)
{
	s.unmark();
}
