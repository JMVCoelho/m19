#ifndef __M19_AST_FUNCTION_DECLARATION_H__
#define __M19_AST_FUNCTION_DECLARATION_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace m19 {

	class function_declaration_node: public cdk::basic_node {
		int _qualifier; //2 public, 1 private, 0 external
		basic_type *_type;
		std::string _identifier;
		cdk::sequence_node *_arguments;

	public:
		//used ! to indicate that function wont return a value => type will be void
		function_declaration_node(int lineno, int qualifier, const std::string &identifier, cdk::sequence_node *arguments):
			cdk::basic_node(lineno), _qualifier(qualifier), _type(new basic_type(0, basic_type::TYPE_VOID)), _identifier(identifier) , _arguments(arguments) {}

		//not void, passed type
		function_declaration_node(int lineno, int qualifier, basic_type *type, const std::string &identifier, cdk::sequence_node *arguments):
			cdk::basic_node(lineno), _qualifier(qualifier), _type(type), _identifier(identifier) , _arguments(arguments) {}

	public:
		int qualifier() {
			return _qualifier;
		}
		basic_type *type() {
			return _type;
		}
		const std::string &identifier() const {
			return _identifier;
		}
		cdk::sequence_node *arguments() {
			return _arguments;
		}

		void accept(basic_ast_visitor *sp, int level) {
			sp->do_function_declaration_node(this, level);
		}
	};
} // m19

#endif
