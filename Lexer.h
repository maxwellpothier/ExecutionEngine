#ifndef LAB4_OPT_RULE_EVAL_LEXER_H
#define LAB4_OPT_RULE_EVAL_LEXER_H
#include <string>
#include <fstream>
#include <vector>
#include <string>
#include "Token.h"
using namespace std;

class Lexer {
private:
  int parse;
  ifstream inFile;
  int line;
  int lineStart;
  vector<Token> tokens;
public:
  Lexer(string file);
  ~Lexer();
  Token getToken();
  vector <Token> getTokens();
  void stringHub(string);
  void pushCOLON();
  void pushHASH();
  void pushSTRING();
  void pushID();
};


#endif //LAB4_OPT_RULE_EVAL_LEXER_H
