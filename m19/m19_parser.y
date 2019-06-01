%{
//-- don't change any of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change any of these --- END!
%}

%union {
  int                   i;	/* integer value */
  double                d;  /* real value */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  basic_type           *type;       /* expression type */
  m19::body_node            *body;
  m19::initial_section_node *initial_section;
  m19::final_section_node   *final_section;
  m19::block_node           *block;
};

%token <i> tINTEGER
%token <d> tREAL
%token <s> tIDENTIFIER tSTRING tAT
%token tRETURN tSTOP tCONTINUE tRIGHTRIGHT tLEFTLEFT
%token tEXCLEXCL tEXCL

%right '='
%left tOR
%left tAND
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc '?' tUNARY
%nonassoc "["

%type <node> vardecl declaration function section instruction conditional loop
%type <sequence> vardecls declarations sections instructions expressions file
%type <expression> expression funcall literal
%type <lvalue> lvalue
%type <type> data_type
%type <block> block
%type <body> body
%type <initial_section> initSection
%type <final_section> finalSection
%type <s> stringLiteral

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%
file : declarations { compiler->ast($1); }
     ;

declarations :              declaration { $$ = new cdk::sequence_node(LINE, $1);     }
             | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
             ;

declaration   : vardecl ';' { $$ = $1; }
              | function    { $$ = $1; }
              ;

vardecls      : vardecl               { $$ = new cdk::sequence_node(LINE, $1);     }
              | vardecls ',' vardecl  { $$ = new cdk::sequence_node(LINE, $3, $1); }
              ;

vardecl  : data_type tIDENTIFIER tEXCL                   { $$ = new m19::variable_declaration_node(LINE, $1, *$2, 2, nullptr); delete $2; }
         | data_type tIDENTIFIER tEXCL '=' expression    { $$ = new m19::variable_declaration_node(LINE, $1, *$2, 2, $5); delete $2; }
         | data_type tIDENTIFIER '?'                     { $$ = new m19::variable_declaration_node(LINE, $1, *$2, 1, nullptr); delete $2; }
         | data_type tIDENTIFIER                         { $$ = new m19::variable_declaration_node(LINE, $1, *$2, 0, nullptr); delete $2; }
         | data_type tIDENTIFIER '=' expression          { $$ = new m19::variable_declaration_node(LINE, $1, *$2, 0, $4); delete $2; }
         ;

