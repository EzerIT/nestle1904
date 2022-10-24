#include <algorithm>

#include "util.hpp"

using namespace std;

// Converts a UTF-8 string to a UTF-32 string
u32string u8_to_u32(const string& s)
{
    u32string result;

    int i = 0;
    while (i<s.size()) {
        unsigned char ch0 {(unsigned char)s[i]};

        if ((ch0 & 0x80) == 0) {
            // One byte
            i += 1;
            result.push_back(ch0);
        }
        else if ((ch0 & 0xe0) == 0xc0) {
            // Two bytes
            unsigned char ch1 {(unsigned char)s[i+1]};
            i += 2;
            
            result.push_back(((ch0 & 0x1f) << 6) | (ch1 & 0x3f));
        }
        else if ((ch0 & 0xf0) == 0xe0) {
            // Three bytes
            unsigned char ch1 {(unsigned char)s[i+1]};
            unsigned char ch2 {(unsigned char)s[i+2]};
            i += 3;

            result.push_back(((ch0 & 0x0f) << 12) | ((ch1 & 0x3f) << 6) | (ch2 & 0x3f));
        }
        else if ((ch0 & 0xf8) == 0xf0) {
            // Four bytes
            unsigned char ch1 {(unsigned char)s[i+1]};
            unsigned char ch2 {(unsigned char)s[i+2]};
            unsigned char ch3 {(unsigned char)s[i+3]};
            i += 4;

            result.push_back(((ch0 & 0x07) << 18) | ((ch1 & 0x3f) << 12) | ((ch2 & 0x3f) << 6) | (ch3 & 0x3f));
        }
        else
            return U"ERROR";
    }

    return result;
}


// Converts a UTF-32 string to a UTF-8 string
string u32_to_u8(const u32string& s)
{
    string result;

    for (char32_t c : s) {
        if (c<=0x7f)
            // One byte
            result.push_back(c);
        else if (c<=0x7ff) {
            // Two bytes
            result.push_back((c >> 6) | 0xc0);
            result.push_back((c & 0x3f) | 0x80);
        }
        else if (c<=0xffff) {
            // Three bytes
            result.push_back((c >> 12) | 0xe0);
            result.push_back(((c >> 6) & 0x3f) | 0x80);
            result.push_back((c & 0x3f) | 0x80);
        } 
        else if (c<=0x1fffff) {
            // Four bytes
            result.push_back((c >> 18) | 0xf0);
            result.push_back(((c >> 12) & 0x3f) | 0x80);
            result.push_back(((c >> 6) & 0x3f) | 0x80);
            result.push_back((c & 0x3f) | 0x80);
        }
        else
            return "ERROR";
    }

    return result;
}


// Replaces every occurence of one string with another.
// From http://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-of-a-string-with-another-string
// Parameters:
//    subject: The string to modify
//    search: The string to replace
//    replace: The repacement string

// We're not using a template for these two functions because then we can't use a char[] instead of
// a string as search/replace arguments.

void replace_string_in_place(string& subject, const string& search, const string& replace) {
    string::size_type pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

void replace_string_in_place(u32string& subject, const u32string& search, const u32string& replace) {
    u32string::size_type pos = 0;
    while ((pos = subject.find(search, pos)) != u32string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}


// Checks if the last characters of one string are the same as another string.
// Parameters:
//    full_string: String in which to search for at suffix.
//    ending: Suffix to search for
//
// From http://stackoverflow.com/questions/874134/find-if-string-endswith-another-string-in-c
bool ends_with(const string& full_string, const string& ending)
{
    return full_string.length()>=ending.length() && 
        full_string.compare(full_string.length()-ending.length(), ending.length(), ending)==0;
}


// Splits the string s into 6 components. The components are separated by the tab character.
tuple<string,string,string,string,string,string> split6(const string& s)
{
    auto it1 = find(begin(s),end(s),'\t');
    auto it2 = find(it1+1,end(s),'\t');
    auto it3 = find(it2+1,end(s),'\t');
    auto it4 = find(it3+1,end(s),'\t');
    auto it5 = find(it4+1,end(s),'\t');
    auto it6 = find(it5+1,end(s),'\t');

    return make_tuple(string{begin(s),it1},
                      string{it1+1,it2},
                      string{it2+1,it3}, 
                      string{it3+1,it4}, 
                      string{it4+1,it5},
                      string{it5+1,it6});
}

// Splits the string s into 7 components. The components are separated by the tab character.
tuple<string,string,string,string,string,string,string> split7(const string& s)
{
    auto it1 = find(begin(s),end(s),'\t');
    auto it2 = find(it1+1,end(s),'\t');
    auto it3 = find(it2+1,end(s),'\t');
    auto it4 = find(it3+1,end(s),'\t');
    auto it5 = find(it4+1,end(s),'\t');
    auto it6 = find(it5+1,end(s),'\t');
    auto it7 = find(it6+1,end(s),'\t');

    return make_tuple(string{begin(s),it1},
                      string{it1+1,it2},
                      string{it2+1,it3}, 
                      string{it3+1,it4}, 
                      string{it4+1,it5}, 
                      string{it5+1,it6}, 
                      string{it6+1,it7});
}
