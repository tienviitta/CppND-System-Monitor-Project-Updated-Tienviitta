#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  // Get CPU times
  std::vector<std::string> cpuUtilizationStr = LinuxParser::CpuUtilization();
  // Convert and cpuSum CPU times from string to unsigned int
  unsigned int value;
  unsigned int cpuSum = 0;
  for (std::size_t i = 1; i < cpuUtilizationStr.size(); ++i) {
    std::istringstream(cpuUtilizationStr[i]) >> value;
    cpuSum += value;
  }
  // Compute CPU utilization
  unsigned int cpuDelta = cpuSum - cpuLastSum;
  cpuLastSum = cpuSum;
  std::istringstream(cpuUtilizationStr[4]) >> value;
  unsigned int cpuIdle = value - cpuLastIdle;
  cpuLastIdle = value;
  unsigned int cpuUsed = cpuDelta - cpuIdle;
  return static_cast<float>(cpuUsed) / static_cast<float>(cpuDelta);
}
