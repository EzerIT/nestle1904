#include <dirent.h>
#include <stdexcept>
#include <algorithm>

#include "findfiles.hpp"

using namespace std;

void findfiles(const string& dirname, vector<string>& filenames)
{
    DIR *d = opendir(dirname.c_str());
    dirent *dp; 
    
    if (!d) {
        throw invalid_argument("Cannot open directory " + dirname);
    }

    while (dp = readdir(d))
        if (dp->d_type == DT_REG)
            filenames.emplace_back(dp->d_name);
    
    sort(begin(filenames), end(filenames));

    closedir(d);
}

