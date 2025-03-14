#include "processor.h"

#include "linux_parser.h"

using std::string;
using std::vector;

/// Calculate the agregate CPU utilization for the system. uses calculations
/// @return the CPU utilization as a percentage (value 0.0 - 1.0)
float Processor::Utilization() {
  // get current utlization from from parser in Jiffies
  auto idle = LinuxParser::IdleJiffies();
  auto nonIdle = LinuxParser::ActiveJiffies();
  auto total =idle + nonIdle; // more efficient not to call Jiffies()

  // need deltas to calculate current % usage
  auto totald = float(total) - float(prevTotal_);      // change in total usage
  auto idled = float(idle) - float(prevIdle_);  // change in idle usage
  auto percentage =
      (totald - idled) / totald;  // percent change in active usage

  // save previous states for next iteration
  prevIdle_ = idle;
  prevTotal_ = total;

  return percentage;  // use delta to get current usage
}