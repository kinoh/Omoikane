#include <iostream>
#include "code_dumper.h"
#include "ast.h"

omoikane::code_dumper::code_dumper(std::ostream &out)
	: out(out)
{
}
void omoikane::code_dumper::dump(omoikane::ast::node *node)
{
	node->accept(this);
}

void omoikane::code_dumper::visit(omoikane::ast::binary_expression *expr)
{
	out << '(';
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
	out << ')';
}
void omoikane::code_dumper::visit(omoikane::ast::symbol *symbol)
{
	out << symbol->name;
}
void omoikane::code_dumper::visit(omoikane::ast::integer_literal *integer)
{
	out << integer->value;
}
void omoikane::code_dumper::visit(omoikane::ast::real_literal *real)
{
	out << real->value;
}
