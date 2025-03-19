#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  enum SortBy { kPid, kUser, kCpu, kMem, kTime, kCmd };
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();  ///< change to returning long for consistency
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();
  void SetOrderAsc(
      bool order_asc);  ///< to manipulate the sort order of the processes
  bool GetOrderAsc();
  void SortProcesses(SortBy col);

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};

  static SortBy
      sort_by_;  ///< this needs to persist when processes are reloaded
  static bool
      order_asc_;  ///< this needs to persist when processes are reloaded

  void DoSort();  ///< does the heavy lifting for the sorting
};

#endif