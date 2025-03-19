#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  ///< format time in HH:MM:SS
std::string ToPaddedString(int val);  ///< adds leading '0' if necessary
};  // namespace Format

#endif