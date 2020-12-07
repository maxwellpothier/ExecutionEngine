#ifndef LAB4_OPT_RULE_EVAL_DATABASE_H
#define LAB4_OPT_RULE_EVAL_DATABASE_H
#include "Relation.h"
#include "Scheme.h"
#include "Tuple.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Parameter.h"
#include <iostream>
#include <ostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
using namespace std;

class Database {
private:
  map<string, Relation> relations;
  ostream* out;
  vector<Predicate> queries;
  vector<Rule> rules;
  stringstream* oss = new stringstream();
  bool stable = false;

  void evaluateQueries();
  void evaluateRules();
  Relation getFirst(Relation, Relation);
  static Scheme toScheme(Predicate);
  static vector <Scheme> toScheme(vector <Predicate>);
  static Tuple toTuple(Predicate);
  static vector<Tuple> toTuple(vector <Predicate>);
  void select(Relation &relation, Scheme &query);
  void project(Relation &relation, Scheme &query);
  void rename(Relation &relation, Scheme &query);
  Relation join(Relation, Relation);
  Relation Union(Relation, Relation);
  void projectJoined(Relation &relation, Scheme &query);
public:
  Database(DatalogProgram, ostream*);
  ~Database() {
    delete oss;
  }
};


#endif //LAB4_OPT_RULE_EVAL_DATABASE_H
