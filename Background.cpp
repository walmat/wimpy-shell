#include "Background.h"

using namespace std;

Background::Background(command cmd) {
  mine = cmd;

  _running = false;
  _terminate = false;
  _finished = false;
  _failed = false;

  pid = -1;

}

bool Background::run() {

  int _pid = fork();

  if (pid < 0) {
    cout << "Error: " << endl;
    cout << "Could not run process." << endl;
    _failed = true;
    return false;
  }

  if (0 == pid) {
    if (mine.redirectInput()) {
      in_redirect();
    }

    if (mine.redirectOutput()) {
      out_redirect();
    }

    execvp(mine.getCommand().c_str(), mine.getArrayArgs());

    _failed = true;
    cout << "Error: " << endl;

    if (2 == errno) {
      cout << "Command unknown." << endl;
    } else {
      cout << strerror(errno) << "." << endl;
    }

    cout << "wsh: ";
    exit(-1);
  }

  pid = _pid;
  _running = true;
  return true;
}

bool Background::waitOnMe() {
  int success = waitpid(pid, NULL, 0);

  if (-1 == success) {
    cout << "Unable to wait for this job." << endl;
    cout << strerror(errno) << "." << endl;
    return false;
  }
  setDone(true);
  return true;
}

command Background::getCmd() const {
  return mine;
}

int Background::getPID() const {
  return pid;
}

bool Background::running() const {
  return _running;
}

bool Background::finished() const {
  return _finished;
}

bool Background::terminate() const {
  return _terminate;
}

bool Background::failed() const {
  return _failed;
}

void Background::setDone(bool tf) {
  _running = false;
  _finished = tf;
  _terminate = false;
}

void Background::setRan(bool tf) {
  _running = false;
  _finished = false;
  _terminate = tf;
}

void Background::out_redirect() {

  char* p = NULL;
  p = new char [mine.getOutputName().size() + 1];
  strcpy(p, mine.getOutputName().c_str());

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

void Background::in_redirect() {
  char* p = NULL;
  p = new char [mine.getInputName().size() + 1];
  strcpy(p, mine.getInputName().c_str());

  int file = open(p, O_RDONLY);

  if (-1 == file) {
    cout << "Input error: " <<endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }

  int success =dup2(file, 0);

  if (-1 == success) {
    cout << "Input redirect error: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }

  close(file);

  if (-1 == success) {
    cout << "Input error: " <<endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }
}
