#ifndef LAB4_OPT_RULE_EVAL_TUPLE_H
#define LAB4_OPT_RULE_EVAL_TUPLE_H
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Tuple: public vector<string> {
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


#endif //LAB4_OPT_RULE_EVAL_TUPLE_H
