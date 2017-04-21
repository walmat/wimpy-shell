#include "pCommand.h"

using namespace std;


pCommand::pCommand() {
  text = "null";
}

void pCommand::setText(string newCmd) {
  text = newCmd;
}

string pCommand::getText() const {
  return text;
}

string pCommand::getError() const {
  return error;
}

vector<command> pCommand::getCmd() const {
  return commands;
}

bool pCommand::isPiping() {

  int pos = text.find('|');

  if (pos == string::npos){
    return false;
  }
  return true;
}

bool pCommand::parsePipe() {


  int pos = 0;
  bool word = true;
  int cmd;

  if (text[0] == '|') {

    error = "No job.";
    return false;
  }

  while (pos < text.size()) {
    if (text[pos] != '|') {
      if (word) {
	word = false;

	commands.push_back(command("pipe"));

	cmd = commands.size()-1;

	commands[cmd].setText("");

      }
      commands[cmd].setText(commands[cmd].getText() + text[pos]);
      pos += 1;
    } else {

      pos += 1; //jump over pipe
      if (!commands[cmd].parseText()){
	error = commands[cmd].getError();

	if ("Empty" == error) {
	  error = "Unable to find job.";
	}
	return false;
      }
      word = true;
    }
  }
  if (!commands[cmd].parseText()){
    error = commands[cmd].getError();

    if ("Empty" == error) {
      error = "Unable to find a job.";
    }
    return false;
  }

  if (commands.size() < 2) {
    error = "Unable to find a job.";
    return false;
  }
  return true;
}
