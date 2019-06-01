#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void m19::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {

}
void m19::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {

}
void m19::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunction) {
    _pf.DOUBLE(node->value());
  } else {
      _pf.SDOUBLE(node->value());
  }
}
void m19::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();
}
void m19::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}
void m19::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    if (_inSections) {
      node->node(i)->accept(this, lvl);
      if (((m19::conditional_section_node*)node->node(i))->qualifier() == 1) {
        break;
      }
    } else {
      node->node(i)->accept(this, lvl);
    }
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunction) {
    _pf.INT(node->value());
  } else {
      _pf.SINT(node->value());
  }
}

void m19::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  if(_currentFunction) {
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  } else {
      _pf.DATA();
      _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

void m19::postfix_writer::do_stack_alloc_node(m19::stack_alloc_node *const node, int lvl) {
    ASSERT_SAFE_EXPRESSIONS;

    node->argument()->accept(this, lvl);
    _pf.INT(3);
    _pf.SHTL();

    _pf.ALLOC();
    _pf.SP();
}

void m19::postfix_writer::do_identity_node(m19::identity_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}


//---------------------------------------------------------------------------

void m19::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void m19::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void m19::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void m19::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void m19::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void m19::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void m19::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void m19::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void m19::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void m19::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void m19::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  const std::string &id = node->name();
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);
  if (symbol->offset() == 0) {
    _pf.ADDR(symbol->name());
  } else {
    _pf.LOCAL(symbol->offset());
  }
}

void m19::postfix_writer::do_indexing_node(m19::indexing_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->base()) {
      node->base()->accept(this, lvl);
  } else {
      if (_currentFunction) {
          _pf.LOCV(-_currentFunction->type()->size());
      } else {
          std::cerr << "FATAL" << std::endl;
      }
  }
  node->index()->accept(this, lvl);
  _pf.INT(3);
  _pf.SHTL();
  _pf.ADD();
}

void m19::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  }
  else {
    // integers, pointers, and strings
    _pf.LDINT();
  }
}

void m19::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl + 2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    if (node->rvalue()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }

  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
}

void m19::postfix_writer::do_variable_declaration_node(m19::variable_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string id = node->identifier();

  //internal offset
  int offset = 0;

  // size of the variable type
  int typesize = node->type()->size();// in bytes

  // in the body of the function
  if (_inFunction) {
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    offset = _offset;
    _offset += typesize;
  } else {
    offset = 0; // global variable
  }

  std::shared_ptr<m19::symbol> symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (_inFunction) {
    // if we are dealing with local variables, then no action is needed
    // unless an initial value exists
    if (node->initialValue()) {
      node->initialValue()->accept(this, lvl);
      if (node->type()->name() == basic_type::TYPE_INT || node->type()->name() == basic_type::TYPE_STRING
          || node->type()->name() == basic_type::TYPE_POINTER) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
        _pf.LOCAL(symbol->offset());
        _pf.STDOUBLE();
      } else {
        std::cerr << "cannot initialize" << std::endl;
      }
    }
  } else {
    if (!_currentFunction) {
      if (node->initialValue() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(id);
        _pf.SALLOC(typesize);
      } else {

        if (node->type()->name() == basic_type::TYPE_INT || node->type()->name() == basic_type::TYPE_DOUBLE
            || node->type()->name() == basic_type::TYPE_POINTER) {

          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);

          if (node->type()->name() == basic_type::TYPE_INT) {
            node->initialValue()->accept(this, lvl);
          } else if (node->type()->name() == basic_type::TYPE_POINTER) {
            node->initialValue()->accept(this, lvl);
          } else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
            if (node->initialValue()->type()->name() == basic_type::TYPE_DOUBLE) {
              node->initialValue()->accept(this, lvl);
            } else if (node->initialValue()->type()->name() == basic_type::TYPE_INT) {
              cdk::integer_node *dclini = dynamic_cast<cdk::integer_node *>(node->initialValue());
              cdk::double_node ddi(dclini->lineno(), dclini->value());
              ddi.accept(this, lvl);
            } else {
              std::cerr << node->lineno() << ": '" << id << "' has bad initial value for real value\n";
            }
          }
        } else if (node->type()->name() == basic_type::TYPE_STRING) {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);
          node->initialValue()->accept(this, lvl);
        } else {
          std::cerr << node->lineno() << ": '" << id << "' has unexpected initial value\n";
        }

      }

    }
  }
}

