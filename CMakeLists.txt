cmake_minimum_required(VERSION 3.17)
project(lab4_opt_rule_eval)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab4_opt_rule_eval main.cpp Database.cpp Database.h DatalogProgram.h Lexer.cpp
        Lexer.h Parameter.h Parser.cpp Parser.h Predicate.h Relation.h Rule.h Scheme.h Token.h Tuple.h)
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})