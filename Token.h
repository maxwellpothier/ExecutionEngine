#ifndef LAB4_OPT_RULE_EVAL_TOKEN_H
#define LAB4_OPT_RULE_EVAL_TOKEN_H
#include <string>
#include <sstream>
#include <ostream>
using namespace std;

class Token {
private:
  int line;
  string literal;
  string name;
public:
  Token(int line = -1, string literal = "", string name = "UNDEFINED") {
    this->line = line;
    this->literal = literal;
    this->name = name;
  }
  ~Token() {  }

  int getLine() {
    return line;
  }
  string getLiteral() {
    return literal;
  }
  string getName() {
    return name;
  }
  bool isType(string name) {
    if (name == this->name) return true;
    return false;
  }
};


#endif //LAB4_OPT_RULE_EVAL_TOKEN_H