void m19::postfix_writer::do_address_of_node(m19::address_of_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->leftValue()->accept(this, lvl+2);
}

void m19::postfix_writer::do_block_node(m19::block_node *const node, int lvl) {
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
}

void m19::postfix_writer::do_body_node(m19::body_node *const node, int lvl) {
  if (node->initialSection()) node->initialSection()->accept(this, lvl + 2);
  _symtab.push();
  if (node->sections()) {
    _inSections = true;
    node->sections()->accept(this, lvl + 2);
    _inSections = false;
  }
  _symtab.pop();
  _symtab.push();
  if (node->finalSection()) node->finalSection()->accept(this, lvl + 2);
  _symtab.pop();
}

void m19::postfix_writer::do_initial_section_node(m19::initial_section_node *const node, int lvl) {
  node->block()->accept(this, lvl + 2);
}

void m19::postfix_writer::do_final_section_node(m19::final_section_node *const node, int lvl) {
  _pf.LABEL(mklbl(_ret_lbl));
  node->block()->accept(this, lvl + 2);
}

void m19::postfix_writer::do_conditional_section_node(m19::conditional_section_node *const node, int lvl) {
  int lbl1;
  if(node->condition()) {
    node->condition()->accept(this, lvl + 2);
    _pf.JZ(mklbl(lbl1 = ++_lbl));
  }
  node->block()->accept(this, lvl + 2);
  if(node->condition()) {
    _pf.LABEL(mklbl(lbl1));
  }

}

void m19::postfix_writer::do_stop_node(m19::stop_node *const node, int lvl) {
  std::string currEndLabel = mklbl(_stop_list.back());

  _pf.JMP(currEndLabel);

  //_next_list.pop_back();
  //_stop_list.pop_back();
}

void m19::postfix_writer::do_next_node(m19::next_node *const node, int lvl) {
  std::string currCondLabel = mklbl(_next_list.back());

  _pf.JMP(currCondLabel);
}

void m19::postfix_writer::do_return_node(m19::return_node *const node, int lvl) {
  _pf.JMP(mklbl(_ret_lbl));
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_function_call_node(m19::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  size_t argsSize = 0;
  if (node->arguments()) {
    for (int ax = node->arguments()->size(); ax > 0; ax--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ax - 1));
      arg->accept(this, lvl + 2);
      argsSize += arg->type()->size();
    }
  }
  _pf.CALL(node->identifier());
  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  std::shared_ptr<m19::symbol> symbol = _symtab.find(node->identifier());

  basic_type *type = symbol->type();

  if (type->name() == basic_type::TYPE_INT || type->name() == basic_type::TYPE_POINTER || type->name() == basic_type::TYPE_STRING) {
    _pf.LDFVAL32();
  }
  else if (type->name() == basic_type::TYPE_DOUBLE) {
    _pf.LDFVAL64();
  }
  else {
    // cannot happer!
  }
}

void m19::postfix_writer::do_function_declaration_node(m19::function_declaration_node *const node, int lvl) {
  if(_inFunction || _inFunctionArgs) {
      throw std::string("error, cannot declare function in body");
  }

  ASSERT_SAFE_EXPRESSIONS;

  if(!new_symbol()) return;
  std::shared_ptr<m19::symbol> function = new_symbol();
  _functions_to_declare.insert(function->name());
  reset_new_symbol();

}

