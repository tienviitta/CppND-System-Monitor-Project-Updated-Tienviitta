#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

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
  // - 0:user, 1:nice, 2:system, 3:idle, 4:iowait, 5:irq,
  // - 6:softirq, 7:steal, 8:guest, 9:guest_nice
  // idle + iowait
  long idle = cpuUtil[3] + cpuUtil[4];
  // user + nice + system + irq + softirq + steal
  long nonIdle = cpuUtil[0] + cpuUtil[1] + cpuUtil[2] + cpuUtil[5] +
                 cpuUtil[6] + cpuUtil[7];
  long total = idle + nonIdle;
  long d_total = total - this->p_total;
  long d_idle = idle - this->p_idle;
  this->p_total = total;
  this->p_idle = idle;

  return static_cast<float>(d_total - d_idle) / static_cast<float>(d_total);
}
