#include "Lexer.h"

//CONSTRUCTOR
Lexer::Lexer(string file) {
inFile.open(file.c_str());
line = 1;
lineStart = line;
parse = ' ';
}
Lexer::~Lexer() {
  inFile.close();
}

//PARSE THROUGH ALL TOKENS
Token Lexer::getToken() {
  if (inFile.eof()) {
    return tokens.back();
  }
  parse = inFile.get();
  inFile.peek();
  while (isspace(parse)) {
    if (parse == '\n') line++;
    parse = inFile.get();
    inFile.peek();
  }
  lineStart = line;
  switch (parse) {
    case ',':
      tokens.push_back(Token(lineStart, ",", "COMMA"));
      return tokens.back();
    case '.':
      tokens.push_back(Token(lineStart, ".", "PERIOD"));
      return tokens.back();
    case '?':
      tokens.push_back(Token(lineStart, "?", "Q_MARK"));
      return tokens.back();
    case '(':
      tokens.push_back(Token(lineStart, "(", "LEFT_PAREN"));
      return tokens.back();
    case ')':
      tokens.push_back(Token(lineStart, ")", "RIGHT_PAREN"));
      return tokens.back();
    case ':':
      pushCOLON();
      return tokens.back();
    case '#': {
      pushHASH();
      return Token();
    }
    case '\'': {
      pushSTRING();
      return tokens.back();
    }
    default:
      pushID();
      return tokens.back();
  }
}
vector<Token> Lexer::getTokens() {
  while (!inFile.eof()) {
    getToken();
  }
  if (tokens.back().getName() != "EOF")
    tokens.push_back(Token(line, "", "EOF"));
  return tokens;
}
void Lexer::stringHub(string id) {
  if (id == "Schemes") {
    tokens.push_back(Token(lineStart, id, "SCHEMES"));
  } else if (id == "Facts") {
    tokens.push_back(Token(lineStart, id, "FACTS"));
  } else if (id == "Rules") {
    tokens.push_back(Token(lineStart, id, "RULES"));
  } else if (id == "Queries") {
    tokens.push_back(Token(lineStart, id, "QUERIES"));
  } else {
    tokens.push_back(Token(lineStart, id, "ID"));
  }
}

//POPULATE TOKENS VECTOR
void Lexer::pushCOLON() {
  if (inFile.peek() == '-') {
    inFile.get();
    tokens.push_back(Token(lineStart, ":-", "COLON_DASH"));
  } else {
    tokens.push_back(Token(lineStart, ":", "COLON"));
  }
}
void Lexer::pushHASH() {
  string comment;
  comment += parse;
  while (inFile.peek() != '\n' && (!inFile.eof()))
    comment += inFile.get();
}
void Lexer::pushSTRING() {
  string t;
  t += parse;
  while (!inFile.eof() && inFile.peek() != '\n') {
    t += inFile.get();
    if (t.back() == '\'') break;
    inFile.peek();
  }
  if (t.back() != '\'') {
    tokens.push_back(Token(lineStart, t, "UNDEFINED"));
  } else {
    tokens.push_back(Token(lineStart, t, "STRING"));
  }
}
void Lexer::pushID() {
  if (inFile.eof()) {
    tokens.push_back(Token(lineStart, "", "EOF"));
    return;
  }
  string value;
  value += parse;
  if (!isalpha(parse)) {
    tokens.push_back(Token(lineStart, value, "UNDEFINED"));
    return;
  }
  while (isalpha(inFile.peek()) || isdigit(inFile.peek())) {
    value += inFile.get();
  }
  stringHub(value);
}