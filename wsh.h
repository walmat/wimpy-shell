#ifndef WSH_H
#define WSH_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "jManager.h"
#include "pManager.h"
#include "command.h"
#include "pCommand.h"
#include "Foreground.h"

const int MAX_SIZE = 256;

class wsh {

 public:

  wsh(); //constructor

  void start(); //run shell

 private:

  bool runCommands();
  void runChange();
  void runPause();

  jManager manager;
  command line;

};

#endif
