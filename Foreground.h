#ifndef FOREGROUND_H
#define FOREGROUND_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "command.h"

using namespace std;

class Foreground {

 public:

  Foreground(command cmd);

  bool run();

 private:

  void output();
  void input();

  command c;

};

#endif
