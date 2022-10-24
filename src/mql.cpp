#include <ostream>
#include "mql.hpp"

using namespace std;

// Writes the initial information that goes into the MQL file

void mql_header(ostream& output)
{
    output <<
        "CREATE DATABASE 'nestle1904' GO\n"
        "USE DATABASE 'nestle1904' GO\n"
        "\n"
        "CREATE ENUMERATION boolean_t = {\n"
        "    false = 0,\n"
        "    true = 1\n"
        "}\n"
        "GO\n"
        "\n"
        "CREATE ENUMERATION book_name_t = {\n"
        "    Matthew,\n"
        "    Mark,\n"
        "    Luke,\n"
        "    John,\n"
        "    Acts,\n"
        "    Romans,\n"
        "    I_Corinthians,\n"
        "    II_Corinthians,\n"
        "    Galatians,\n"
        "    Ephesians,\n"
        "    Philippians,\n"
        "    Colossians,\n"
        "    I_Thessalonians,\n"
        "    II_Thessalonians,\n"
        "    I_Timothy,\n"
        "    II_Timothy,\n"
        "    Titus,\n"
        "    Philemon,\n"
        "    Hebrews,\n"
        "    James,\n"
        "    I_Peter,\n"
        "    II_Peter,\n"
        "    I_John,\n"
        "    II_John,\n"
        "    III_John,\n"
        "    Jude,\n"
        "    Revelation\n"
        "}\n"
        "GO\n"
        "\n";
}


// Writes the final information that goes into the MQL file

void mql_trailer(ostream& output)
{
    output << "VACUUM DATABASE ANALYZE GO\n";
}

