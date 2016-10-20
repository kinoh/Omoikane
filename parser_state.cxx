#include "parser_state.h"

omoikane::parser_state::parser_state() :
	node_stack(),
	op_stack(),
	marks()
{
}
void omoikane::parser_state::push_op(omoikane::op_kind op)
{
	op_stack.push_back(op);
}
omoikane::op_kind omoikane::parser_state::pop_op(op_group group)
{
	if (op_stack.size() == 0)
		return op_kind::NONE;

	auto r = op_stack.back();

	if (op_spec::group_of(r) != group)
		return op_kind::NONE;

	op_stack.pop_back();
	return r;
}
void omoikane::parser_state::push_node(omoikane::ast::node *node)
{
	node_stack.push_back(node);
}
omoikane::ast::node *omoikane::parser_state::pop_node()
{
	const auto r = node_stack.back();
	node_stack.pop_back();
	return r;
}
void omoikane::parser_state::mark()
{
	marks.push_back(mark_index{ node_stack.size(), op_stack.size() });
}
void omoikane::parser_state::unmark()
{
	marks.pop_back();
}
void omoikane::parser_state::clear()
{
	if (marks.size() == 0)
	{
		op_stack.clear();
		node_stack.clear();
		return;
	}
	auto i = marks.back();
	op_stack.erase(op_stack.begin() + i.op, op_stack.end());
	node_stack.erase(node_stack.begin() + i.node, node_stack.end());
}
omoikane::ast::node *omoikane::parser_state::finish()
{
	assert(op_stack.size() == 0);
	assert(node_stack.size() == 1);
	const auto r = node_stack.back();
	node_stack.clear();
	return r;
}
