#ifndef __M19_AST_BODY_H__
#define __M19_AST_BODY_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

	class body_node: public cdk::basic_node {
    m19::initial_section_node *_initialSection;
    cdk::sequence_node *_sections;
    m19::final_section_node *_finalSection;

	public:
		body_node(int lineno, m19::initial_section_node *initialSection, cdk::sequence_node *sections, m19::final_section_node *finalSection) :
        	cdk::basic_node(lineno), _initialSection(initialSection), _sections(sections), _finalSection(finalSection) {}

    public:
      m19::initial_section_node *initialSection() {
        return _initialSection;
      }
    	cdk::sequence_node *sections() {
    		return _sections;
    	}
      m19::final_section_node *finalSection() {
        return _finalSection;
      }

		void accept(basic_ast_visitor *sp, int level) {
			sp->do_body_node(this, level);
		}
	};
} // m19
#endif
