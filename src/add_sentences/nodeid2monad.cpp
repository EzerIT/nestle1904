#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>

#include "nodeid2monad.hpp"

#define M2N_FILE "xmlWithNode.txt"


using namespace std;

static vector<string> nodeids;

void build_nodeid2monad()
{
    ifstream ifile{M2N_FILE};

    if (!ifile) {
        cerr << "Cannot open " << M2N_FILE << endl;
        exit(1);
    }

    string line;

    while (getline(ifile, line)) {
        auto it = find(begin(line),end(line),':');

        nodeids.emplace_back(begin(line), it);
    }
}

int nodeid2monad(const string& s)
{
    auto it = find(begin(nodeids), end(nodeids), s);

    if (it==end(nodeids)) {
        cerr << "Cannot find nodeId " << s << endl;
        exit(1);
    }

    return it - begin(nodeids) + 1; // Monad is index + 1
}

