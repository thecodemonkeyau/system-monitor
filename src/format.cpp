#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  // break it down
  int hrs, mins, secs;
  hrs = seconds / 3600;
  mins = (seconds % 3600) / 60;
  secs = (seconds % 3600) % 60;
  //   build result
  return ToPaddedString(hrs) + ":" + ToPaddedString(mins) + ":" +
         ToPaddedString(secs);
}
// convert int to string and pad if necessary
string Format::ToPaddedString(int val) {
  std::string str = "";
  if (val < 10) str += "0";
  str += std::to_string(val);
  return str;
}