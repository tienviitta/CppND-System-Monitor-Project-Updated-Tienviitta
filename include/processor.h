#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  bool enableCpuUtilization = false;
  unsigned int cpuLastSum;
  unsigned int cpuLastIdle;
};

#endif