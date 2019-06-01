#ifndef __M19_AST_ADDRESS_OF_H__
#define __M19_AST_ADDRESS_OF_H__

#include <cdk/ast/unary_expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {
	class address_of_node: public cdk::expression_node {
		cdk::lvalue_node *_leftValue; //sufix ? applied to a left value returns its address

	public:
		address_of_node(int lineno, cdk::lvalue_node *leftValue) :
			cdk::expression_node(lineno), _leftValue(leftValue) {}

	public:
		cdk::lvalue_node *leftValue() {
			return _leftValue;
		}

		void accept(basic_ast_visitor *sp, int level) {
			sp->do_address_of_node(this, level);
		}
	};
} // m19
#endif