data_type   : '#'               { $$ = new basic_type(4, basic_type::TYPE_INT); }
            | '%'               { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
            | '$'               { $$ = new basic_type(4, basic_type::TYPE_STRING); }
            | '<' data_type '>' { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2; }
            ;

function    : tEXCL tIDENTIFIER tEXCL '(' vardecls  ')'       { $$ = new m19::function_declaration_node(LINE, 2, *$2, $5); }
            | tEXCL tIDENTIFIER tEXCL '('           ')'       { $$ = new m19::function_declaration_node(LINE, 2, *$2, new cdk::sequence_node(LINE)); }
            | tEXCL tIDENTIFIER '(' vardecls  ')'             { $$ = new m19::function_declaration_node(LINE, 1, *$2, $4); }
            | tEXCL tIDENTIFIER '('           ')'             { $$ = new m19::function_declaration_node(LINE, 1, *$2, new cdk::sequence_node(LINE)); }
            | tEXCL tIDENTIFIER '?' '(' vardecls  ')'         { $$ = new m19::function_declaration_node(LINE, 0, *$2, $5); }
            | tEXCL tIDENTIFIER '?' '(' ')'                   { $$ = new m19::function_declaration_node(LINE, 0, *$2, new cdk::sequence_node(LINE)); }
            | data_type tIDENTIFIER tEXCL '(' vardecls  ')'   { $$ = new m19::function_declaration_node(LINE, 2, $1, *$2, $5); }
            | data_type tIDENTIFIER tEXCL '('           ')'   { $$ = new m19::function_declaration_node(LINE, 2, $1, *$2, new cdk::sequence_node(LINE)); }
            | data_type tIDENTIFIER '(' vardecls  ')'         { $$ = new m19::function_declaration_node(LINE, 1, $1, *$2, $4); }
            | data_type tIDENTIFIER '('           ')'         { $$ = new m19::function_declaration_node(LINE, 1, $1, *$2, new cdk::sequence_node(LINE)); }
            | data_type tIDENTIFIER '?' '(' vardecls  ')'     { $$ = new m19::function_declaration_node(LINE, 0, $1, *$2, $5); }
            | data_type tIDENTIFIER '?' '(' ')'               { $$ = new m19::function_declaration_node(LINE, 0, $1, *$2, new cdk::sequence_node(LINE)); }
            | tEXCL tIDENTIFIER tEXCL '(' vardecls  ')' body       { $$ = new m19::function_definition_node(LINE, 2, *$2, $5, $7, nullptr); }
            | tEXCL tIDENTIFIER tEXCL '('           ')' body       { $$ = new m19::function_definition_node(LINE, 2, *$2, new cdk::sequence_node(LINE), $6, nullptr); }
            | tEXCL tIDENTIFIER '(' vardecls  ')' body             { $$ = new m19::function_definition_node(LINE, 1, *$2, $4, $6, nullptr); }
            | tEXCL tIDENTIFIER '('           ')' body             { $$ = new m19::function_definition_node(LINE, 1, *$2, new cdk::sequence_node(LINE), $5, nullptr); }
            | data_type tIDENTIFIER tEXCL '(' vardecls  ')' '=' literal body   { $$ = new m19::function_definition_node(LINE, 2, $1, *$2, $5, $9, $8); }
            | data_type tIDENTIFIER tEXCL '(' vardecls  ')' body               { $$ = new m19::function_definition_node(LINE, 2, $1, *$2, $5, $7, nullptr); }
            | data_type tIDENTIFIER tEXCL '('           ')' '=' literal body   { $$ = new m19::function_definition_node(LINE, 2, $1, *$2, new cdk::sequence_node(LINE), $8, $7); }
            | data_type tIDENTIFIER tEXCL '('           ')' body               { $$ = new m19::function_definition_node(LINE, 2, $1, *$2, new cdk::sequence_node(LINE), $6, nullptr); }
            | data_type tIDENTIFIER '(' vardecls  ')' '=' literal body         { $$ = new m19::function_definition_node(LINE, 1, $1, *$2, $4, $8, $7); }
            | data_type tIDENTIFIER '(' vardecls  ')' body                     { $$ = new m19::function_definition_node(LINE, 1, $1, *$2, $4, $6, nullptr); }
            | data_type tIDENTIFIER '('           ')' '=' literal body         { $$ = new m19::function_definition_node(LINE, 1, $1, *$2, new cdk::sequence_node(LINE), $7, $6); }
            | data_type tIDENTIFIER '('           ')' body                     { $$ = new m19::function_definition_node(LINE, 1, $1, *$2, new cdk::sequence_node(LINE), $5, nullptr); }
            ;

funcall     : tIDENTIFIER '(' expressions ')'                  { $$ = new m19::function_call_node(LINE, *$1, $3);}
            | tIDENTIFIER '('             ')'                  { $$ = new m19::function_call_node(LINE, *$1,  new cdk::sequence_node(LINE));}
            | tAT '(' expressions ')'                          { $$ = new m19::function_call_node(LINE, *$1, $3);}
            | tAT '('             ')'                          { $$ = new m19::function_call_node(LINE, *$1,  new cdk::sequence_node(LINE));}
            ;

body        : initSection sections finalSection             { $$ = new m19::body_node(LINE, $1, $2, $3); }
            | initSection sections                          { $$ = new m19::body_node(LINE, $1, $2, nullptr); }
            | initSection          finalSection             { $$ = new m19::body_node(LINE, $1, nullptr, $2); }
            | initSection                                   { $$ = new m19::body_node(LINE, $1, nullptr, nullptr); }
            |             sections finalSection             { $$ = new m19::body_node(LINE, nullptr, $1, $2); }
            |             sections                          { $$ = new m19::body_node(LINE, nullptr, $1, nullptr); }
            |                      finalSection             { $$ = new m19::body_node(LINE, nullptr, nullptr, $1); }
            ;

sections    : section                                        { $$ = new cdk::sequence_node(LINE, $1); }
            | sections section                               { $$ = new cdk::sequence_node(LINE, $2, $1); }
            ;

initSection : tLEFTLEFT block                                {$$ = new m19::initial_section_node(LINE, $2);}
            ;

finalSection : tRIGHTRIGHT block                             {$$ = new m19::final_section_node(LINE, $2);}
             ;

section     : '[' expression ']' block                       {$$ = new m19::conditional_section_node(LINE, $2, 1, $4);}
            | '['            ']' block                       {$$ = new m19::conditional_section_node(LINE, nullptr, 1, $3);}
            | '(' expression ')' block                       {$$ = new m19::conditional_section_node(LINE, $2, 0, $4);}
            | '('            ')' block                       {$$ = new m19::conditional_section_node(LINE, nullptr, 0, $3);}
            | block                                          {$$ = new m19::conditional_section_node(LINE, nullptr, 0, $1);}
            ;

block       : '{' declarations instructions '}'             {$$ = new m19::block_node(LINE, $2, $3);}
            | '{' declarations              '}'             {$$ = new m19::block_node(LINE, $2, new cdk::sequence_node(LINE));}
            | '{'              instructions '}'             {$$ = new m19::block_node(LINE, new cdk::sequence_node(LINE), $2);}
            | '{'                           '}'             {$$ = new m19::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE));}
            ;

