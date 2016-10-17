#ifndef CODE_DUMPER_H
#define CODE_DUMPER_H

#include "ast.h"

namespace omoikane
{
	class code_dumper : ast::visitor
	{
	public:
		code_dumper(std::ostream &);
		void dump(ast::node *);

	private:
		std::ostream &out;

		void visit(ast::binary_expression *);
		void visit(ast::symbol *);
		void visit(ast::integer_literal *);
		void visit(ast::real_literal *);
	};
}

#endif
