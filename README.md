# CppND-System-Monitor

Starter code for System Monitor Project in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

Follow along with the classroom lesson to complete the project!

![System Monitor](images/monitor.png)

## Udacity Linux Workspace
[Udacity](https://www.udacity.com/) provides a browser-based Linux [Workspace](https://engineering.udacity.com/creating-a-gpu-enhanced-virtual-desktop-for-udacity-497bdd91a505) for students. 

You are welcome to develop this project on your local machine, and you are not required to use the Udacity Workspace. However, the Workspace provides a convenient and consistent Linux development environment we encourage you to try.

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Within the Udacity Workspace, `.student_bashrc` automatically installs ncurses every time you launch the Workspace.

If you are not using the Workspace, install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/udacity/CppND-System-Monitor-Project-Updated.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)

4. Follow along with the lesson.

5. Implement the `System`, `Process`, and `Processor` classes, as well as functions within the `LinuxParser` namespace.

6. Submit!

## Project Specification

* Basic Requirements
  + The program must build an executable system monitor.
  + The program must build without generating compiler warnings.
  + The system monitor must run continuously without error, until the user terminates the program.
  + The project should be organized into appropriate classes.
* System Requirements
  + The system monitor program should list at least the operating system, kernel version, memory utilization, total number of processes, number of running processes, and uptime.
  + **TODO**: The System class should be composed of at least one other class.??
* Processor Requirements
  + The system monitor should display the CPU utilization.
* Process Requirements
  + The system monitor should display a partial list of processes running on the system.
  + The system monitor should display the PID, user, CPU utilization, memory utilization, uptime, and command for each process.
* Suggestions
  + Calculate CPU utilization dynamically, based on recent utilization
  + Sort processes based on CPU or memory utilization
  + Make the display interactive
  + Restructure the program to use abstract classes (interfaces) and pure virtual functions
  + Port the program to another operating system
