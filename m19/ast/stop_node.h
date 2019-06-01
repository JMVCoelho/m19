#ifndef __M19_AST_STOP_H__
#define __M19_AST_STOP_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {
	class stop_node: public cdk::basic_node {
		//gr8 uses level, but m19 doesnt specify it
	public:
		stop_node(int lineno) :
			cdk::basic_node(lineno) {}

	public:
		void accept(basic_ast_visitor *sp, int level) {
			sp->do_stop_node(this, level);
		}
	};
} //m19
#endif
