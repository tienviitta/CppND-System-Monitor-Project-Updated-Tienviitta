#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_;
  std::string uid_ = "";
  std::string user_ = "";
  float cpu_ = 0;
  std::string mem_ = "";
  long int uptime_ = 0;
  std::string cmd_ = "";
};

#endif