A project for COM S 352 @ Iowa State University. Author: Matthew Wall

Compiling
-----------
   Files:
	 Makefile

   Description:
         the unix makefile contains the dependencies and commands
	 for running an environment. To "make" my project, simply
	 run the command 'make' flags: clean removes all auto-
	 generated files and autosaved files. Make will compile
	 the shell and the files needed for execution.


Executable
--------------------------------------------------
   Files:
         wsh

   Description:
         When compiled, an executable named 'wsh' will be created.
	 Simply, running the executable './wsh' will start my shell.


Entry Point
--------------------------------------------------
   Files:
         main.cpp

   Description:
         Code contained in this file is simply to create and instance
	 of my shell and start it. The rest is handled in the following
	 class.


'wsh' Class
--------------------------------------------------
   Files:
         wimpyshell.h
	 wimpyshell.cpp

   Description:
	 In this file, the entire shell exists and is running.
	 It contains one function to start the shell and then
	 relinquishes control to other classes. IMPORTANT: A
	 maximum number of 256 characters is allowed per command.

      
'command' Class
--------------------------------------------------
   Files:
         command.h
	 command.cpp

   Description:
	 This contains the data and functions for storing
	 and more importantly, parsing the command.


'pCommand' Class
--------------------------------------------------
   Files:
         pCommand.h
	 pCommand.cpp

   Description:
       	 Similarly to the command class, this one is for parsing
	 and storing piped commands. This is in some ways, a subclass
	 of command. 


'Foreground' Class
--------------------------------------------------
   Files:
         Foreground.h
	 Foreground.cpp

   Description:
	 Contains code needed for redirection and execution of a
	 foreground process.


'Background' Class
--------------------------------------------------
   Files:
         Background.h
	 Background.cpp

   Description:
         Contains code needed for redirection and execution of
	 a background process.


'jManager' Class
--------------------------------------------------
   Files:
         jManager.h
	 jManager.cpp

   Description:
         Contains and manages background processes that are run.
	 In charge of starting, managing, and waiting for background
	 processes of the shell. Each process == instance of Background

'pManager' Class
--------------------------------------------------
   Files:
         pManager.h
	 pManager.cpp

   Description:
         Contains the code needed to execute piped commands.
	 Handles creation and redirection (instance of pCommand).