instructions    : instruction                { $$ = new cdk::sequence_node(LINE, $1); }
                | instructions instruction   { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

instruction     : expression ';'              { $$ = new m19::evaluation_node(LINE, $1);}
                | lvalue '=' expression       { $$ = new cdk::assignment_node(LINE, $1, $3); }
                | expression tEXCL            { $$ = new m19::print_node(LINE, $1, false); }
                | expression tEXCLEXCL        { $$ = new m19::print_node(LINE, $1, true);}
                | tSTOP                       { $$ = new m19::stop_node(LINE);}
                | tCONTINUE                   { $$ = new m19::next_node(LINE);}
                | tRETURN                     { $$ = new m19::return_node(LINE);}
                | conditional                 { $$ = $1;}
                | loop                                                  { $$ = $1;}
                | block                                                 { $$ = $1;}
                ;

conditional     : '[' expression ']' '#' instruction                   { $$ = new m19::if_node(LINE, $2, $5); }
                | '[' expression ']' '?' instruction ':' instruction   { $$ = new m19::if_else_node(LINE, $2, $5, $7); }
                | '[' expression ']' '?' instruction                   { $$ = new m19::if_node(LINE, $2, $5); }
                ;

loop            : '[' vardecls    ';' expressions ';' expressions ']' instruction    { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
                | '[' vardecls    ';' expressions ';'             ']' instruction    { $$ = new m19::for_node(LINE, $2, $4, new cdk::sequence_node(LINE), $7); }
                | '[' vardecls    ';'             ';' expressions ']' instruction    { $$ = new m19::for_node(LINE, $2, new cdk::sequence_node(LINE), $5, $7); }
                | '[' vardecls    ';'             ';'             ']' instruction    { $$ = new m19::for_node(LINE, $2, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), $6); }
                | '[' expressions ';' expressions ';' expressions ']' instruction    { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
                | '[' expressions ';' expressions ';'             ']' instruction    { $$ = new m19::for_node(LINE, $2, $4, new cdk::sequence_node(LINE), $7); }
                | '[' expressions ';'             ';' expressions ']' instruction    { $$ = new m19::for_node(LINE, $2, new cdk::sequence_node(LINE), $5, $7); }
                | '[' expressions ';'             ';'             ']' instruction    { $$ = new m19::for_node(LINE, $2, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), $6); }
                | '['             ';' expressions ';' expressions ']' instruction    { $$ = new m19::for_node(LINE, new cdk::sequence_node(LINE), $3, $5, $7); }
                | '['             ';' expressions ';'             ']' instruction    { $$ = new m19::for_node(LINE, new cdk::sequence_node(LINE), $3, new cdk::sequence_node(LINE), $6); }
                | '['             ';'             ';' expressions ']' instruction    { $$ = new m19::for_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), $4, $6); }
                | '['             ';'             ';'             ']' instruction    { $$ = new m19::for_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), new cdk::sequence_node(LINE), $5); }
                ;

