#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) {
  pid_ = pid;
  user_ = LinuxParser::User(pid_);
  cpu_ = LinuxParser::CpuUtilization(pid_);
  mem_ = LinuxParser::Ram(pid_);
  uptime_ = LinuxParser::UpTime(pid_);
  cmd_ = LinuxParser::Command(pid_);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_; }

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return mem_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& proc) const {
  return (cpu_ < proc.cpu_);
}