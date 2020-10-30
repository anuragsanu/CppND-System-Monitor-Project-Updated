#include <string>
#include <sstream>
#include <iostream>

#include "format.h"

using std::string;
const static int num = 60;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long localTime) { 

    int hours = localTime / (num*num);
    int remainder_time = (localTime % (num* num));
    int minutes = remainder_time/num;
    int seconds = (remainder_time % num);
    std::ostringstream ostring;
    ostring << hours << ":" << minutes << ":" << seconds;
    string res = ostring.str();
    return res ;

 }
