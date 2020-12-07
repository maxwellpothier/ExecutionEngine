#ifndef LAB4_OPT_RULE_EVAL_RULE_H
#define LAB4_OPT_RULE_EVAL_RULE_H
#include <string>
#include <vector>
#include "Token.h"
#include "Predicate.h"
using namespace std;

class Rule {
private:
  Predicate predicate;
  vector<Predicate> predicateList;
public:
  Rule(Predicate predicate) {
    this->predicate = predicate;
  }
  ~Rule() {  }
  Predicate getHeadPredicate() {
    return predicate;
  }
  vector<Predicate> getPredicateList() {
    return predicateList;
  }
  void setPredicateList(vector<Predicate> predicateList) {
    this->predicateList = predicateList;
  }
  string toString() {
    stringstream oss;
    oss << predicate.toString() << " :- ";
    for (unsigned int i=0; i < predicateList.size(); i++) {
      oss << predicateList[i].toString();
      if (i < predicateList.size() - 1) {
        oss << ',';
      }
    }
    oss << ".\n";
    return oss.str();
  }
};


#endif //LAB4_OPT_RULE_EVAL_RULE_H
