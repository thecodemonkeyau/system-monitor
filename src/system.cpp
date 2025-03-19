#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// static member so we can update and retain the sort order
System::SortBy System::sort_by_{System::SortBy::kCpu};
bool System::order_asc_{false};

/// @brief Return the system's CPU
/// @return system's CPU
Processor& System::Cpu() { return cpu_; }

/// @brief fetches a container composed of the system's Processes
/// @return a container composed of the system's Processes
vector<Process>& System::Processes() {
  auto pids = LinuxParser::Pids();
  processes_.clear();
  for (auto pid : pids) {
    Process process(pid);
    processes_.push_back(process);
  }
  SortProcesses(sort_by_);
  return processes_;
}

/// Return the system's kernel identifier (string)
/// @return kernel identifier
std::string System::Kernel() { return LinuxParser::Kernel(); }

/// Return the system's memory utilization
/// @return memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

/// Return the operating system name
/// @return operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

/// Return the number of processes actively running on the system
/// @return number of running processes
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

/// Return the total number of processes on the system
/// @return total number of processes
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

/// Return the number of seconds since the system started running
/// @return number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

void System::SetOrderAsc(bool order_asc) { order_asc_ = order_asc; }
bool System::GetOrderAsc() { return order_asc_; }

void System::SortProcesses(SortBy col) {
  // order_asc_ = (sort_by_ == col);  // if you choose the same column again,
  // reverse the order
  sort_by_ = col;  // sort order changed, remember this
  DoSort();
}

void System::DoSort() {
  switch (sort_by_) {
    case SortBy::kCpu:
      std::sort(processes_.begin(), processes_.end(),
                [](Process& a, Process& b) {
                  if (order_asc_) return a < b;
                  return a > b;
                });
      break;
    case SortBy::kUser:
      std::sort(processes_.begin(), processes_.end(),
                [](Process& a, Process& b) {
                  if (order_asc_) return a.User() < b.User();
                  return a.User() > b.User();
                });
      break;
    case SortBy::kMem:
      // need to convert to numeric values for sorting
      std::sort(processes_.begin(), processes_.end(),
                [](Process& a, Process& b) {
                  if (order_asc_) return a.Ram() < b.Ram();
                  return a.Ram() > b.Ram();
                });
      break;
    case SortBy::kTime:
      std::sort(processes_.begin(), processes_.end(),
                [](Process& a, Process& b) {
                  if (order_asc_) return a.UpTime() < b.UpTime();
                  return a.UpTime() > b.UpTime();
                });
      break;
    case SortBy::kPid:
      std::sort(processes_.begin(), processes_.end(),
                [](Process& a, Process& b) {
                  if (order_asc_) return a.Pid() < b.Pid();
                  return a.Pid() > b.Pid();
                });
      break;
    case SortBy::kCmd:
      std::sort(processes_.begin(), processes_.end(),
                [](Process& a, Process& b) {
                  if (order_asc_) return a.Command() < b.Command();
                  return a.Command() > b.Command();
                });
      break;
  }
}
