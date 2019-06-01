#ifndef __M19_AST_CONDITIONAL_SECTION_H__
#define __M19_AST_CONDITIONAL_SECTION_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include "ast/block_node.h"

namespace m19 {

  class conditional_section_node: public cdk::basic_node {
    cdk::expression_node *_condition;
    int _qualifier;  //1 is exclusive, 0 is inclusive
    m19::block_node *_block;

  public:
    conditional_section_node(int lineno, cdk::expression_node *condition, int qualifier, m19::block_node *block) :
      cdk::basic_node(lineno), _condition(condition), _qualifier(qualifier), _block(block) {}

  public:
    cdk::expression_node *condition()  {
      return _condition;
    }
    int qualifier() {
      return _qualifier;
    }
    m19::block_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_conditional_section_node(this, level);
    }
  };
} // m19
#endif
