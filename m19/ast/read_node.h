#ifndef __m19_AST_READ_H__
#define __m19_AST_READ_H__

#include <cdk/ast/expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class read_node: public cdk::expression_node {
  public:
    read_node(int lineno) :
        cdk::expression_node(lineno) {}

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // m19

#endif
