#ifndef __M19_AST_VARIABLE_DECLARATION_H__
#define __M19_AST_VARIABLE_DECLARATION_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>

namespace m19 {

	class variable_declaration_node: public cdk::basic_node {
		basic_type *_type;
		std::string _identifier;
    int _qualifier;
		cdk::expression_node *_initialValue;

	public:
		variable_declaration_node(int lineno, basic_type *type, const std::string &identifier, int qualifier, cdk::expression_node *initialValue) :
			cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(qualifier), _initialValue(initialValue) {}

	public:
		basic_type *type() {
			return _type;
		}
		const std::string &identifier() const {
			return _identifier;
		}
    int qualifier() {
			return _qualifier;
		}
		cdk::expression_node *initialValue() {
			return _initialValue;
		}

		void accept(basic_ast_visitor *sp, int level) {
			sp->do_variable_declaration_node(this, level);
		}
	};
} // m19
#endif
