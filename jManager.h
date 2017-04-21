#ifndef MANAGER_H
#define MANAGER_H

#include "command.h"
#include "Background.h"
#include <vector>
#include <iostream>

#include <sys/types.h>
#include <sys/wait.h>

class jManager {

 public:

  jManager(); //constructor

  void create(command cmd);
  bool wait(int job);

  void update();
  void print();
  void clear();

 private:

  int numToVector(int job);
  int vectorToNum(int index);

  vector<Background> jobs;

  int running;
  int finished;

};

#endif
