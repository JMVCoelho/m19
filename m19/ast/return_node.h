#ifndef __M19_AST_RETURN_H__
#define __M19_AST_RETURN_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {
	class return_node: public cdk::basic_node {
		//gr8 uses retrival, but no arg is passed to return in m19
	public:
		return_node(int lineno) :
			cdk::basic_node(lineno) {}

	public:
		void accept(basic_ast_visitor *sp, int level) {
			sp->do_return_node(this, level);
		}
	};
} //m19
#endif
