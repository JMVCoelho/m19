#ifndef __M19_AST_INDEXING_H__
#define __M19_AST_INDEXING_H__

#include <cdk/ast/lvalue_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {
	class indexing_node: public cdk::lvalue_node {
		cdk::expression_node *_base, *_index;
	public:
		indexing_node(int lineno, cdk::expression_node *base, cdk::expression_node *index) :
			cdk::lvalue_node(lineno), _base(base), _index(index) {}

	public:
		cdk::expression_node *base() {
			return _base;
		}
		cdk::expression_node *index() {
			return _index;
		}

		void accept(basic_ast_visitor *sp, int level) {
			sp->do_indexing_node(this, level);
		}
	};
} // m19
#endif