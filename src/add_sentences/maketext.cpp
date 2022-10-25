// Generates the file xmlWithNode.txt which contains nodeId:word from the NT XML files

#include <unistd.h>
//#include <cstring>
//#include <cctype>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>

#include "pugixml.hpp"

using namespace std;

class simple_walker : public pugi::xml_tree_walker {
  public:

    // This function is called for every XML element
    virtual bool for_each(pugi::xml_node& node) {
        if (node.type()==pugi::node_element) {
            pugi::xml_attribute unicode = node.attribute("Unicode");
            if (!unicode.empty())
                values.emplace_back(string{node.attribute("nodeId").value()} + ":"s + string{unicode.value()});
        }
    
        return true;
    }

    void printit(ostream& ofile) {
        sort(values.begin(),values.end()); // The nodeId gives the word order
        for (const string& s : values)
            ofile << s << "\n";
    }

  private:
    vector<string> values;
};


static void usage(const char* progname)
{
    cerr << "Usage:\n"
         << progname << " [-o textfile]\n";
}


const string filenames[] = {
    "../../greek-new-testament/syntax-trees/nestle1904/xml/01-matthew.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/02-mark.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/03-luke.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/04-john.xml",
    "../../corrected/05-acts.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/06-romans.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/07-1corinthians.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/08-2corinthians.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/09-galatians.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/10-ephesians.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/11-philippians.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/12-colossians.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/13-1thessalonians.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/14-2thessalonians.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/15-1timothy.xml",
    "../../corrected/16-2timothy.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/17-titus.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/18-philemon.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/19-hebrews.xml",
    "../../corrected/20-james.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/21-1peter.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/22-2peter.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/23-1john.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/24-2john.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/25-3john.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/26-jude.xml",
    "../../greek-new-testament/syntax-trees/nestle1904/xml/27-revelation.xml",
};
        

// Main function. Expects these arguments:
//     [-o outputfile]

int main(int argc, char **argv)
{
    // Decode arguments and open files

    int c;
    bool oflag = false;
    string output_name;  // Name of text file

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

    ofstream ofile;
    if (oflag) {
        ofile.open(output_name);
        if (!ofile) {
            cerr << "Cannot open " << output_name << endl;
            return 1;
        }
    }

    simple_walker w;

    for (string xmlfile : filenames) {
        pugi::xml_document doc;

//        cerr << xmlfile << endl;  // Debug output
        
        if (!doc.load_file(xmlfile.c_str())) {
            cerr << "Error: Cannot read \"" << xmlfile << "\"" << endl;
            continue;
        }

        doc.document_element().traverse(w);
    }

    w.printit(oflag ? ofile : cout);
}
