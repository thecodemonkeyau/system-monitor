#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();

 private:
  int prevIdle_;   ///< for calculating CPU utilization
  int prevTotal_;  ///< for calculating CPU utilization
};

#endif