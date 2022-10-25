// Generates the file xmlWithNode.txt which contains nodeId:word from the NT XML files

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <map>

#include "findfiles.hpp"
#include "pugixml.hpp"
#include "../oxia2tonos.hpp"

using namespace std;

class simple_walker : public pugi::xml_tree_walker {
  public:

    // This function is called for every XML element
    virtual bool for_each(pugi::xml_node& node) {
        if (node.type()==pugi::node_element) {
            pugi::xml_attribute unicode = node.attribute("Unicode");
            if (!unicode.empty()) {
                string nodeId = node.attribute("nodeId").value();
                if (!values.contains(nodeId))  // Use only the first occurrence of a word
                    values[nodeId] = unicode.value();
            }
        }
    
        return true;
    }
 
    void printit(ostream& ofile) {
        for (const pair<string,string>& s : values)
            ofile << s.first << ":" << tonos2oxia(s.second) << "\n";
    }

  private:
    map<string,string> values;
};


static void usage(const char* progname)
{
    cerr << "Usage:\n"
         << progname << " [-o textfile]\n";
}



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

    vector<string> filenames;
    string xml_dir{"../../greek-new-testament/syntax-trees/nestle1904/xml/"};

    findfiles(xml_dir, filenames);

    for (string xmlfile : filenames) {
        if (!isdigit(xmlfile[0]) || !isdigit(xmlfile[1]))
            continue; // we only want the files 01-matthew.xml to 27-revelation.xml

        pugi::xml_document doc;
        if (!doc.load_file((xml_dir + xmlfile).c_str())) return -1;

        doc.document_element().traverse(w);
    }

    w.printit(oflag ? ofile : cout);
}
