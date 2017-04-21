#include "wsh.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

using namespace std;

/*

Empty Constructor of a wsh class

*/

wsh::wsh() {}

void wsh::start(){

  cout << "Initializing wsh..." << endl;

  while (!cin.eof()){

    line.reset();

    cout << "wsh: ";

    string input;
    getline(cin, input);
    line.setText(input);
    
    manager.update();

    pCommand p;

    p.setText(line.getText());

    //check if it was a piped command
    if (p.isPiping()){

      if (!p.parsePipe()) {
	cout << "Unable to read: " << endl;
	cout << " " << p.getError() << endl;
      } else {
	//no error in command parsing

	if (!runCommands()) {

	  //must be background or foreground
	  if (line.background()) {
	    manager.create(line);
	  } else {
	    Foreground run(line);
	    run.run();
	  }	  
	}
      }
    }
    manager.update();
    manager.print();
    manager.clear();
  } //end main
  line.reset();
}

bool wsh::runCommands() {

  if (line.getText() == "exit") {
    exit(0);
  }

  if (line.getText() == "cd") {
    runChange();
    return true;
  }

  if (line.getText() == "wait") {
    runPause();
    return true;
  }
  return false;
}


void wsh::runChange() {

  //error handling, no directory given.
  if (line.getArgs().empty()) {

    cout << "Unable to change directory: " << endl;
    cout << "Please provide a valid directory." << endl;

  } else { //attempt to change directory

    int success = chdir(line.getArgs()[0].c_str());

    if (success < 0) {
      cout << "Unable to change directory: " << endl;

      if (2 == errno) {
	cout << "Directory doesn't exist." << endl;
      } else {
	cout << strerror(errno) << "." << endl;
      }
    }
  }

  char* p = NULL;
  p = new char [MAX_SIZE];

  getcwd(p, MAX_SIZE);

  string dir = static_cast<const char*>(p);
  cout << "Your working directory is: " << dir << endl;

  delete[] p;
  p = NULL;
  
}

void wsh::runPause() {

  if (line.getArgs().size() == 0) {
    cout << "Unable to wait for job: " << endl;
    cout << "Please provide a valid job number." << endl;
  } else {
    int num = atoi(line.getArgs()[0].c_str());
    manager.wait(num);
  }
}

