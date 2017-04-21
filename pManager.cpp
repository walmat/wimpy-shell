#include "pManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

pManager::pManager(pCommand cmd) {
  mine = cmd;
}

void pManager::run() {

  create();

  if (!createLast()) return;

  int middles = mine.getCmd().size() - 2;

  for (int i = middles; i > 0; i--){
    if (!createMiddle(i)) return;
  }
  if (!createFirst()) return;

  closePipe();
  waitChildren();
  del();

  return;

}

void pManager::o_redirect(int file) {

  int success = dup2(file, 1);

  if (-1 == success) {
    cout << "Unable to use pipeline: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }
  success = close(file);

  if (-1 == success) {
    cout << "Output error: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }

}

void pManager::i_redirect(int file) {

  int success = dup2(file, 0);

  if (-1 == success) {
    cout << "Unable to use pipeline: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }
  success = close(file);

  if (-1 == success) {
    cout << "Input error: " << endl;
    cout << strerror(errno) << "." << endl;
    exit(-1);
  }

}

void pManager::callRun(int index) {
  if (index >= mine.getCmd().size()) {
    cout << "Unable to use pipeline: " << endl;
    cout << "Please provide a valid index." << endl;
    exit(-1);
  }

  execvp(mine.getCmd()[index].getCommand().c_str(), mine.getCmd()[index].getArrayArgs());

  cout << "Unable to use pipeline: " << endl;
  
  if (2 == errno) {
    cout << "Command unknown." << endl;
  } else {
    cout << strerror(errno) << "." << endl;
  }
  exit(-1);

}

bool pManager::create() {

  int num = mine.getCmd().size() - 1;

  for (int i = 0; i < num; i++) {
    int* fd = new int[2];
    pipes.push_back(fd);

    if (-1 == pipe(pipes[pipes.size()-1])) {
      cout << "Unable to create pipe: " << endl;
      cout << strerror(errno) << "." << endl;
      return false;
    }
  }
}

void pManager::closePipe() {

  for (int i = 0; i < pipes.size(); i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }
}

void pManager::del() {

  for (int i = 0; i < pipes.size(); i++) {
    delete[] pipes[i];
  }
  pipes.clear();
}

bool pManager::createLast() {

  int pid = fork();

  if (-1 == pid) {
    cout << "Error: " << endl;
    cout << "Unable to create last child." << endl;
    return false;
  }

  if (0 == pid) {

    int thisPipe = pipes.size() - 1;

    close(pipes[thisPipe][1]);
    i_redirect(pipes[thisPipe][0]);

    for (int i = 0; i < pipes.size(); i++) {

      if (i != thisPipe) {
	close(pipes[i][0]);
	close(pipes[i][1]);
      }
    }

    callRun(mine.getCmd().size() - 1);
  }
  pids.push_back(pid);
  return true;
}

bool pManager::createMiddle(int index) {

  int pid = fork();

  if (-1 == pid) {
    cout << "Error: " << endl;
    cout << "Unable to create middle child." << endl;
    return false;
  }

  if (0 == pid) {
    int out = index;
    int in = index - 1;

    i_redirect(pipes[in][0]);
    close(pipes[in][1]);

    o_redirect(pipes[out][1]);
    close(pipes[out][0]);

    for (int i = 0; i < pipes.size(); i++) {

      if ((i != in) && (i != out)) {
	close(pipes[i][0]);
	close(pipes[i][1]);
      }
    }
    callRun(index);
  }
  pids.push_back(pid);
  return true;
}

bool pManager::createFirst() {

  int pid = fork();

  if (-1 == pid) {
    cout << "Error: " << endl;
    cout << "Unable to create first child." << endl;
    return false;
  }

  if (0 == pid) {

    int thisPipe = 0;

    close(pipes[thisPipe][0]);
    o_redirect(pipes[thisPipe][1]);

    for (int i = 0; i < pipes.size(); i++) {
      if(i != thisPipe) {
	close(pipes[i][0]);
	close(pipes[i][1]);
      }
    }
    callRun(0);
  }
  pids.push_back(pid);
  return true;
}

void pManager::waitChildren() {
  for (int i = pids.size()-1; i > -1; i--) {
    waitpid(pids[i], NULL, 0);
  }
  pids.clear();
}
