// $Id: hello.cpp,v 1.1 2016-03-22 14:37:53-07 - - $

#include <iostream>
#include <string>
using namespace std;

#include "hello.h"

hello::hello() {
   cout << this << "->hello::hello()" << endl;
}

hello::hello (const hello& that): message (that.message) {
   cout << this << "->hello::hello (const hello&)" << endl;
}

hello& hello::operator= (const hello&) {
   cout << this << "->hello& hello::operator= (const hello&)" << endl;
   return *this;
}

hello::hello (hello&&) {
   cout << this << "->hello::hello (hello&&)" << endl;
}

hello& hello::operator= (hello&&) {
   cout << this << "->hello& hello::operator= (hello&&)" << endl;
   return *this;
}

hello::~hello() {
   cout << this << "->hello::~hello()" << endl;
}

hello::hello (const string& message): message(message) {
   cout << this << "->hello::hello (" << message << ")" << endl;
}

void hello::say (ostream& out) {
   out << message << endl;
}

