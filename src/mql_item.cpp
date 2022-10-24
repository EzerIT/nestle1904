#include <iostream>
#include "mql_item.hpp"

using namespace std;

// See mql_item.hpp for documentation of the functions


//////////////////////////////////////////////////////////////////////
// range
//////////////////////////////////////////////////////////////////////

void range::add(int i)
{
    if (m_first==0)
        m_first = m_last = i;
    else if (m_last+1==i)
        m_last = i;
    else if (m_last!=i)
        cerr << "Bad range addition: " << i << endl;
}


//////////////////////////////////////////////////////////////////////
// mql_item
//////////////////////////////////////////////////////////////////////

void obj_definition::define_obj(ostream& output, const string& objtype, bool single_monad, const vector<obj_definition>& defs)
{
    output <<
        "CREATE OBJECT TYPE\n" <<
        "IF NOT EXISTS\n" <<
        (single_monad 
         ? "WITH SINGLE MONAD OBJECTS\n"
         : "WITH SINGLE RANGE OBJECTS\n") <<
        "HAVING UNIQUE FIRST AND LAST MONADS\n"
        "[" << objtype << "\n";

    for (const auto& def : defs) {
        output << "    " << def.m_name << " : " << def.m_type;
        if (def.m_from_set)
            output << " FROM SET";
        if (def.m_with_index)
            output << " WITH INDEX";
        if (!def.m_default_value.empty())
            output << " DEFAULT " << def.m_default_value;
        output << ";\n";
    }

    output <<
        "]\n"
        "GO\n\n";
}


void mql_item::trans_end(ostream& output, const string& objtype)
{
    output <<
        "GO\n"
        "CREATE INDEXES ON OBJECT TYPE[" << objtype << "] GO\n\n";
}

void mql_item::trans_start(ostream& output, const string& objtype)
{
    output <<
        "CREATE OBJECTS\n"
        "WITH OBJECT TYPE[" << objtype << "]\n";
}

//////////////////////////////////////////////////////////////////////
// mql_book
//////////////////////////////////////////////////////////////////////

void mql_book::define_obj(ostream& output)
{
    obj_definition::define_obj(output, "book", false, {{"book",    "book_name_t", "Matthew"}});
}

void mql_book::generate_object(ostream& output) const
{
    output <<
        "CREATE OBJECT\n"
        "FROM MONADS= { " << m_range.get_first() << "-" << m_range.get_last() << " }\n"
        "[\n"
        "    book := " << m_book << ";\n"
        "]\n";
}


//////////////////////////////////////////////////////////////////////
// mql_chapter
//////////////////////////////////////////////////////////////////////

void mql_chapter::define_obj(ostream& output)
{
    obj_definition::define_obj(output, "chapter", false, {{"book",    "book_name_t", "Matthew"},
                {"chapter", "integer",     "0"      }});
}

void mql_chapter::generate_object(ostream& output) const
{
    output <<
        "CREATE OBJECT\n"
        "FROM MONADS= { " << m_range.get_first() << "-" << m_range.get_last() << " }\n"
        "[\n"
        "    book := " << m_book << ";\n"
        "    chapter := " << m_chapter << ";\n"
        "]\n";
}

//////////////////////////////////////////////////////////////////////
// mql_verse
//////////////////////////////////////////////////////////////////////

void mql_verse::define_obj(ostream& output)
{
    obj_definition::define_obj(output, "verse", false, {{"book",    "book_name_t", "Matthew"},
                                                        {"chapter", "integer",     "0"      },
                                                        {"verse",   "integer",     "0"      }});
}

void mql_verse::generate_object(ostream& output) const
{
    output <<
        "CREATE OBJECT\n"
        "FROM MONADS= { " << m_range.get_first() << "-" << m_range.get_last() << " }\n"
        "[\n"
        "    book := " << m_book << ";\n"
        "    chapter := " << m_chapter << ";\n"
        "    verse := " << m_verse << ";\n"
        "]\n";
}

