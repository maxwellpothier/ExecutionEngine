#ifndef LAB4_OPT_RULE_EVAL_DATALOGPROGRAM_H
#define LAB4_OPT_RULE_EVAL_DATALOGPROGRAM_H
#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include <string>
#include <vector>
#include <set>
using namespace std;

class DatalogProgram {
private:
  vector<Predicate> schemes;
  vector<Predicate> facts;
  vector<Rule> rules;
  vector<Predicate> queries;
  set<string> domain;
public:
  void addScheme(Predicate scheme) {
    schemes.push_back(scheme);
  }
  void addFact(Predicate fact) {
    facts.push_back(fact);
  }
  void addRule(Rule rule) {
    rules.push_back(rule);
  }
  void addQuery(Predicate query) {
    queries.push_back(query);
  }
  void addToDomain(string id) {
    domain.insert(id);
  }
  vector<Predicate> getSchemes() {
    return schemes;
  }
  vector<Predicate> getFacts() {
    return facts;
  }
  vector<Rule> getRules() {
    return rules;
  }
  vector<Predicate> getQueries() {
    return queries;
  }
};


#endif //LAB4_OPT_RULE_EVAL_DATALOGPROGRAM_H
