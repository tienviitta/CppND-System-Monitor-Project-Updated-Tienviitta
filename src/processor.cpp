#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

using namespace LinuxParser;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  // Get CPU times from the "cpu" line
  std::vector<std::string> cpuUtilStr = LinuxParser::CpuUtilization();

  // Convert CPU times from string to long
  long value;
  std::vector<long> cpuUtil;
  for (std::size_t i = 1; i < cpuUtilStr.size(); ++i) {
    std::istringstream(cpuUtilStr[i]) >> value;
    cpuUtil.push_back(value);
  }

  // Compute CPU utilization
  // - idle + iowait
  long idle = cpuUtil[kIdle_] + cpuUtil[kIOwait_];
  // - user + nice + system + irq + softirq + steal
  long nonIdle = cpuUtil[kUser_] + cpuUtil[kNice_] + cpuUtil[kSystem_] +
                 cpuUtil[kIRQ_] + cpuUtil[kSoftIRQ_] + cpuUtil[kSteal_];
  long total = idle + nonIdle;
  long deltaTotal = total - prevTotal;
  long deltaIdle = idle - prevIdle;
  prevTotal = total;
  prevIdle = idle;

  return static_cast<float>(deltaTotal - deltaIdle) /
         static_cast<float>(deltaTotal);
}
