#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "command.h"

using namespace std;

class Background {

 public:

  Background(command cmd);

  bool run();
  bool waitOnMe();

  command getCmd() const;
  int getPID() const;
  bool running() const;
  bool finished() const;
  bool terminate() const;
  bool failed() const;

  void setDone(bool tf);
  void setRan(bool tf);

 private:

  void out_redirect();
  void in_redirect();

  int pid;
  bool _running;
  bool _finished;
  bool _terminate;
  bool _failed;
  command mine;

};

#endif
