#include "Database.h"

Database::Database(DatalogProgram dataLog, ostream * out) {
  this->out = out;
  vector<Scheme> schemes = toScheme(dataLog.getSchemes());
  for (unsigned int i = 0; i < schemes.size(); i++) {
    string name = schemes[i].getName();
    relations.emplace(name, Relation(name, schemes[i]));
  }
  vector<Tuple> tuples = toTuple(dataLog.getFacts());
  for (unsigned int i = 0; i < tuples.size(); i++) {
    string name = tuples[i].getName();
    relations.at(name).addTuple(tuples[i]);
  }
  cout << "Rule Evaluation" << endl;
  rules = dataLog.getRules();
  unsigned int i;
  for (i = 0; !stable; i++) {
    evaluateRules();
  }
  cout << "\nSchemes populated after " << i << " passes through the Rules.\n" << endl;
  cout << "Query Evaluation" << endl;
  queries = dataLog.getQueries();
  evaluateQueries();
}

void Database::evaluateRules() {
  stable = true;
  for (unsigned int i = 0; i < rules.size(); i++) {
    cout << rules[i].toString();
    Scheme conclusion = toScheme(rules[i].getHeadPredicate());
    vector<Scheme> premises = toScheme(rules[i].getPredicateList());
    vector<Relation> relations;
    for (unsigned int i = 0; i < premises.size(); i++) {
      Relation relation = this->relations.at(premises[i].getName());
      select(relation, premises[i]);
      relations.push_back(relation);
    }
    Relation newRelation;
    if (premises.size() == 1) {
      newRelation = relations.at(0);
      rename(newRelation, premises.at(0));
      select(newRelation, premises.at(0));
      projectJoined(newRelation, conclusion);
      Scheme tempScheme = this->relations.at(conclusion.getName()).getScheme();
      rename(newRelation, tempScheme);
    } else {
      while (premises.size() > 1) {
        Relation relation0 = relations.at(0);
        Relation relation1 = relations.at(1);
        Scheme scheme0 = premises.at(0);
        Scheme scheme1 = premises.at(1);
        premises.erase(premises.begin());
        premises.erase(premises.begin());
        relations.erase(relations.begin());
        relations.erase(relations.begin());
        relation0.setScheme(scheme0);
        relation1.setScheme(scheme1);
        Relation natJoin = join(relation0, relation1);
        relations.push_back(natJoin);
        premises.push_back(natJoin.getScheme());
      }
      Scheme newScheme = this->relations.at(conclusion.getName()).getScheme();
      newRelation = relations.at(0);
      select(newRelation, conclusion);
      projectJoined(newRelation, conclusion);
      rename(newRelation, conclusion);
      newRelation.setScheme(newScheme);
    }
    Relation oldRelation = this->relations.at(newRelation.getName());
    newRelation = getFirst(newRelation, oldRelation);
    cout << newRelation.toString();
    if (newRelation.getTruths().size() != 0) {
      stable = false;
    }
    this->relations.erase(newRelation.getName());
    this->relations.emplace(newRelation.getName(), Union(newRelation, oldRelation));
  }
}

void Database::evaluateQueries() {
  for (unsigned int i = 0; i < queries.size(); i++) {
    oss->str("");
    Relation results = relations.at(queries[i].getIDLiteral());
    Scheme scheme = toScheme(queries[i]);
    cout << queries.at(i).toString() << "? ";
    select(results, scheme);
    project(results, scheme);
    rename(results, scheme);
    *oss << results.toString();
    if (results.getTruths().size() > 0) {
      cout << "Yes(" << results.getTruths().size() << ')' << endl;
      cout << oss->str();
    } else {
      cout << "No" << endl;
    }
    oss->clear();
  }
}

Relation Database::getFirst(Relation r0, Relation r1) {
  set<Tuple> firstTuples;
  if (r0.getName() == r1.getName()) {
    set<Tuple> newTuples = r0.getTruths();
    set<Tuple> oldTuples = r1.getTruths();
    for (Tuple tuple : newTuples) {
      if (oldTuples.find(tuple) == oldTuples.end()) {
        firstTuples.insert(tuple);
      }
    }
  } else {
    throw logic_error("relation names don't match");
  }
  r0.setTruths(firstTuples);
  return r0;
}

Scheme Database::toScheme(Predicate predicate) {
  Scheme scheme;
  scheme.setName(predicate.getIDLiteral());
  for (Parameter param : predicate.getParameterList()) {
    scheme.push_back(param.getLiteral());
  }
  return scheme;
}
vector<Scheme> Database::toScheme(vector<Predicate> predicates) {
  vector<Scheme> schemes;
  for (unsigned int i = 0; i < predicates.size(); i++) {
    schemes.push_back(toScheme(predicates[i]));
  }
  return schemes;
}

Tuple Database::toTuple(Predicate predicate) {
  Tuple tuple;
  tuple.setName(predicate.getIDLiteral());
  for (Parameter param : predicate.getParameterList()) {
    tuple.push_back(param.getLiteral());
  }
  return tuple;
}
vector<Tuple> Database::toTuple(vector<Predicate> predicates) {
  vector<Tuple> tuples;
  for (unsigned int i = 0; i < predicates.size(); i++) {
    tuples.push_back(toTuple(predicates[i]));
  }
  return tuples;
}

