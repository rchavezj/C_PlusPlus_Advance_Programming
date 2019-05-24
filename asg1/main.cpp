// $Id: main.cpp,v 1.54 2016-06-14 18:19:17-07 - - $

#include <cassert>
#include <deque>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
using namespace std;

#include <unistd.h>

#include "bigint.h"
#include "debug.h"
#include "iterstack.h"
#include "libfns.h"
#include "scanner.h"
#include "util.h"

using bigint_stack = iterstack<bigint>;

void do_arith (bigint_stack& stack, const char oper) {
   if (stack.size() < 2) throw ydc_exn ("stack empty");
   bigint right = stack.top();
   stack.pop();
   DEBUGF ('d', "right = " << right);
   bigint left = stack.top();
   stack.pop();
   DEBUGF ('d', "left = " << left);
   bigint result;
   switch (oper) {
      case '+': result = left + right; break;
      case '-': result = left - right; break;
      case '*': result = left * right; break;
      case '/': result = left / right; break;
      case '%': result = left % right; break;
      case '^': result = pow (left, right); break;
      default: throw invalid_argument ("do_arith operator "s + oper);
   }
   DEBUGF ('d', "result = " << result);
   stack.push (result);
}

void do_clear (bigint_stack& stack, const char) {
   DEBUGF ('d', "");
   stack.clear();
}


void do_dup (bigint_stack& stack, const char) {
   bigint top = stack.top();
   DEBUGF ('d', top);
   stack.push (top);
}

void do_printall (bigint_stack& stack, const char) {
   for (const auto& elem: stack) cout << elem << endl;
}

void do_print (bigint_stack& stack, const char) {
   cout << stack.top() << endl;
}

void do_debug (bigint_stack& stack, const char) {
   (void) stack; // SUPPRESS: warning: unused parameter 'stack'
   cout << "Y not implemented" << endl;
}

class ydc_quit: public exception {};
void do_quit (bigint_stack&, const char) {
   throw ydc_quit();
}

using function_t = void (*)(bigint_stack&, const char);
using fn_hash = unordered_map<string,function_t>;
fn_hash do_functions = {
   {"+"s, do_arith},
   {"-"s, do_arith},
   {"*"s, do_arith},
   {"/"s, do_arith},
   {"%"s, do_arith},
   {"^"s, do_arith},
   {"Y"s, do_debug},
   {"c"s, do_clear},
   {"d"s, do_dup},
   {"f"s, do_printall},
   {"p"s, do_print},
   {"q"s, do_quit},
};


//
// scan_options
//    Options analysis:  The only option is -Dflags. 
//
void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            error() << "-" << static_cast<char> (optopt)
                    << ": invalid option" << endl;
            break;
      }
   }
   if (optind < argc) {
      error() << "operand not permitted" << endl;
   }
}


//
// Main function.
//
int main (int argc, char** argv) {
   exec::execname (argv[0]);
   scan_options (argc, argv);
   bigint_stack operand_stack;
   scanner input;
   try {
      for (;;) {
         try {
            token lexeme = input.scan();
            switch (lexeme.symbol) {
               case tsymbol::SCANEOF:
                  throw ydc_quit();
                  break;
               case tsymbol::NUMBER:
                  operand_stack.push (bigint (lexeme.lexinfo));
                  break;
               case tsymbol::OPERATOR: {
                  fn_hash::const_iterator fn
                           = do_functions.find (lexeme.lexinfo);
                  if (fn == do_functions.end()) {
                     throw ydc_exn (octal (lexeme.lexinfo[0])
                                    + " is unimplemented");
                  }
                  fn->second (operand_stack, lexeme.lexinfo.at(0));
                  break;
                  }
               default:
                  assert (false);
            }
         }catch (ydc_exn& exn) {
            cout << exn.what() << endl;
         }
      }
   }catch (ydc_quit&) {
      // Intentionally left empty.
   }
   return exec::status();
}

