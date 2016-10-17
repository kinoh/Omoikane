#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include <cassert>
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
		size_t node_count();
		size_t op_count();
		void push_op(const op_kind);
		op_kind pop_op();
		std::vector<op_kind>::const_iterator op_begin();
		std::vector<op_kind>::const_iterator op_end();
		void push_node(ast::node *);
		ast::node *pop_node();
		std::vector<ast::node *>::const_iterator node_begin();
		std::vector<ast::node *>::const_iterator node_end();
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
