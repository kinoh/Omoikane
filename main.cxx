#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "pegtl.hh"

namespace omoikane
{
	using namespace pegtl;

	enum class op_kind
	{
		MUL,
		DIV,
		ADD,
		SUB,
	};

	namespace ast
	{
		struct node
		{
			virtual void accept(class visitor*) = 0;
			virtual ~node() {}
		};
		struct expression : node
		{
		};
		struct symbol : expression
		{
			std::string name;
			/*virtual*/void accept(visitor *v);
		};
		struct literal : expression
		{
		};
		struct integer_literal : literal
		{
			int value;
			/*virtual*/void accept(visitor*);
		};
		struct real_literal : literal
		{
			double value;
			/*virtual*/void accept(visitor*);
		};
		struct binary_expression : expression
		{
			op_kind op;
			expression *left, *right;
			/*virtual*/void accept(visitor *v);
		};

		class visitor
		{
		public:
			virtual void visit(symbol*) = 0;
			virtual void visit(integer_literal*) = 0;
			virtual void visit(real_literal*) = 0;
			virtual void visit(binary_expression*) = 0;
		};
	}

	class parser_state
	{
	public:
		parser_state() :
			node_stack(),
			op_stack()
		{
		}
		size_t node_count()
		{
			return node_stack.size();
		}
		size_t op_count()
		{
			return op_stack.size();
		}
		void push_op(const op_kind op)
		{
			op_stack.push_back(op);
		}
		op_kind pop_op()
		{
			auto r = op_stack.back();
			op_stack.pop_back();
			return r;
		}
		std::vector<op_kind>::const_iterator op_begin()
		{
			return op_stack.begin();
		}
		std::vector<op_kind>::const_iterator op_end()
		{
			return op_stack.end();
		}
		void push_node(ast::node *node)
		{
			node_stack.push_back(node);
		}
		ast::node *pop_node()
		{
			const auto r = node_stack.back();
			node_stack.pop_back();
			return r;
		}
		std::vector<ast::node *>::const_iterator node_begin()
		{
			return node_stack.begin();
		}
		std::vector<ast::node *>::const_iterator node_end()
		{
			return node_stack.end();
		}
		void clear()
		{
			op_stack.clear();
			node_stack.clear();
		}
		ast::node *finish()
		{
			assert(op_stack.size() == 0);
			assert(node_stack.size() == 1);
			const auto r = node_stack.back();
			node_stack.clear();
			return r;
		}

	private:
		std::vector<ast::node *> node_stack;
		std::vector<op_kind> op_stack;
	};

	class code_dumper : ast::visitor
	{
	public:
		code_dumper(std::ostream &out)
			: out(out)
		{
		}
		void dump(ast::node *node)
		{
			node->accept(this);
		}

	private:
		std::ostream &out;

		void visit(ast::binary_expression *expr)
		{
			expr->left->accept(this);
			switch (expr->op)
			{
			case op_kind::ADD:
				out << '+';
				break;
			case op_kind::SUB:
				out << '-';
				break;
			case op_kind::MUL:
				out << '*';
				break;
			case op_kind::DIV:
				out << '/';
				break;
			}
			expr->right->accept(this);
		}
		void visit(ast::symbol *symbol)
		{
			out << symbol->name;
		}
		void visit(ast::integer_literal *integer)
		{
			out << integer->value;
		}
		void visit(ast::real_literal *real)
		{
			out << real->value;
		}
	};

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
		static void apply(const action_input &in, parser_state &s)
		{
			auto lit = new ast::integer_literal;
			lit->value = std::atoi(in.string().c_str());
			s.push_node(lit);
		}
	};
	template<>
	struct action<real>
	{
		static void apply(const action_input &in, parser_state &s)
		{
			auto lit = new ast::real_literal;
			lit->value = std::atof(in.string().c_str());
			s.push_node(lit);
		}
	};
	template<>
	struct action<symbol>
	{
		static void apply(const action_input &in, parser_state &s)
		{
			auto symbol = new ast::symbol;
			symbol->name = in.string();
			s.push_node(symbol);
		}
	};
	template<>
	struct action<mul_op>
	{
		static void apply(const action_input &in, parser_state &s)
		{
			if (in.string() == "*")
				s.push_op(op_kind::MUL);
			else if (in.string() == "/")
				s.push_op(op_kind::DIV);
		}
	};
	template<>
	struct action<mul_expr>
	{
		static void apply(const action_input &in, parser_state &s)
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
	};
	template<>
	struct action<add_op>
	{
		static void apply(const action_input &in, parser_state &s)
		{
			if (in.string() == "+")
				s.push_op(op_kind::ADD);
			else if (in.string() == "-")
				s.push_op(op_kind::SUB);
		}
	};
	template<>
	struct action<add_expr>
	{
		static void apply(const action_input &in, parser_state &s)
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
	};
}

void omoikane::ast::symbol::accept(omoikane::ast::visitor *v)
{
	v->visit(this);
}
void omoikane::ast::integer_literal::accept(omoikane::ast::visitor *v)
{
	v->visit(this);
}
void omoikane::ast::real_literal::accept(omoikane::ast::visitor *v)
{
	v->visit(this);
}
void omoikane::ast::binary_expression::accept(omoikane::ast::visitor *v)
{
	v->visit(this);
}

int main(int argc, char *argv[])
{
	omoikane::parser_state state;
	pegtl::parse_arg<omoikane::grammar, omoikane::action>(1, argv, state);

	auto ast = state.finish();
	omoikane::code_dumper dumper(std::cout);
	dumper.dump(ast);
}