expression        : literal                                 { $$ = $1; }
                  | funcall                                 { $$ = $1; }
                  | tAT                                     { $$ = new m19::read_node(LINE); }
                  | '-' expression %prec tUNARY             { $$ = new cdk::neg_node(LINE, $2); }
                  | '+' expression %prec tUNARY             { $$ = new m19::identity_node(LINE, $2); }
                  | '~' expression                          { $$ = new cdk::not_node(LINE, $2); }
                  | expression '+' expression	              { $$ = new cdk::add_node(LINE, $1, $3); }
                  | expression '-' expression	              { $$ = new cdk::sub_node(LINE, $1, $3); }
                  | expression '*' expression	              { $$ = new cdk::mul_node(LINE, $1, $3); }
                  | expression '/' expression	              { $$ = new cdk::div_node(LINE, $1, $3); }
                  | expression '%' expression	              { $$ = new cdk::mod_node(LINE, $1, $3); }
                  | expression '<' expression	              { $$ = new cdk::lt_node(LINE, $1, $3); }
                  | expression '>' expression	              { $$ = new cdk::gt_node(LINE, $1, $3); }
                  | expression tGE expression	              { $$ = new cdk::ge_node(LINE, $1, $3); }
                  | expression tLE expression               { $$ = new cdk::le_node(LINE, $1, $3); }
                  | expression tNE expression	              { $$ = new cdk::ne_node(LINE, $1, $3); }
                  | expression tEQ expression	              { $$ = new cdk::eq_node(LINE, $1, $3); }
                  | expression tAND expression              { $$ = new cdk::and_node(LINE, $1, $3); }
                  | expression tOR expression               { $$ = new cdk::or_node (LINE, $1, $3); }
                  | lvalue                                  { $$ = new cdk::rvalue_node(LINE, $1); }
                  | lvalue '?'                              { $$ = new m19::address_of_node(LINE, $1); }
                  | lvalue '=' expression                   { $$ = new cdk::assignment_node(LINE, $1, $3); }
                  | '(' expression ')'                      { $$ = $2; }
                  | '[' expression ']'                      { $$ = new m19::stack_alloc_node(LINE, $2);}
                  | tAT '=' expression                      { $$ = new cdk::assignment_node(LINE, new cdk::variable_node(LINE, $1), $3); }
                  ;

lvalue          : tIDENTIFIER                               { $$ = new cdk::variable_node(LINE, $1); }
                | expression  '[' expression ']'            { $$ = new m19::indexing_node(LINE, $1, $3);}
                ;

expressions       : expression                              { $$ = new cdk::sequence_node(LINE, $1);     }
                  | expressions ',' expression              { $$ = new cdk::sequence_node(LINE, $3, $1); }
                  ;

literal           : tINTEGER                                   { $$ = new cdk::integer_node(LINE, $1); };
                  | tREAL                                      { $$ = new cdk::double_node(LINE, $1); };
                  | stringLiteral                              { $$ = new cdk::string_node(LINE, $1);}
                  ;

stringLiteral     : stringLiteral tSTRING           { $$ = new std::string(*$1 + *$2); delete $1; delete $2;}
                  | tSTRING                  { $$ = $1;}
                  ;


%%
