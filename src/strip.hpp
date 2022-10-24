#ifndef _STRIP_HPP
#define _STRIP_HPP

#include <string>

// Removes non-letters from string and replaces characters with lower case unaccented variants.
// Argument:
//   s: Source string
// Returns:
//   Modified string
std::string strip_string(const std::string& s);

#endif // _STRIP_HPP
