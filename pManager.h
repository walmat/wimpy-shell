#ifndef PIPE_H
#define PIPE_H

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "pCommand.h"

using namespace std;

class pManager {

 public:

  pManager(pCommand cmd);

  void run();

 private:

  bool create();
  void closePipe();
  void del();

  bool createLast();
  bool createMiddle(int index);
  bool createFirst();
  void waitChildren();

  void o_redirect(int file);
  void i_redirect(int file);
  void callRun(int index);

  pCommand mine;
  vector<int*> pipes;
  vector<int> pids;

};

#endif
