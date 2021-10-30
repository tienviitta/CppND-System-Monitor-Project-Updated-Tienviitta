#include "format.h"

#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  // Constants
  const int SEC_PER_MIN = 60;
  const int MIN_PER_HR = 60;
  const int HR_PER_DAY = 24;

  // Computation
  long sec = seconds % SEC_PER_MIN;
  seconds /= SEC_PER_MIN;
  long min = seconds % MIN_PER_HR;
  seconds /= MIN_PER_HR;
  long hour = seconds % HR_PER_DAY;
  seconds /= HR_PER_DAY;

  // Format output HH:MM:SS
  char buffer[64];
  sprintf(buffer, "%02ld:%02ld:%02ld", hour, min, sec);
  string output(buffer);

  return output;
}