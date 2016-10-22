#include "ast.h"

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
void omoikane::ast::expression_list::accept(omoikane::ast::visitor *v)
{
	v->visit(this);
}
