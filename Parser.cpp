#include "Parser.h"

bool Parser::parse() {
  try {
    schemeMeta();
    factMeta();
    ruleMeta();
    queryMeta();
    if (!peek().isType("EOF")) {
      throw next();
    }
    next();
  } catch (Token& lastToken) {
    this->lastToken = lastToken;
    return false;
  } catch (out_of_range& oor) {
    this->lastToken = tokens.back();
    return false;
  }
  if (lastToken.isType("UNDEFINED")) {
    this->lastToken = tokens.back();
  }
  return true;
}

Token Parser::next() {
  if (pos + 1 >= tokens.size()) throw tokens.at(pos);
  pos++;
  if (tokens.at(pos).isType("COMMENT")) return next();
  return tokens.at(pos);
}
Token Parser::peek(int n) {
  if ((pos + n + 1) >= tokens.size()) throw tokens.at(pos + n + 1);
  if (tokens.at(pos + n + 1).isType("COMMENT")) return peek(n + 1);
  return tokens.at(pos + n + 1);
}

//MAKE SURE THEY ARE IN ORDER
void Parser::schemeMeta() {
  if (!peek().isType("SCHEMES")) throw next();
  next();
  if (!peek().isType("COLON")) throw next();
  next();
  parseSchemeList();
}
void Parser::factMeta() {
  if (!peek().isType("FACTS")) throw next();
  next();
  if (!peek().isType("COLON")) throw next();
  next();
  parseFactList();
}
void Parser::ruleMeta() {
  if (!peek().isType("RULES")) throw next();
  next();
  if (!peek().isType("COLON")) throw next();
  next();
  parseRuleList();
}
void Parser::queryMeta() {
  if (!peek().isType("QUERIES")) throw next();
  next();
  if (!peek().isType("COLON")) throw next();
  next();
  parseQueryList();
}

//PARSE FUNCTIONS
void Parser::parseScheme() {
  Predicate scheme = parsePredicate();
  program.addScheme(scheme);
  return;
}
void Parser::parseSchemeList() {
  parseScheme();
  if (!peek().isType("FACTS")) {
    parseSchemeList();
  }
  return;
}
void Parser::parseFact() {
  Predicate fact = parsePredicate();
  if (!peek().isType("PERIOD")) throw next();
  next();
  program.addFact(fact);
  return;
}
void Parser::parseFactList() {
  if (!peek().isType("RULES")) {
    parseFact();
    parseFactList();
  }
  return;
}
void Parser::parseRule() {
  Rule rule(parsePredicate());
  if (!peek().isType("COLON_DASH")) throw next();
  next();
  vector<Predicate> ruleList;
  parsePredicateList(ruleList);
  rule.setPredicateList(ruleList);
  if (!peek().isType("PERIOD")) throw next();
  next();
  program.addRule(rule);
  return;
}
void Parser::parseRuleList() {
  if (!peek().isType("QUERIES")) {
    parseRule();
    parseRuleList();
  }
  return;
}
void Parser::parseQuery() {
  Predicate query = parsePredicate();
  if (!peek().isType("Q_MARK")) throw next();
  next();
  program.addQuery(query);
  return;
}
void Parser::parseQueryList() {
  parseQuery();
  if (!peek().isType("EOF")) {
    parseQueryList();
  }
  return;
}
Predicate Parser::parsePredicate() {
  if (!peek().isType("ID")) throw next();
  Predicate predicate = Predicate(next());
  if (!peek().isType("LEFT_PAREN")) throw next();
  next();
  vector<Parameter> pList;
  parseParameterList(pList);
  predicate.setParamList(pList);
  if (!peek().isType("RIGHT_PAREN")) throw next();
  next();
  return predicate;
}
void Parser::parsePredicateList(vector<Predicate>& predicateVector) {
  predicateVector.push_back(parsePredicate());
  if (peek().isType("COMMA")) {
    next();
    parsePredicateList(predicateVector);
  }
}
Parameter Parser::parseParameter() {
  if (peek().isType("STRING")) program.addToDomain(peek().getLiteral());
  if (!peek().isType("STRING") && !peek().isType("ID")) throw next();
  return Parameter(next());
}
void Parser::parseParameterList(vector<Parameter>& pList) {
  pList.push_back(parseParameter());
  if (peek().isType("COMMA")) {
    next();
    parseParameterList(pList);
  }
  return;
}