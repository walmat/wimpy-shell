#include <string.h>
#include <stdio.h>
#include "command.h"

using namespace std;

command::command(){

  i_redirect = false;
  o_redirect = false;
  _background = false;
  piped = false;

  text = "null";
  name = "null";
  i_file = "null";
  o_file = "null";
  error = "null";

}

command::command(string pipe) {

  i_redirect = false;
  o_redirect = false;
  _background = false;
  piped = (pipe == "pipe") ? true : false;

  text = "null";
  name = "null";
  i_file = "null";
  o_file = "null";
  error = "null";

}


/* getters and setters*/
void command::setText(string text) {
  this->text = text;
}

string command::getText() const {
  return this->text;
}

string command::getError() const {
  return error;
}

string command::getCommand() const {
  return name;
}

string command::getInputName() const {
  return i_file;
}
string command::getOutputName() const {
  return o_file;
}

vector<string> command::getArgs() const {
  return args;
}

char** command::getArrayArgs() {

  int size = args.size();
  size += 2;

  char** a = new char* [size];

  a[0] = NULL;
  a[0] = new char[name.size() + 1];
  strcpy(a[0], name.c_str());

  for (int ctrl = 1; ctrl < (size-1); ctrl++){
    int vecCtrl = ctrl-1;
    string arg = args[ctrl];

    a[ctrl] = NULL;
    a[ctrl] = new char[arg.size() + 1];
    strcpy(a[ctrl], arg.c_str());
  }
  a[size-1] = NULL;

  return a;
  
}

bool command::redirectInput() const {
  return i_redirect;
}

bool command::redirectOutput() const {
  return o_redirect;
}

bool command::background() const {
  return _background;
}

bool command::isPiped() const {
  return piped;
}

bool command::parseText() {

  if (text == "") {
    error = "Empty.";
    return false;
  }

  string non_features = checkFeatures();
  if (non_features != "null") {
    cout << "Alert: " << endl;
    cout << "unsupported feature: " << non_features << endl;
  }

  int pos = 0;
  pos = parseSpaces(pos);

  if (pos == text.size()) {
    error = "Empty.";
    return false;
  }

  pos = parseCmd(pos);

  while (pos < text.size()) {

    if (text[pos] == ' ') {
      pos += 1;
    } else if (text[pos] == '<') {
      if (piped) {
	error = "Can't redirect and pipe";
	return false;
      }

      if (i_redirect) {
	error = "Multiple input redirects";
	return false;
      }
      i_redirect = true;
      pos = parseInputString(pos);
    } else if (text[pos] == '>') {
      if (piped) {
	error = "Can't redirect and pipe";
	return false;
      }

      if (o_redirect) {
	error = "Multiple output redirects";
	return false;
      }
      o_redirect = true;
      pos = parseOutputString(pos);
    } else if (text[pos] == '&') {
      if (piped) {
	error = "Can't pipe in background";
	return false;
      }
      _background = true;
      pos += 1;
    } else {
      pos = parseArg(pos);
    }
  }
  return true;
}

void command::reset() {

  i_redirect = false;
  o_redirect = false;
  _background = false;
  piped = false;

  text = "null";
  name = "null";
  i_file = "null";
  o_file = "null";
  error = "null";

  args.clear();
}

string command::checkFeatures() {
  int pos = 0;
  string any = "null";

  /*had to look these codes up*/
  
  while (pos < text.size()) {
    if (text[pos] == (char) 0x22) {
      //double quotes
      any = "quotes";
    }
    else if (text[pos] == (char) 0x27) {
      //single quotes
      any = "quotes";
    }
    else if (text[pos] == (char) 0x2A) {
      //wildcard
      any = "wildcard";
    }
    else if (text[pos] == (char) 0x3F) {
      //expansion
      any = "expansion";
    }
    else if (text[pos] == (char) 0x7E) {
      //wildcard
      any = "wildcard";
    }
    else if (text[pos] == (char) 0x5C) {
      any = "escapes";
    }
    pos += 1;
  }
  return any;
}

bool command::separator(int pos) {
  return (text[pos] == ' ' || text[pos] == '>' || text[pos] == '<' || text[pos] == '&') ? true : false;
}

int command::parseCmd(int pos) {

  name = "";

  while ((pos < text.size()) && (!separator(pos))) {
    name += text[pos];
    pos += 1;
  }
  return pos;
}

int command::parseArg(int pos) {

  bool init = true;
  int arg;

  while ((pos < text.size()) && (!separator(pos))) {
    if (init) {
      init = false;
      args.push_back("");
      arg = args.size()-1;
    }
    args[arg] += text[pos];
    pos+=1;
  }
  return pos;
}

int command::parseInputString(int pos) {

  i_file == "";
  pos+=1;
  pos = parseSpaces(pos);

  while ((pos < text.size()) && (!separator(pos))) {
    i_file += text[pos];
    pos+=1;
  }
  return pos;
}

int command:: parseOutputString(int pos) {

  o_file = "";
  pos+=1;
  pos = parseSpaces(pos);

  while ((pos < text.size()) && (!separator(pos))) {
    o_file += text[pos];
    pos+=1;
  }
  return pos;
}

int command::parseSpaces(int pos) {

  while (text[pos] == ' '){
    pos += 1;
  }
  return pos;

}