void m19::postfix_writer::do_function_definition_node(m19::function_definition_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _currentFunction = new_symbol();
  _functions_to_declare.erase(_currentFunction->name());

  reset_new_symbol();

  _offset = 8;
  _symtab.push();

  if (node->arguments()) {
     _inFunctionArgs = true;
     for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
       cdk::basic_node *arg = node->arguments()->node(ix);
       if (arg == nullptr)
         break; // empty sequence of arguments
       arg->accept(this, 0); // the function symbol is at the top of the stack
     }
     _inFunctionArgs = false;
   }

  _pf.TEXT();
  _pf.ALIGN();

  // is public
  if (node->identifier() != "m19") {
    _pf.GLOBAL(_currentFunction->name(), _pf.FUNC());
  } else {
    _pf.GLOBAL("_main", _pf.FUNC());
  }

  _pf.LABEL(_currentFunction->name());

  size_t returnSize;
  if (node->type()->name() == basic_type::TYPE_VOID) {
    returnSize = 0;
  } else {
    returnSize = node->type()->size();
  }

  frame_size_calculator lsc(_compiler, _symtab);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize());

  _inFunction = true;


  _offset = -returnSize;

  if(node->default_value()){
     node->default_value()->accept(this, lvl);
     if(node->type()->name() != basic_type::TYPE_DOUBLE) {
       _pf.LOCAL(_offset);
       _pf.STINT();
    }
     else{
       // if (node->default_value()->type()->name() == basic_type::TYPE_INT) {
       //         _pf.I2D();
       // }
        _pf.I2D(); //should only be added if default value is integer
       _pf.LOCAL(_offset);
       _pf.STDOUBLE();
     }
   }


  int _ret_lbl = ++_lbl;
  node->body()->accept(this, lvl+4);
  if (!node->body()->finalSection()) {
    _pf.LABEL(mklbl(_ret_lbl));
  }

  if(node->type()->name() != basic_type::TYPE_VOID) {
     _pf.LOCAL(-returnSize);
     if (node->type()->name() == basic_type::TYPE_DOUBLE) {
       _pf.LDDOUBLE();
       _pf.STFVAL64();
     } else {
       _pf.LDINT();
       _pf.STFVAL32();
     }
   }

  _inFunction = false;
  _symtab.pop();

  // for early returns
  _next_list.clear();
  _stop_list.clear();

  _pf.LEAVE();
  _pf.RET();

  if (node->identifier() == "m19") {
    // declare external functions
    for (std::string s : _functions_to_declare)
      _pf.EXTERN(s);
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_evaluation_node(m19::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl); // determine the value

  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.TRASH(8); // delete the evaluated value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_VOID) {
    // do nothing
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }

}

void m19::postfix_writer::do_print_node(m19::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _functions_to_declare.insert("printi");
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _functions_to_declare.insert("printd");
    _pf.CALL("printd");
    _pf.TRASH(8); // trash double
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _functions_to_declare.insert("prints");
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  if (node->newline()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println");
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_read_node(m19::read_node * const node, int lvl) {

  if (_lvalueType == basic_type::TYPE_DOUBLE) {
    _pf.CALL("readd");
    _pf.LDFVAL64();
  }
  else if (_lvalueType == basic_type::TYPE_INT) {
    _pf.CALL("readi");
    _pf.LDFVAL32();
  }
  else {
    std::cerr << "FATAL: cannot read specified type" << std::endl;
    return;
  }
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_for_node(m19::for_node * const node, int lvl) {
 //ASSERT_SAFE_EXPRESSIONS;

 int forCond = ++_lbl;
 int forIncr = ++_lbl;
 int forEnd = ++_lbl;

 _next_list.push_back(forIncr);
 _stop_list.push_back(forEnd);

 if (node->init()) {
     node->init()->accept(this, lvl);
 }

 _pf.ALIGN();
 _pf.LABEL(mklbl(forCond));
 if (node->condition()) {
     node->condition()->accept(this, lvl);
 }

 _pf.JZ(mklbl(forEnd));

 node->block()->accept(this, lvl+2);

 _pf.ALIGN();
 _pf.LABEL(mklbl(forIncr));
 if (node->incr()) {
     node->incr()->accept(this, lvl);
 }

 _pf.JMP(mklbl(forCond));
 _pf.ALIGN();
 _pf.LABEL(mklbl(forEnd));
 //_next_list.pop_back();
 //_stop_list.pop_back();


}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_if_node(m19::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void m19::postfix_writer::do_if_else_node(m19::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_else, lbl_end;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl_else = lbl_end = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  if (node->elseblock()) {
    _pf.JMP(mklbl(lbl_end = ++_lbl));
    _pf.LABEL(mklbl(lbl_else));
    node->elseblock()->accept(this, lvl + 2);
  }
  _pf.LABEL(mklbl(lbl_end));
}
