#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class command {

 public:

  command();
  command(string pipe);

  void setText(string text);

  string getText() const;
  string getError() const;
  string getCommand() const;
  string getInputName() const;
  string getOutputName() const;
  vector<string> getArgs() const;
  char** getArrayArgs();

  bool redirectInput() const;
  bool redirectOutput() const;
  bool background() const;
  bool isPiped() const;

  bool parseText();
  void reset();

 private:

  string checkFeatures();
  bool separator(int pos);

  int parseCmd(int pos);
  int parseArg(int pos);
  int parseOutputString(int pos);
  int parseInputString(int pos);
  int parseSpaces(int pos);

  bool i_redirect;
  bool o_redirect;
  bool _background;
  bool piped;

  string text;

  string name;
  string i_file;
  string o_file;
  string error;

  vector<string> args;

};

#endif
