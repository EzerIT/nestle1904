#include <array>
#include <string>

#include "oxia2tonos.hpp"

#ifdef HAS_MAIN
#include <iostream>
#include <fstream>
#include <unistd.h>
#endif

using namespace std;

array<string,11> oxia_characters {
    "\xcd\xbe",     // \u037e - GREEK QUESTION MARK
    "\xce\x87",     // \u0387 - GREEK ANO TELEIA
    "\xe1\xbd\xb1", // \u1f71 - GREEK SMALL LETTER ALPHA WITH OXIA
    "\xe1\xbd\xb3", // \u1f73 - GREEK SMALL LETTER EPSILON WITH OXIA
    "\xe1\xbd\xb5", // \u1f75 - GREEK SMALL LETTER ETA WITH OXIA
    "\xe1\xbd\xb7", // \u1f77 - GREEK SMALL LETTER IOTA WITH OXIA
    "\xe1\xbd\xb9", // \u1f79 - GREEK SMALL LETTER OMICRON WITH OXIA
    "\xe1\xbd\xbb", // \u1f7b - GREEK SMALL LETTER UPSILON WITH OXIA
    "\xe1\xbd\xbd", // \u1f7d - GREEK SMALL LETTER OMEGA WITH OXIA
    "\xe1\xbf\x93", // \u1fd3 - GREEK SMALL LETTER IOTA WITH DIALYTIKA AND OXIA
    "\xe1\xbf\xa3", // \u1fe3 - GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND OXIA
};

array<string,11> tonos_characters {
    "\x3b",         // \u003b - SEMICOLON
    "\xc2\xb7",     // \u00b7 - MIDDLE DOT
    "\xce\xac",     // \u03ac - GREEK SMALL LETTER ALPHA WITH TONOS
    "\xce\xad",     // \u03ad - GREEK SMALL LETTER EPSILON WITH TONOS
    "\xce\xae",     // \u03ae - GREEK SMALL LETTER ETA WITH TONOS
    "\xce\xaf",     // \u03af - GREEK SMALL LETTER IOTA WITH TONOS
    "\xcf\x8c",     // \u03cc - GREEK SMALL LETTER OMICRON WITH TONOS
    "\xcf\x8d",     // \u03cd - GREEK SMALL LETTER UPSILON WITH TONOS
    "\xcf\x8e",     // \u03ce - GREEK SMALL LETTER OMEGA WITH TONOS
    "\xce\x90",     // \u0390 - GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS
    "\xce\xb0",     // \u03b0 - GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS
};


// Taken from https://en.cppreference.com/w/cpp/string/basic_string/replace
size_t replace_all(string& inout, string_view what, string_view with)
{
    size_t count{};
    for (string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count)
        inout.replace(pos, what.length(), with.data(), with.length());

    return count;
}


// Returns a string in which all oxia accents have been replace by tonos accents
string oxia2tonos(string s)
{
    for (int i=0; i<oxia_characters.size(); ++i)
        replace_all(s, oxia_characters[i], tonos_characters[i]);

    return s;
}

// Returns a string in which all tonos accents have been replace by oxia accents
string tonos2oxia(string s)
{
    for (int i=0; i<tonos_characters.size(); ++i)
        replace_all(s, tonos_characters[i], oxia_characters[i]);

    return s;
}

#ifdef HAS_MAIN

static void usage(const char* progname)
{
    cerr << "Usage:\n"
         << progname << " [-o outputfile] [inputfile]\n";
}


// Main function. Expects these arguments:
//     [-o outputfile]
// the converted is written to mqlfile (cout if -o is not given)

int main(int argc, char **argv)
{
    // Decode arguments and open files

    int c;
    bool has_ofile = false;
    bool has_ifile = false;
    string output_name;
    string input_name;

    while ((c = getopt(argc, argv, "o:")) != -1) {
        switch(c) {
          case 'o':
                if (has_ofile) {
                    usage(argv[0]);
                    return 1;
                }

                has_ofile = true;
                output_name = optarg;
                break;
                
          case '?':
                usage(argv[0]);
                return 1;
        }
    }

    if (optind<argc) {
        has_ifile = true;
        input_name = argv[optind];
    }

    if (++optind<argc) {
        usage(argv[0]);
        return 1;
    }

    ofstream ofile;
    if (has_ofile) {
        ofile.open(output_name);
        if (!ofile) {
            cerr << "Cannot open " << output_name << endl;
            return 1;
        }
    }

    ifstream ifile;
    if (has_ifile) {
        ifile.open(input_name);
        if (!ifile) {
            cerr << "Cannot open " << input_name << endl;
            return 1;
        }
    }

    ostream& output = has_ofile ? ofile : cout; // References output stream
    istream& input = has_ifile ? ifile : cin; // References input stream

    bool is_o2t = string{argv[0]}.ends_with("o2t");
    
    string buf;
    while (getline(input, buf)) {
        if (is_o2t)
            output << oxia2tonos(buf) << "\n";
        else
            output << tonos2oxia(buf) << "\n";
    }
}

#endif
