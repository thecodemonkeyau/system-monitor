#include "format.h"

#include <string>

using std::string;

/// @brief convert seconds to HH:MM:SS format
/// @param seconds number of seconds
/// @return string representation of seconds in HH:MM:SS format
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

/// @brief convert int to string and pad if necessary
/// @param val integer to convert
/// @return string representation of val n3with leading '0' if necessary
string Format::ToPaddedString(int val) {
  std::string str = "";
  if (val < 10) str += "0";
  str += std::to_string(val);
  return str;
}