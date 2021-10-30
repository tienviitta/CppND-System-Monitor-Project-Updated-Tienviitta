#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// Return the system's kernel identifier (string)
std::string System::Kernel() {
  if (kernel_ == "") {
    kernel_ = LinuxParser::Kernel();
  }
  return kernel_;
}

// Return the system's memory utilization
float System::MemoryUtilization() {
  long totalUsed = procMeminfo_["MemTotal:"] - procMeminfo_["MemFree:"];
  long memTotal = procMeminfo_["MemTotal:"];
  return static_cast<float>(totalUsed) / static_cast<float>(memTotal);
}

// Return the operating system name
std::string System::OperatingSystem() {
  if (os_ == "") {
    // Cache OS
    os_ = LinuxParser::OperatingSystem();
    // Cache and sort processes
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();
    for (int pid : pids) {
      processes_.emplace_back(Process(pid));
    }
    std::sort(processes_.rbegin(), processes_.rend());
  }
  // Parse /proc/stat
  procStat_ = LinuxParser::ParseProcStat();
  // Parse /proc/stat
  procMeminfo_ = LinuxParser::ParseProcMeminfo();
  return os_;
}

// Return the number of processes actively running on the system
int System::RunningProcesses() {
  return static_cast<int>(procStat_["procs_running"].at(0));
}

// Return the total number of processes on the system
int System::TotalProcesses() {
  return static_cast<int>(procStat_["processes"].at(0));
}

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }