#ifndef LAB4_OPT_RULE_EVAL_PARAMETER_H
#define LAB4_OPT_RULE_EVAL_PARAMETER_H
#include <string>
#include "Token.h"
using namespace std;

class Parameter {
private:
  Token token;
public:
  Parameter(Token token) {
    this->token = token;
  }
  ~Parameter() {  }

  string getLiteral() {
    return token.getLiteral();
  }
  string getName() {
    return token.getName();
  }
  string toString() {
    return token.getLiteral();
  }
};


#endif //LAB4_OPT_RULE_EVAL_PARAMETER_H
