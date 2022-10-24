#include <iostream>
#include <stdexcept>
#include "read_inflection.hpp"
#include "rapidcsv/src/rapidcsv.h"
#include "morph.hpp"

using namespace std;


static string get_fix_spelling(const rapidcsv::Document& doc, size_t col, size_t row)
{
    string s = doc.GetCell<string>(col,row);

    if (s=="ο-stamm") return "ο-stamme";
    if (s=="indeklinalbel") return "indeklinabel";
    if (s=="uregelmæssig ") return "uregelmæssig";

    return s;
}

static verb_type_t get_translate_verb_type(const rapidcsv::Document& doc, size_t col, size_t row)
{
    static map<string,verb_type_t> trans{ { "uregelmæssig", verb_type_t::irregular       } ,
                                          { "α-stamme",     verb_type_t::alpha           },
                                          { "β-stamme",     verb_type_t::beta            },
                                          { "γ-stamme",     verb_type_t::gamma           },
                                          { "δ-stamme",     verb_type_t::delta           },
                                          { "ε-stamme",     verb_type_t::epsilon         },
                                          { "ευ-stamme",    verb_type_t::epsilon_upsilon },
                                          { "ζ-stamme",     verb_type_t::zeta            },
                                          { "η-stamme",     verb_type_t::eta             },
                                          { "θ-stamme",     verb_type_t::theta           },
                                          { "ι-stamme",     verb_type_t::iota            },
                                          { "κ-stamme",     verb_type_t::kappa           },
                                          { "λ-stamme",     verb_type_t::lambda          },
                                          { "μ-stamme",     verb_type_t::mu              },
                                          { "μι-verbum",    verb_type_t::mu_iota         },
                                          { "ν-stamme",     verb_type_t::nu              },
                                          { "ο-stamme",     verb_type_t::omicron         },
                                          { "π-stamme",     verb_type_t::pi              },
                                          { "ρ-stamme",     verb_type_t::rho             },
                                          { "σκ-verbum",    verb_type_t::sigma_kappa     },
                                          { "τ-stamme",     verb_type_t::tau             },
                                          { "υ-stamme",     verb_type_t::upsilon         },
                                          { "φ-stamme",     verb_type_t::phi             },
                                          { "χ-stamme",     verb_type_t::khi             } };

    try {
        return trans.at( get_fix_spelling(doc, col, row) );
    }
    catch (const out_of_range& e) {
        cerr << e.what() << "\n";
        cerr << "Problem with verb stem: '" << get_fix_spelling(doc, col, row) << "'\n";
        exit(1);
    }
}

static noun_stem_t get_translate_noun_stem(const rapidcsv::Document& doc, size_t col, size_t row)
{
    static map<string,noun_stem_t> trans{ { "indeklinabel", noun_stem_t::indeclinable    },
                                          { "uregelmæssig", noun_stem_t::irregular       },
                                          { "α-stamme",     noun_stem_t::alpha           },
                                          { "β-stamme",     noun_stem_t::beta            },
                                          { "γ-stamme",     noun_stem_t::gamma           },
                                          { "δ-stamme",     noun_stem_t::delta           },
                                          { "ευ-stamme",    noun_stem_t::epsilon_upsilon },
                                          { "ι-stamme",     noun_stem_t::iota            },
                                          { "κ-stamme",     noun_stem_t::kappa           },
                                          { "κτ-stamme",    noun_stem_t::kappa_tau       },
                                          { "ν-stamme",     noun_stem_t::nu              },
                                          { "ντ-stamme",    noun_stem_t::nu_tau          },
                                          { "ο-stamme",     noun_stem_t::omicron         },
                                          { "ου-stamme",    noun_stem_t::omicron_upsilon },
                                          { "π-stamme",     noun_stem_t::pi              },
                                          { "ρ-stamme",     noun_stem_t::rho             },
                                          { "ς-stamme",     noun_stem_t::sigma           },
                                          { "τ-stamme",     noun_stem_t::tau             },
                                          { "υ-stamme",     noun_stem_t::upsilon         },
                                          { "χ-stamme",     noun_stem_t::khi             },
                                          { "ω-stamme",     noun_stem_t::omega           } };

    try {
        return trans.at( get_fix_spelling(doc, col, row) );
    }
    catch (const out_of_range& e) {
        cerr << e.what() << "\n";
        cerr << "Problem with noun stem: '" << get_fix_spelling(doc, col, row) << "'\n";
        exit(1);
    }
}

static noun_declension_t get_translate_noun_declension(const rapidcsv::Document& doc, size_t col, size_t row)
{
    static map<string,noun_declension_t> trans{ { "1. (-η)",                 noun_declension_t::first_eta                },
                                                { "1. (-ᾰ)",                 noun_declension_t::first_alpha_breve        },
                                                { "1. (-ᾱ)",                 noun_declension_t::first_alpha_macron       },
                                                { "1. (-ᾱ; dorisk genitiv)", noun_declension_t::first_alpha_macron_doric },
                                                { "2.",                      noun_declension_t::second_d                 },
                                                { "2. (attisk)",             noun_declension_t::second_attic             },
                                                { "3.",                      noun_declension_t::third_d                  },
                                                { "indeklinabel",            noun_declension_t::indeclinable             },
                                                { "uregelmæssig",            noun_declension_t::irregular                } };

    try {
        return trans.at( get_fix_spelling(doc, col, row) );
    }
    catch (const out_of_range& e) {
        cerr << e.what() << "\n";
        cerr << "Problem with declension: '" << get_fix_spelling(doc, col, row) << "'\n";
        exit(1);
    }
}

map<string,noun_stem_t> noun_stem_map;       // string{lemma,strongs,unreliable} => stem
map<string,noun_declension_t> noun_decl_map; // string{lemma,strongs,unreliable} => declension
map<string,verb_type_t> verb_type_map;       // string{lemma,strongs,unreliable} => stem

void read_inflection_spreadsheets()
{
    {
        rapidcsv::Document verbs;

        try {
            verbs.Load("makewordlist/greek_verbs.csv");
        }
        catch (const ios_base::failure& e) {
            cerr << e.what() << "\n";
            cerr << "Cannot open verbs file\n";
            exit(1);
        }

        for (size_t rix=0; rix<verbs.GetRowCount(); ++rix) {
            auto [it, success] = verb_type_map.insert(pair{verbs.GetCell<string>(0,rix), get_translate_verb_type(verbs, 1, rix)});

            if (!success) {
                cerr << "Duplicate verb '" << verbs.GetCell<string>(0,rix) << "\n";
                exit(1);
            }
        }
    }
    
    {
        rapidcsv::Document nouns;

        try {
            nouns.Load("makewordlist/greek_nouns.csv");
        }
        catch (const ios_base::failure& e) {
            cerr << e.what() << "\n";
            cerr << "Cannot open noun files" << "\n";
            exit(1);
        }

        for (size_t rix=0; rix<nouns.GetRowCount(); ++rix) {
            auto [it, success] = noun_decl_map.insert(pair{nouns.GetCell<string>(0,rix), get_translate_noun_declension(nouns, 1, rix)});

            if (!success) {
                cerr << "Duplicate noun '" << nouns.GetCell<string>(0,rix) << "\n";
                exit(1);
            }

            auto [it2, success2] = noun_stem_map.insert(pair{nouns.GetCell<string>(0,rix), get_translate_noun_stem(nouns, 2, rix)});

            if (!success2) {
                cerr << "PANIC: Duplicate noun '" << nouns.GetCell<string>(0,rix) << "\n";
                exit(1);
            }
        }
    }
}
