#ifndef __M19_AST_FUNCTION_DEFINITION_H__
#define __M19_AST_FUNCTION_DEFINITION_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
// #include "ast/block_node.h"

namespace m19 {

	class function_definition_node: public cdk::basic_node {
		int _qualifier; //2 public, 1 private, 0 external
		basic_type *_type;
		std::string _identifier;
		cdk::sequence_node *_arguments;
		m19::body_node *_body;
		cdk::expression_node *_default_value;

	public:
		//constructor for when ! is used to indicate that function wont return a value => type will be void
		function_definition_node(int lineno, int qualifier, const std::string &identifier, cdk::sequence_node *arguments, m19::body_node *body, cdk::expression_node *default_value) :
			cdk::basic_node(lineno), _qualifier(qualifier), _type(new basic_type(0, basic_type::TYPE_VOID)), _identifier(identifier), _arguments(arguments), _body(body), _default_value(default_value) {}

		//not void, passed type
		function_definition_node(int lineno, int qualifier, basic_type *type, const std::string &identifier, cdk::sequence_node *arguments, m19::body_node *body, cdk::expression_node *default_value) :
			cdk::basic_node(lineno), _qualifier(qualifier), _type(type), _identifier(identifier), _arguments(arguments), _body(body), _default_value(default_value) {}

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
		m19::body_node *body() {
			return _body;
		}
		cdk::expression_node *default_value() {
			return _default_value;
		}

		void accept(basic_ast_visitor *sp, int level) {
			sp->do_function_definition_node(this, level);
		}
	};
} // m19
#endif
