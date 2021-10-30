#ifndef SYSTEM_H
#define SYSTEM_H

#include <map>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

 private:
  Processor cpu_ = {};
  std::string os_ = "";
  std::string kernel_ = "";
  std::map<std::string, std::vector<long>> procStat_;
  std::map<std::string, long> procMeminfo_;
  std::vector<Process> processes_ = {};
};

#endif