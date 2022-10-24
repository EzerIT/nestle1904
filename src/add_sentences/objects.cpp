//#include <cstring>
//#include <algorithm>
//#include <cassert>
#include <fstream>
#include <iostream>
//#include <iomanip>
#include <stdexcept>
//#include <utility>


#include "objects.hpp"

using namespace std;


void monads::new_monad(int monad)
{
    if (_si.count(monad)>0)
        throw invalid_argument("Collection has " + to_string(monad));

    if (monad>_max)
        _max = monad;
    if (monad<_min)
        _min = monad;
    _si.insert(monad);
}

bool monads::is_consecutive(int& init) const
{
    for (int x : _si) {
        if (x != ++init) {
            cerr << "Expect " << init << " got " << x << endl;
            return false;
        }
    }
    return true;
}

bool monads::is_covering(int& init) const
{
    bool first = true;

    for (int x : _si) {
        if (first) {
            if (x != ++init) {
                cerr << "Expect " << init << " got " << x << endl;
                return false;
            }
            first = false;
        }
        else
            init = x;
    }
    return true;
}


const vector<pair<int,int>>& monads::get_segments() const
{
    if (_segments.empty()) {
        int low;

        bool is_in{false};
        for (int i=_min; i<=_max; ++i) {
            if (_si.count(i)>0) {
                if (!is_in) {
                    low = i;
                    is_in = true;
                }
            }
            else {
                if (is_in) {
                    _segments.emplace_back(low, i-1);
                    is_in = false;
                }
            }
        }
        if (is_in)
            _segments.emplace_back(low,_max);
    }
    return _segments;
}

void object_handler::push(bool usethis)
{
    _objects.emplace_back(usethis);
}

void clause_handler::push(bool usethis, const string& clause_type)
{
    static string typ{"typ"};

    _objects.emplace_back(usethis);
    _objects.back().features()[typ] = clause_type;
}


void sentence_handler::mql_head(ostream& output) const
{
    output <<
        "CREATE OBJECT TYPE\n"
        "WITH SINGLE RANGE OBJECTS\n"
        "HAVING UNIQUE FIRST AND LAST MONADS\n"
        "[sentence\n"
        "]\n"
        "GO\n\n";
}

void clause_handler::mql_head(ostream& output) const
{
    if (_level==1) {
        output <<
            "CREATE ENUMERATION clause_type_t = {\n"
            "  ADV,\n"
            "  CL,\n"
            "  IO,\n"
            "  O,\n"
            "  O2,\n"
            "  P,\n"
            "  S,\n"
            "  V,\n"
            "  VC\n"
            "}\n"
            "GO\n\n";
    }

    output <<
        "CREATE OBJECT TYPE\n"
        "[clause" << _level << "\n"
        "  typ : clause_type_t;\n"
        "]\n"
        "GO\n\n";
}

void sentence_handler::mql_start_obj(ostream& output) const
{
    output <<
        "CREATE OBJECTS\n"
        "WITH OBJECT TYPE[sentence]\n";
}

void clause_handler::mql_start_obj(ostream& output) const
{
    output <<
        "CREATE OBJECTS\n"
        "WITH OBJECT TYPE[clause" << _level << "]\n";
}


void object_handler::mql_one_obj(ostream& output, const monads& m) const
{
    output << 
        "CREATE OBJECT\n"
        "FROM MONADS= { ";

    bool first{true};
    for (const pair<int,int>& pi : m.get_segments()) {
        if (first)
            first = false;
        else
            output << ", ";

        if (pi.first==pi.second)
            output << pi.first;
        else
            output << pi.first << "-" << pi.second;
    }

    output <<
        " }\n"
        "[\n";

    mql_fields(output, m);

    output <<
        "]\n";
}

void object_handler::mql_fields(std::ostream& output, const monads &m) const
{
}

void clause_handler::mql_fields(std::ostream& output, const monads &m) const
{
    for (const pair<string,string>& p : m.features())
        output << "  " << p.first << ":=" << p.second << ";\n";
}


void object_handler::mql_end_obj(ostream& output) const
{
    output << "GO\n\n";
}

void object_handler::new_monad(int monad)
{
    if (_allmonads.count(monad)>0)
        return;

    _objects.back().new_monad(monad);
}
