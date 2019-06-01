%option c++ prefix="m19_scanner_" outfile="m19_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{
// make relevant includes before including the parser's tab file
#include <string>
#include <sstream>
#include <algorithm>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "m19_parser.tab.h"
#include <map>

namespace {

    // output stream for building string literals
    static std::ostringstream strlit;

    //instead of hardcoding rules for every possible char, use a map
    static std::map<char, std::string> exponents = {
        {'a', "e-18"},
        {'f', "e-15"},
        {'p', "e-12"},
        {'n', "e-9"},
        {'u', "e-6"},
        {'m', "e-3"},
        {'k', "e+3"},
        {'M', "e+6"},
        {'G', "e+9"},
        {'T', "e+12"},
        {'P', "e+15"},
        {'E', "e+18"}
    };


    double engNotConv(char *str, int len) {
        std::string number(str);
        
        int i = 0;
        auto exponent = exponents.find(str[i]);
        
        while (exponent == exponents.end() && i < len) {
            exponent = exponents.find(str[++i]);
        }
        
        replace(number.begin(), number.end(), str[i], '.');
        
        return stod(number + exponent->second, NULL);
    }
}

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT X_STRING_SC
%%

  /* ====================================================================== */
  /* ====[                      1 - COMMENTS                          ]==== */
  /* ====================================================================== */

"//".*$                 ;
"/*"                    yy_push_state(X_COMMENT);
<X_COMMENT>"/*"         yy_push_state(X_COMMENT);
<X_COMMENT>"*/"         yy_pop_state();
<X_COMMENT>.|\n         ;

  /* ====================================================================== */
  /* ====[                      2 - Tokens p Simbolos                 ]==== */
  /* ====================================================================== */

">@<"                  return tRETURN;
">>@"                  return tSTOP;
"@>>"                  return tCONTINUE;

">>"                   return tRIGHTRIGHT;
"<<"                   return tLEFTLEFT;

">="                   return tGE;
"<="                   return tLE;
"=="                   return tEQ;
"!="                   return tNE;

"!!"		               return tEXCLEXCL;
"!"                    return tEXCL;

"@"                    return tAT;

"&&"                   return tAND;

"||"                   return tOR;

[()[\]+\-?*/%~=<>@:;#$,{}]     return *yytext;



  /* ====================================================================== */
  /* ====[                      3 - INTEIROS                          ]==== */
  /* ====================================================================== */


[0-9]+                 {
			yylval.i = strtol(yytext, nullptr, 10);
                        if(errno == ERANGE) yyerror("B10Int - OVERFLOW");
                        return tINTEGER;
                        }

0[xX][0-9a-fA-F]+      {
			yylval.i = strtol(yytext, nullptr, 16);
                        if(errno == ERANGE) yyerror("B16Int - OVERFLOW");
                        return tINTEGER;
                        }

  /* ====================================================================== */
  /* ====[                      4 - REAIS                             ]==== */
  /* ====================================================================== */

[0-9]+\.[0-9]+                      {
                                        yylval.d = strtod(yytext, NULL);
                                        if(errno == ERANGE) yyerror("REAL - OVERFLOW");
                                        return tREAL;
                                    }

[0-9]+[afpnumkMGTPE][0-9]+          {
                                        yylval.d = engNotConv(yytext, yyleng);
                                        if(errno == ERANGE) yyerror("REAL - OVERFLOW");
                                        return tREAL;
                                    }


  /* ====================================================================== */
  /* ====[                      5 - IDENTIFICADORES                   ]==== */
  /* ====================================================================== */

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

  /* ====================================================================== */
  /* ====[                      6 - CAD CAR                           ]==== */
  /* ====================================================================== */

\"                     yy_push_state(X_STRING);

<X_STRING>\"          {
                                        yylval.s = new std::string(strlit.str());
                                        strlit.str("");
                                        yy_pop_state();
                                        return tSTRING;
                                      }

<X_STRING>\\           yy_push_state(X_STRING_SC);
<X_STRING>.            strlit << *yytext;

<X_STRING_SC>n         strlit << '\n'; yy_pop_state();
<X_STRING_SC>r         strlit << '\r'; yy_pop_state();
<X_STRING_SC>t         strlit << '\t'; yy_pop_state();
<X_STRING_SC>\"        strlit << '\"'; yy_pop_state();
<X_STRING_SC>\\        strlit << '\\'; yy_pop_state();

<X_STRING_SC>[0-9a-fA-F][0-9a-fA-F]?        {
						                                  strlit << std::string(1,strtol(yytext, nullptr, 16));
                                              if(errno == ERANGE) yyerror("OVERFLOW");
                                              yy_pop_state();
					                                  }

<X_STRING_SC>.          yyerror("Error: invalid ascii character");


  /* ====================================================================== */
  /* ====[                      6 - OTHER STUFF                       ]==== */
  /* ====================================================================== */

[[:space:]]+                  ; /* ignore whitespace */

.                             yyerror("Unknown character");

%%
