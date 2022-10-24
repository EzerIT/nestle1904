#include <unistd.h>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>

#include "pugixml.hpp"
#include "findfiles.hpp"
#include "nodeid2monad.hpp"
#include "objects.hpp"
#include "walker.hpp"

using namespace std;



static void usage(const char* progname)
{
    cerr << "Usage:\n"
         << progname << " [-o mqlfile]\n";
}


// Main function. Expects these arguments:
//     [-o mqlfile]
// where
//     the generated MQL code is written to mqlfile (cout if -o is not given)

int main(int argc, char **argv)
{
    // Decode arguments and open files

    int c;
    bool oflag = false;
    string output_name;  // Name of MQL file

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

    ostream& output = oflag ? ofile : cout; // References output stream

    sentence_handler sentences;
    clause_handler clauses[2] {1,2};

    build_nodeid2monad();

    simple_walker w {&sentences, clauses, 2};
    vector<string> filenames;
    string xml_dir{"../../greek-new-testament/syntax-trees/nestle1904/xml/"};

    findfiles(xml_dir, filenames);

    sentences.mql_head(output);
    for (const clause_handler& chand : clauses)
        chand.mql_head(output);

    int monad = 0;

    for (string xmlfile : filenames) {
//        if (xmlfile!="03-luke.xml") continue; // For debugging
        
        if (!isdigit(xmlfile[0]) || !isdigit(xmlfile[1]))
            continue; // we only want the files 01-matthew.xml to 27-revelation.xml

        
        pugi::xml_document doc;
        if (!doc.load_file((xml_dir + xmlfile).c_str())) return -1;

        cerr << xmlfile << endl;

        for (pugi::xml_node sentence : doc.document_element().children()) {
            assert(strcmp(sentence.name(), "Sentence")==0);

            bool first_trees = true;
            for (pugi::xml_node trees : sentence) {
                assert(strcmp(trees.name(), "Trees")==0);
                if (first_trees)
                    first_trees = false;
                else {
                    cerr << "More than one <Trees> in <Sentence>\n";
                    exit(1);
                }
                    

                for (pugi::xml_node tree : trees) {
                    assert(strcmp(tree.name(), "Tree")==0);

                    bool first_node = true;
                    for (pugi::xml_node node : tree) {
                        assert(strcmp(node.name(), "Node")==0);
                        if (first_node)
                            first_node = false;
                        else {
                            cerr << "More than one <Node> in <Tree>\n";
                            exit(1);
                        }
                        
                        sentences.push(true);

                        node.traverse(w);
                    }
                    break; // Skip alternate <Tree> elements
                }
            }
        }
    }

    sentences.mql_start_obj(output);
    for (const monads& cl : sentences.get_monads()) {
        if (cl.useit())
            sentences.mql_one_obj(output, cl);
    }
    sentences.mql_end_obj(output);


    for (const clause_handler& chand : clauses) {
        chand.mql_start_obj(output);
        for (const monads& cl : chand.get_monads()) {
            if (cl.useit())
                chand.mql_one_obj(output, cl);
        }
        chand.mql_end_obj(output);
    }

}
