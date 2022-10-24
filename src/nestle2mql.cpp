#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <map>
#include <unistd.h>
#include "mql_item.hpp"
#include "mql_word.hpp"
#include "mql.hpp"


using namespace std;


// The MQL objects:
static vector<mql_word> words;
static vector<mql_book> books;
static vector<mql_chapter> chapters;
static vector<mql_verse> verses;



// Splits a reference of the form "Matt 3:8" into a book name, a chapter, and a verse.
// Parameter:
//    s: String containing reference
// Returns:
//    A tuple containing book name (using the Emdros enumeration), chapter, and verse.

static tuple<string,int,int> split_ref(const string& s)
{
    static map<string,string> book_names {
        {"Matt",   "Matthew"},
        {"Mark",   "Mark"},
        {"Luke",   "Luke"},
        {"John",   "John"},
        {"Acts",   "Acts"},
        {"Rom",    "Romans"},
        {"1Cor",   "I_Corinthians"},
        {"2Cor",   "II_Corinthians"},
        {"Gal",    "Galatians"},
        {"Eph",    "Ephesians"},
        {"Phil",   "Philippians"},
        {"Col",    "Colossians"},
        {"1Thess", "I_Thessalonians"},
        {"2Thess", "II_Thessalonians"},
        {"1Tim",   "I_Timothy"},
        {"2Tim",   "II_Timothy"},
        {"Titus",  "Titus"},
        {"Phlm",   "Philemon"},
        {"Heb",    "Hebrews"},
        {"Jas",    "James"},
        {"1Pet",   "I_Peter"},
        {"2Pet",   "II_Peter"},
        {"1John",  "I_John"},
        {"2John",  "II_John"},
        {"3John",  "III_John"},
        {"Jude",   "Jude"},
        {"Rev",    "Revelation"},
    };

    auto it1 = find(begin(s),end(s),' ');
    auto it2 = find(it1+1,end(s),':');

    return make_tuple(book_names.at(string{begin(s),it1}),
                      stoi(string{it1+1,it2}),
                      stoi(string{it2+1,end(s)}));
}


static void usage(const char* progname)
{
    cerr << "Usage:\n"
         << progname << " [-o mqlfile] bibletext\n";
}
        


// Main function. Expects these arguments:
//     [-o mqlfile] bibletext
// where
//     the generated MQL code is written to mqlfile (cout if -o is not given)
//     bibletext is the name of a csv file containing the Bible text

int main(int argc, char **argv)
{
    // Decode arguments and open files

    int c;
    bool oflag = false;
    string output_name;  // Name of MQL file
    string text_name;    // Name of Bible text file

    while ((c = getopt(argc, argv, "o:")) != -1) {
        switch(c) {
          case 'o':
                if (oflag) {
                    usage(argv[0]);
                    return 1;
                }

                oflag = true;
                output_name = optarg;
                break;
                
          case '?':
                usage(argv[0]);
                return 1;
        }
    }

    if (optind < argc)
        text_name = argv[optind];
    else {
        usage(argv[0]);
        return 1;
    }


    ofstream ofile;
    if (oflag) {
        ofile.open(output_name);
        if (!ofile) {
            cerr << "Cannot open " << output_name << endl;
            return 1;
        }
    }

    ostream& output = oflag ? ofile : cout; // References output stream

    ifstream bible_text{text_name};   // Bible text file stream
    if (!bible_text) {
        cerr << "Cannot open " << text_name << endl;
        return 1;
    }


    string line;

    // Read text from csv file
    while (getline(bible_text, line)) {
        int monad = words.size() + 1;

        words.emplace_back(monad, line);
        mql_word& w = words.back();

        string book;
        int chapter;
        int verse;
        tie(book, chapter, verse) = split_ref(w.get_ref());

        add_monad(books, monad, book);
        add_monad(chapters, monad, book, chapter);
        add_monad(verses, monad, book, chapter, verse);
    }



    // Generate occurrences and frequency rank
    mql_word::set_freq(words);

    // Store inflection information
    mql_word::set_inflection(words);

    
    // Generate MQL

    mql_header(output);

    // Define enumerations
    mql_word::define_obj(output);
    mql_book::define_obj(output);
    mql_chapter::define_obj(output);
    mql_verse::define_obj(output);


    // Create objects
    generate_mql_objects(output, words);
    generate_mql_objects(output, books);
    generate_mql_objects(output, chapters);
    generate_mql_objects(output, verses);

    mql_trailer(output);
}
