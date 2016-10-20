#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include <cassert>
#include <functional>
#include <typeinfo>
#include <typeindex>
#include <vector>
#include "pegtl.hh"

#include "ast.h"

namespace omoikane
{
	using namespace pegtl;

	class parser_state
	{
	public:
		parser_state();
		void push_op(op_kind);
		op_kind pop_op(op_group);
		void push_node(ast::node *);
		ast::node *pop_node();
		void mark();
		void unmark();
		void clear();
		ast::node *finish();

	private:
		struct mark_index
		{
			size_t node, op;
		};

		std::vector<ast::node *> node_stack;
		std::vector<op_kind> op_stack;
		std::vector<mark_index> marks;
	};
}

#endif
