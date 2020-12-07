#ifndef LAB4_OPT_RULE_EVAL_RELATION_H
#define LAB4_OPT_RULE_EVAL_RELATION_H
#include <set>
#include <string>
#include <iostream>
#include "Tuple.h"
#include "Scheme.h"
using namespace std;

class Relation {
private:
  string name;
  Scheme scheme;
  set<Tuple> truths;
public:
  Relation(string name, Scheme scheme) {
    this->name = name;
    this->scheme = scheme;
  }
  Relation() {
    this->name = "";
  }
  ~Relation() {  }

  //SETTERS
  void setName(string name) {
    this->name = name;
  }
  void setScheme(Scheme scheme) {
    this->scheme = scheme;
  }
  void setTruths(set<Tuple> truths) {
    this->truths = truths;
  }

  //GETTERS
  string getName() {
    return name;
  }
  Scheme getScheme() {
    return scheme;
  }
  set<Tuple> getTruths() {
    return truths;
  }
  void addTuple(Tuple truth) {
    truths.insert(truth);
  }

  //TOSTRING
  string toString() {
    stringstream oss;
    oss.str("");
    for (Tuple tuple : truths) {
      if (tuple.size() > 0)
        oss << "  ";
      for (unsigned int i = 0; i < tuple.size(); i++) {
        oss << scheme[i] << "=" << tuple.at(i);
        if (i < tuple.size() - 1) {
          oss << " ";
        }
      }
      if (tuple.size() > 0) {
        oss << endl;
      }
    }
    return oss.str();
  }
};


#endif //LAB4_OPT_RULE_EVAL_RELATION_H
