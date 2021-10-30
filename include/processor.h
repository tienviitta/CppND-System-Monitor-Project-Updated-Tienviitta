#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();

 private:
  // Previous total CPU time
  long prevTotal = 0;
  // Previous idle CPU time
  long prevIdle = 0;
};

#endif