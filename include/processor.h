#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  // Previous total CPU time
  long p_total = 0;
  // Previous idle CPU time
  long p_idle = 0;
};

#endif