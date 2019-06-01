#ifndef __M19_AST_NEXT_H__
#define __M19_AST_NEXT_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {
	class next_node: public cdk::basic_node {
		//gr8 uses level but its not specified in m19
	public:
		next_node(int lineno) :
			cdk::basic_node(lineno) {}

	public:
		void accept(basic_ast_visitor *sp, int level) {
			sp->do_next_node(this, level);
		}
	};
} //m19
#endif
