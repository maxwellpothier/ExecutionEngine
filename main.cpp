#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "Database.h"
#include <string>
using namespace std;

int main(int argc, char* args[]) {
  ofstream outFile;
  outFile.open(args[2]);
  Lexer lx(args[1]);
  vector<Token> tokens = lx.getTokens();
  Parser parser = Parser(tokens);
  parser.parse();
  Database db = Database(parser.getProgram(), &outFile);

  return 0;
}
