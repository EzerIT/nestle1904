#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <tuple>

// Converts a UTF-8 string to a UTF-32 string
std::u32string u8_to_u32(const std::string& s);

// Converts a UTF-32 string to a UTF-8 string
std::string u32_to_u8(const std::u32string& s);


// Replaces every occurence of one string with another.
// From http://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-of-a-string-with-another-string
// Parameters:
//    subject: The string to modify
//    search: The string to replace
//    replace: The repacement string
void replace_string_in_place(std::string& subject, const std::string& search, const std::string& replace);
void replace_string_in_place(std::u32string& subject, const std::u32string& search, const std::u32string& replace);

// Checks if the last characters of one string are the same as another string.
// Parameters:
//    full_string: String in which to search for at suffix.
//    ending: Suffix to search for
//
// From http://stackoverflow.com/questions/874134/find-if-string-endswith-another-string-in-c
bool ends_with(const std::string& full_string, const std::string& ending);


// Splits the string s into 6 components. The components are separated by the tab character.
std::tuple<std::string,std::string,std::string,std::string,std::string,std::string> split6(const std::string& s);

// Splits the std::string s into 7 components. The components are separated by the tab character.
std::tuple<std::string,std::string,std::string,std::string,std::string,std::string,std::string> split7(const std::string& s);

#endif // _UTIL_H
