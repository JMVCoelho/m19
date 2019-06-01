#ifndef __M19_AST_STACK_ALLOC_H__
#define __M19_AST_STACK_ALLOC_H__

#include <cdk/ast/unary_expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {
	//pq e' q "arg" nao vem aqui? ainda tnh de ver a diferenca entre unary_exp e basic
	class stack_alloc_node: public cdk::unary_expression_node {
	public:
		stack_alloc_node(int lineno, cdk::expression_node *arg) :
			cdk::unary_expression_node(lineno, arg) {}

	public:
		void accept(basic_ast_visitor *sp, int level) {
			sp->do_stack_alloc_node(this, level);
		}
	};
} // m19
#endif
