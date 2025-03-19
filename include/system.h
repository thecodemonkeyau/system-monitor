#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  enum SortBy { kPid, kUser, kCpu, kMem, kTime, kCmd };
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp
  void SetOrderAsc(bool order_asc);
  bool GetOrderAsc();
  void SortProcesses(SortBy col);

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  static SortBy sort_by_;
  static bool order_asc_;

  void DoSort();
};

#endif