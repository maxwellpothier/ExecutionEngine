#ifndef LAB4_OPT_RULE_EVAL_PARSER_H
#define LAB4_OPT_RULE_EVAL_PARSER_H
#include <string>
#include <vector>
#include "Token.h"
#include "Predicate.h"
#include "Parameter.h"
#include "DatalogProgram.h"
using namespace std;

class Parser {
private:
  vector<Token> tokens;
  DatalogProgram program;
  Token lastToken;
  int pos;

  Token next();
  Token peek(int n = 0);
  void schemeMeta();
  void factMeta();
  void ruleMeta();
  void queryMeta();
  void parseScheme();
  void parseSchemeList();
  void parseFact();
  void parseFactList();
  void parseRule();
  void parseRuleList();
  void parseQuery();
  void parseQueryList();
  Predicate parsePredicate();
  void parsePredicateList(vector<Predicate>&);
  Parameter parseParameter();
  void parseParameterList(vector<Parameter>&);
public:
  Parser(vector<Token> tokenVector) {
    tokens = tokenVector;
    pos = -1;
    lastToken = Token();
  }
  ~Parser() {  }
  bool parse();
  DatalogProgram getProgram() {
    return program;
  }
};


#endif //LAB4_OPT_RULE_EVAL_PARSER_H
