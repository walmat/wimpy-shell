#include "Foreground.h"

using namespace std;


Foreground::Foreground(command cmd) {
  c = cmd;
}

bool Foreground::run() {

  int pid = fork();

  if (pid < 0) { //error
    cout << "Unable to execute: " << endl;
    cout << "process creation." << endl;
    return false;
  }

  if (0 == pid) { //child
    if (c.redirectInput()) {
      input();
    }

    if (c.redirectOutput()) {
      output();
    }

    execvp(c.getCommand().c_str(), c.getArrayArgs());

    cout << "Execution error: " << endl;

    if (2 == errno) {
      cout << "No command with that name." << endl;
    } else {
      cout << strerror(errno) << "." << endl;
    }
    exit(-1);
  }

  int success = waitpid(pid, NULL, 0);

  if (-1 == success) {
    cout << "Execution error: " << endl;
    cout << strerror(errno) << "." << endl;
    return false;
  }
  return true;

}

void Foreground::output() {

  char* p = NULL;
  p = new char[c.getOutputName().size()+1];
  strcpy(p, c.getOutputName().c_str());

  int file = open(p, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

  if (-1 == file) {
    cout << "Output error: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }

  int success = dup2(file, 1);

  if (-1 == success) {
    cout << "Output redirect error: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }
  close(file);

  if (-1 == success) {
    cout << "Output error: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }
}

void Foreground::input() {
  
  char* p = NULL;
  p = new char [c.getInputName().size() + 1];
  strcpy(p, c.getInputName().c_str());

  int file = open(p, O_RDONLY);

  if (-1 == file) {
    cout << "Input error: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }

  int success = dup2(file, 0);

  if (-1 == success) {
    cout << "Input redirect error: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }

  close(file);

  if (-1 == success) {
    cout << "Input error: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }
}
