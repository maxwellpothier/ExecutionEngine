#ifndef LAB4_OPT_RULE_EVAL_PREDICATE_H
#define LAB4_OPT_RULE_EVAL_PREDICATE_H
#include <string>
#include <vector>
#include "Token.h"
#include "Parameter.h"
using namespace std;

class Predicate {
private:
  Token id;
  vector<Parameter> parameters;
public:
  Predicate(Token id = Token()) {
    this->id = id;
  }
  ~Predicate(){}

  string getIDLiteral() {
    return id.getLiteral();
  }
  vector<Parameter> getParameterList() {
    return parameters;
  }
  void setParamList(vector<Parameter> parameters) {
    this->parameters = parameters;
  }
  string toString() {
    stringstream oss;
    oss << id.getLiteral() << '(';
    for (unsigned int i=0; i < parameters.size(); i++) {
      oss << parameters[i].toString();
      if (i < parameters.size() - 1) {
        oss << ',';
      }
    }
    oss << ")";
    return oss.str();
  }
};


#endif //LAB4_OPT_RULE_EVAL_PREDICATE_H
