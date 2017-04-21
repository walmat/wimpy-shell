#ifndef PIPECOMMAND_H
#define PIPECOMMAND_H

#include <string>
#include <vector>
#include <iostream>

#include "command.h"

using namespace std;

class pCommand {

 public:

  pCommand(); //constructor

  void setText(string newCmd);

  string getText() const;
  string getError() const;
  vector<command> getCmd() const;

  bool isPiping();
  bool parsePipe();

 private:

  string text;
  string error;

  vector<command> commands;

};

#endif
