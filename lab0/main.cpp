// $Id: main.cpp,v 1.1 2016-03-22 14:37:53-07 - - $

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <libgen.h>

#include "hello.h"

int main (int argc, char** argv) {
 // Basename is used break a null-terminated 
 // string into directory and file name components. 
 string execname {basename (argv[0])};
 //define args with a set of vector strings from 
 //address pointer coommand argv[1] to argv[argc]
 vector<string> args (&argv[1], &argv[argc]);
 //cout: Standard output stream
 //print out the first command user agrument
 cout << execname << endl;  
 auto hello_ptr = make_shared<hello>();
 hello_ptr->say (cout);
 hello goodbye {"Goodbye, world!"};
 goodbye.say (cout);
 hello second {*hello_ptr};
 second.say (cout);
 for (const auto& arg: args) cout << "for: " << arg << endl;
 for_each (&argv[0], &argv[argc],
           [=] (const char* arg) {
              cout << "for_each: " << arg << endl;
           });
 return EXIT_SUCCESS;
}