void Database::select(Relation &relation, Scheme &query) {
  set<Tuple> tuples = relation.getTruths();
  set<Tuple> selection;
  for (Tuple tuple : tuples) {
    bool viable = true;
    map<string, string> usedVariables;
    for (unsigned int i = 0; i < query.size(); i++) {
      if (query.at(i).at(0) == '\'' && query.at(i) != tuple.at(i)) {
        viable = false;
        break;
      } else if (query.at(i).at(0) != '\'') {
        string var = "";
        try {
          var = usedVariables.at(query.at(i));
        } catch (out_of_range &e) {
          usedVariables.emplace(query.at(i), tuple.at(i));
          var = tuple.at(i);
        }
        if (var != tuple.at(i)) {
          viable = false;
          break;
        }
      }
    }
    if (viable) {
      selection.insert(tuple);
    }
  }
  relation.setTruths(selection);
}
void Database::project(Relation &relation, Scheme &query) {
  Scheme scheme = relation.getScheme();
  set<Tuple> tuples = relation.getTruths();
  set<Tuple> newTuples;
  for (Tuple tuple : tuples) {
    set<string> usedVars;
    Tuple newTuple;
    for (unsigned int i = 0; i < query.size(); i++) {
      if (query.at(i).at(0) != '\'' && usedVars.find(query.at(i)) == usedVars.end()) {
        usedVars.insert(query.at(i));
        newTuple.push_back(tuple.at(i));
      }
    }
    newTuples.insert(newTuple);
  }
  for (unsigned int i = 0; i < query.size(); i++) {
    set<string> usedVars;
    if (query.at(i).at(0) == '\'' && usedVars.find(query.at(i)) == usedVars.end()) {
      usedVars.insert(query.at(i));
      query.erase(query.begin() + i);
      scheme.erase(scheme.begin() + i);
      i--;
    }
  }
  relation.setScheme(scheme);
  relation.setTruths(newTuples);
}
void Database::rename(Relation &relation, Scheme &query) {
  Scheme scheme = relation.getScheme();
  if (query.size() == scheme.size()) {
    for (unsigned int i = 0; i < query.size(); i++) {
      if (query[i] != scheme[i]) {
        scheme[i] = query[i];
      }
    }
  }
  relation.setScheme(scheme);
}
Relation Database::join(Relation r0, Relation r1) {
  set<Tuple> finalTuples;
  set<Tuple> tuples0 = r0.getTruths();
  set<Tuple> tuples1 = r1.getTruths();
  Scheme finalScheme;
  Scheme s0 = r0.getScheme();
  Scheme s1 = r1.getScheme();
  for (Tuple t0 : tuples0) {
    for (Tuple t1 : tuples1) {
      map<string, string> usedVars;
      Tuple newTuple;
      Scheme newScheme;
      bool viable = true;
      for (unsigned int i = 0; i < s0.size(); i++) {
        usedVars.emplace(s0.at(i), t0.at(i));
        newTuple.push_back(t0.at(i));
        newScheme.push_back(s0.at(i));
      }
      for (unsigned int i = 0; i < s1.size(); i++) {
        string value = "";
        try {
          value = usedVars.at(s1.at(i));
        } catch (out_of_range &e) {
          usedVars.emplace(s1.at(i), t1.at(i));
          newTuple.push_back(t1.at(i));
          newScheme.push_back(s1.at(i));
          value = t1.at(i);
        }
        if (value != t1.at(i)) {
          viable = false;
        }
      }
      if (viable) {
        finalScheme = newScheme;
        finalTuples.insert(newTuple);
        finalScheme = newScheme;
      }
    }
  }
  Relation finalRelation = Relation("", finalScheme);
  finalRelation.setTruths(finalTuples);
  return finalRelation;
}
Relation Database::Union(Relation r0, Relation r1) {
  set<Tuple> tuplesAll;
  if (r0.getName() == r1.getName()) {
    tuplesAll = r0.getTruths();
    set<Tuple> tuples1 = r1.getTruths();
    tuplesAll.insert(tuples1.begin(), tuples1.end());
  } else {
    throw logic_error("relation names don't match");
  }
  r0.setTruths(tuplesAll);
  return r0;
}

void Database::projectJoined(Relation &relation, Scheme &query) {
  string name = query.getName();
  Scheme scheme = relation.getScheme();
  Scheme newScheme;
  newScheme.setName(name);
  set<Tuple> tuples = relation.getTruths();
  set<Tuple> newTuples;
  vector<int> positionsToTake;
  for (string position : query) {
    if (position[0] != '\'') {
      for (unsigned int i = 0; i < scheme.size(); i++) {
        if (position == scheme.at(i)) {
          positionsToTake.push_back(i);
          newScheme.push_back(scheme.at(i));
        }
      }
    }
  }
  for (Tuple tuple : tuples) {
    Tuple newTuple;
    newTuple.setName(query.getName());
    for (int pos : positionsToTake) {
      newTuple.push_back(tuple.at(pos));
    }
    newTuple.setName(name);
    newTuples.insert(newTuple);
  }
  relation.setName(name);
  relation.setScheme(newScheme);
  relation.setTruths(newTuples);
}

