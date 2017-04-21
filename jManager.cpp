#include "jManager.h"
#include <string.h>
#include <stdio.h>

jManager::jManager() {

  running = 0;
  finished = 0;
  
}

void jManager::create(command cmd){
  jobs.push_back(Background(cmd));

  if (jobs[jobs.size()-1].run()){
    running += 1;
  }
}

bool jManager::wait(int job) {

  int index = numToVector(job);

  bool isJob = false;

  if (!(job >= jobs.size())) {
    if (jobs[job].running()) {
      isJob = true;
    }
  }

  if (isJob) {
    bool success = jobs[job].waitOnMe();

    if (success) {
      running -= 1;
      finished += 1;
    }

    return success;
  }

  cout << "Unable to wait for job: " << endl;
  cout << "Please provide a valid job number." << endl;

  return false;

}

void jManager::update() {

  int pid = waitpid(-1, NULL, WNOHANG);

  while ((pid != -1) && (pid != 0)) {
    for (int ctrl = 0; ctrl < jobs.size(); ctrl++){
      if (jobs[ctrl].getPID() == pid) {
	jobs[ctrl].setDone(true);
	running -= 1;
	finished += 1;
      }
    }
    pid = waitpid(-1, NULL, WNOHANG);
  }

  if ((-1 == pid) && (errno != 10)) { //no child found is errno(10)
    cout << "Background proccess error on pid of: " << pid << endl;
    cout << " " << strerror(errno) << "." << endl;
  }
}

void jManager::print() {

  if (running > 0) { //print running
    cout << "Running: " << endl;

    for (int ctrl = 0; ctrl < jobs.size(); ctrl++) {
      if (jobs[ctrl].running()) {
	cout << "[ " << vectorToNum(ctrl) << " ] " << jobs[ctrl].getCmd().getText() << endl;
      }
    }
  }

  if (finished > 0) {
    cout << "Finished: " << endl;

    for (int ctrl = 0; ctrl < jobs.size(); ctrl++) {
      if (jobs[ctrl].finished()) {
	cout << "[ " << vectorToNum(ctrl) << " ] " << jobs[ctrl].getCmd().getText() << endl;
      }
    }
  }
}

void jManager::clear() {

  for(int ctrl = 0; ctrl < jobs.size(); ctrl++) {
    if (jobs[ctrl].finished()) {
      jobs[ctrl].setRan(true);
      finished -= 1;
    }
  }

  if ((0 == running) && (0 == finished)) {
    jobs.clear();
  }
}

/*HELPER METHODS*/

int jManager::numToVector(int job){
  return job - 1;
}

int jManager::vectorToNum(int index){
  return index + 1;
}
