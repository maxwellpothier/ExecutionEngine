#ifndef LAB4_OPT_RULE_EVAL_SCHEME_H
#define LAB4_OPT_RULE_EVAL_SCHEME_H
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Scheme: public vector<string> {
private:
  string name = "";
public:
  void setName(string name) {
    this->name = name;
  }
  string getName() {
    return name;
  }
};


#endif //LAB4_OPT_RULE_EVAL_SCHEME_H
