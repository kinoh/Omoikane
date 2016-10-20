#ifndef AST_H
#define AST_H

#include <string>

namespace omoikane
{
	enum class op_kind
	{
		NONE,
		MUL,
		DIV,
		ADD,
		SUB,
	};
	enum class op_group
	{
		ADDITIVE,
		MULTIPLICATIVE,
		POWER,
		LISTING,
	};

	struct op_spec
	{
		static op_group group_of(op_kind op)
		{
			switch (op)
			{
			case op_kind::ADD:
			case op_kind::SUB:
				return op_group::ADDITIVE;
			case op_kind::MUL:
			case op_kind::DIV:
				return op_group::MULTIPLICATIVE;
			}
			throw;
		}
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
}

#endif
