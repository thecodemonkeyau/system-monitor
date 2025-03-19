#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>  // For sysconf and _SC_CLK_TCK

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  long int Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

 private:
  int pid_ = 0;
  float cpu_utlization_ = 0.0;
  long ram_usage_ = 0;
  long hertz_ = 0;

  bool IsNumeric(const std::string& s);
};

#endif
