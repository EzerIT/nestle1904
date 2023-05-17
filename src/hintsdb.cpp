/* Copyright © 2022 Claus Tøndering.
 * Released under an MIT License.
 */
 
#include <emdros/emdfdb.h>
#include <emdros/emdros_environment.h>
#include <emdros/mql_sheaf.h>
#include <emdros/emdf_value.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "rapidcsv/src/rapidcsv.h"

#include "emdros_iterators.hpp"
#include "util.hpp"

using namespace std;

enum class cols_nouns {
    bol_ref,bol_monad_num,word,bol_surface,normalized,bol_psp,
    gn0,nu0,ca0, // Gender, number, case in Bible OL
    gn1,nu1,ca1, // Gender, number, case alternative 1
    gn2,nu2,ca2, // Gender, number, case alternative 2
    gn3,nu3,ca3, // Gender, number, case alternative 3
    gn4,nu4,ca4, // Gender, number, case alternative 4
    gn5,nu5,ca5, // Gender, number, case alternative 5
};

enum class cols_verbs {
    bol_ref,bol_monad_num,word,bol_surface,normalized,bol_psp,gender,
    nu0,pe0,te0,vo0,mo0, // Number, person, tense, voice, mood in Bible OL
    nu1,pe1,te1,vo1,mo1, // Number, person, tense, voice, mood alternative
};

string& at(vector<string>& r, int c)
{
    static string unknown = "unknown";
    
    if (c >= r.size())
        return unknown;
    else
        return r.at(c);
}

const string& at(const vector<string>& r, int c)
{
    static string unknown = "unknown";
    
    if (c >= r.size())
        return unknown;
    else
        return r.at(c);
}

template <typename COLS>
string& at(vector<string>& r, COLS c)
{
    return at(r,(int)c);
}

template <typename COLS>
const string& at(const vector<string>& r, COLS c)
{
    return at(r,(int)c);
}


int count_var_nouns(const vector<string>& r)
{
    int count = 0;

    if (!at(r,cols_nouns::gn0).empty()) ++count;
    if (!at(r,cols_nouns::gn1).empty()) ++count;
    if (!at(r,cols_nouns::gn2).empty()) ++count;
    if (!at(r,cols_nouns::gn3).empty()) ++count;
    if (!at(r,cols_nouns::gn4).empty()) ++count;
    if (!at(r,cols_nouns::gn5).empty()) ++count;

    return count;
}

int count_var_verbs(const vector<string>& r)
{
    int count = 0;

    if (!at(r,cols_verbs::nu0).empty()) ++count;
    if (!at(r,cols_verbs::nu1).empty()) ++count;

    return count;
}


vector<vector<int>> noun_feat_diffs{
    {(int)cols_nouns::gn0, (int)cols_nouns::nu0, (int)cols_nouns::ca0},
    {(int)cols_nouns::gn1, (int)cols_nouns::nu1, (int)cols_nouns::ca1},
    {(int)cols_nouns::gn2, (int)cols_nouns::nu2, (int)cols_nouns::ca2},
    {(int)cols_nouns::gn3, (int)cols_nouns::nu3, (int)cols_nouns::ca3},
    {(int)cols_nouns::gn4, (int)cols_nouns::nu4, (int)cols_nouns::ca4},
    {(int)cols_nouns::gn5, (int)cols_nouns::nu5, (int)cols_nouns::ca5},
};

vector<string> noun_feat_diff2string { "gender", "number", "case" };

vector<vector<int>> verb_feat_diffs{
    {(int)cols_verbs::nu0, (int)cols_verbs::pe0, (int)cols_verbs::te0, /*(int)cols_verbs::vo0,*/ (int)cols_verbs::mo0},
    {(int)cols_verbs::nu1, (int)cols_verbs::pe1, (int)cols_verbs::te1, /*(int)cols_verbs::vo1,*/ (int)cols_verbs::mo1},
};

vector<string> verb_feat_diff2string { "number", "person", "tense", /*"voice",*/ "mood" };
    

class selector {
  public:
    selector(const vector<vector<int>>& feat_diffs, const vector<string>& feat_diff2string, const initializer_list<int> two, const initializer_list<int> three)
        : m_feat_diffs{feat_diffs},
          m_feat_diff2string{feat_diff2string},
          m_twovalues{two},
          m_threevalues{three}
        {}
            

    string diff2(const vector<string>&r) {
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            int col0 = m_feat_diffs[0][i];
            int col1 = m_feat_diffs[1][i];

            if (at(r,col0)!=at(r,col1)) {
                if (m_twovalues.contains(i))
                    return m_feat_diff2string.at(i) + "=" + at(r,col0);// + "---21a";
                else
                    return m_feat_diff2string.at(i) + "≠" + at(r,col1);// + "---21b";
            }
        }
        for (auto rc : r)
            cerr << rc << " ";
        cerr << "INDETERMINATE 1\n";
        return "INDETERMINATE 1";
    }

    string diff3(const vector<string>&r) {
        // Look for single ≠ selector
        
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            int col0 = m_feat_diffs[0][i];
            int col1 = m_feat_diffs[1][i];
            int col2 = m_feat_diffs[2][i];

            if (at(r,col0)!=at(r,col1) && at(r,col1)==at(r,col2))
                if (m_twovalues.contains(i))
                    return m_feat_diff2string.at(i) + "=" + at(r,col0);// + "---31a";
                else
                    return m_feat_diff2string.at(i) + "≠" + at(r,col1);// + "---31b";
        }

        // Look for double ≠ selector
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            int col0 = m_feat_diffs[0][i];
            int col1 = m_feat_diffs[1][i];
            int col2 = m_feat_diffs[2][i];

            if (at(r,col0)!=at(r,col1) && at(r,col0)!=at(r,col2))
                if (m_threevalues.contains(i))
                    return m_feat_diff2string.at(i) + "=" + at(r,col0);// + "---32a";
                else
                    return m_feat_diff2string.at(i) + "≠" + at(r,col1) + "," + m_feat_diff2string.at(i) + "≠" + at(r,col2);// + "---32b";
        }

        // Look for selector involving two features
        // (This is tailored to find {feminine nominative, feminine genitive, masculine nominative}
        // triplets as this is the only relevant case in Nestle1904)
        if (at(r,m_feat_diffs[0][0])=="feminine"  && at(r,m_feat_diffs[0][1])=="singular" && at(r,m_feat_diffs[0][2])=="nominative" &&
            at(r,m_feat_diffs[1][0])=="feminine"  && at(r,m_feat_diffs[1][1])=="singular" && at(r,m_feat_diffs[1][2])=="genitive" &&
            at(r,m_feat_diffs[2][0])=="masculine" && at(r,m_feat_diffs[2][1])=="singular" && at(r,m_feat_diffs[2][2])=="nominative")
            return m_feat_diff2string.at(0) + "≠masculine," + m_feat_diff2string.at(2) + "≠genitive";
        
        for (auto rc : r)
            cerr << rc << " ";
        cerr << "INDETERMINATE 2\n";
        return "INDETERMINATE 2";
    }

    string diff4(const vector<string>&r) {
        // Look for single ≠ selector
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            string s0 = at(r, m_feat_diffs[0][i]);
            string s1 = at(r, m_feat_diffs[1][i]);
            string s2 = at(r, m_feat_diffs[2][i]);
            string s3 = at(r, m_feat_diffs[3][i]);
            
            if (s0!=s1 && s1==s2 && s1==s3)
                return m_feat_diff2string.at(i) + "≠" + s1;// + "---41";
        }

        // Look for double ≠ selector
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            string s0 = at(r, m_feat_diffs[0][i]);
            string s1 = at(r, m_feat_diffs[1][i]);
            string s2 = at(r, m_feat_diffs[2][i]);
            string s3 = at(r, m_feat_diffs[3][i]);

            if (s0==s1 || s0==s2 || s0==s3)
                continue;

            string sA, sB;
            
            if (s1==s2) {
                sA = s1;
                sB = s3;
            }
            else if (s1==s3) {
                sA = s1;
                sB = s2;
            }
            else if (s2==s3) {
                sA = s2;
                sB = s1;
            }
            else
                continue;

            return m_feat_diff2string.at(i) + "≠" + sA + "," + m_feat_diff2string.at(i) + "≠" + sB;// + "---42";
        }

//        // Look for single = selector
//        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
//            string s0 = at(r, m_feat_diffs[0][i]);
//            string s1 = at(r, m_feat_diffs[1][i]);
//            string s2 = at(r, m_feat_diffs[2][i]);
//            string s3 = at(r, m_feat_diffs[3][i]);
//            
//            if (s0!=s1 && s0!=s2 && s0!=s3 && s1!=s2 && s1!=s3 && s2!=s3)
//                return m_feat_diff2string.at(i) + "=" + s0 + "---43";
//        }

        // Indeterminate based on one feature, let's try two features
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            for (int j=i+1; j<m_feat_diffs[0].size(); ++j) {
            
                string s0a = at(r, m_feat_diffs[0][i]);
                string s1a = at(r, m_feat_diffs[1][i]);
                string s2a = at(r, m_feat_diffs[2][i]);
                string s3a = at(r, m_feat_diffs[3][i]);
                string s0b = at(r, m_feat_diffs[0][j]);
                string s1b = at(r, m_feat_diffs[1][j]);
                string s2b = at(r, m_feat_diffs[2][j]);
                string s3b = at(r, m_feat_diffs[3][j]);

                string s0 = s0a + "." + s0b;
                string s1 = s1a + "." + s1b;
                string s2 = s2a + "." + s2b;
                string s3 = s3a + "." + s3b;

                if (alldiff(s0,s1,s2,s3))
                    return m_feat_diff2string.at(i) + "=" + s0a + ","
                        + m_feat_diff2string.at(j) + "=" + s0b;// + "---44";
            }
        }

        for (auto rc : r)
            cerr << rc << " ";
        cerr << "INDETERMINATE 3\n";
        return "INDETERMINATE 3";
    }

    string diff5(const vector<string>&r) {
        // Look for single ≠ selector
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            string s0 = at(r, m_feat_diffs[0][i]);
            string s1 = at(r, m_feat_diffs[1][i]);
            string s2 = at(r, m_feat_diffs[2][i]);
            string s3 = at(r, m_feat_diffs[3][i]);
            string s4 = at(r, m_feat_diffs[4][i]);
            
            if (s0!=s1 && s1==s2 && s1==s3 && s1==s4)
                return m_feat_diff2string.at(i) + "≠" + s1;// + "---51";
        }

        // Look for double ≠ selector
//        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
//            string s0 = at(r, m_feat_diffs[0][i]);
//            string s1 = at(r, m_feat_diffs[1][i]);
//            string s2 = at(r, m_feat_diffs[2][i]);
//            string s3 = at(r, m_feat_diffs[3][i]);
//            string s4 = at(r, m_feat_diffs[4][i]);
// 
//            if (s0==s1 || s0==s2 || s0==s3 || s0==s4)
//                continue;
// 
//            string sA, sB;
// 
//            if      (s1==s2 && s3==s4) { sA=s1; sB=s3; } // aabb
//            else if (s1==s3 && s2==s4) { sA=s1; sB=s2; } // abab
//            else if (s1==s4 && s2==s3) { sA=s1; sB=s2; } // abba
// 
//            else if (s1==s2 && s1==s3) { sA=s1; sB=s4; } // aaab
//            else if (s1==s2 && s1==s4) { sA=s1; sB=s3; } // aaba
//            else if (s1==s3 && s1==s4) { sA=s1; sB=s2; } // abaa
// 
//            else if (s2==s3 && s2==s4) { sA=s1; sB=s2; } // abbb
//                
//            return m_feat_diff2string.at(i) + "≠" + sA + "," + m_feat_diff2string.at(i) + "≠" + sB + "---52";
//        }

        // Indeterminate based on one feature, let's try two features
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            for (int j=i+1; j<m_feat_diffs[0].size(); ++j) {
            
                string s0a = at(r, m_feat_diffs[0][i]);
                string s1a = at(r, m_feat_diffs[1][i]);
                string s2a = at(r, m_feat_diffs[2][i]);
                string s3a = at(r, m_feat_diffs[3][i]);
                string s4a = at(r, m_feat_diffs[4][i]);
                string s0b = at(r, m_feat_diffs[0][j]);
                string s1b = at(r, m_feat_diffs[1][j]);
                string s2b = at(r, m_feat_diffs[2][j]);
                string s3b = at(r, m_feat_diffs[3][j]);
                string s4b = at(r, m_feat_diffs[4][j]);

                string s0 = s0a + "." + s0b;
                string s1 = s1a + "." + s1b;
                string s2 = s2a + "." + s2b;
                string s3 = s3a + "." + s3b;
                string s4 = s4a + "." + s4b;

                if (alldiff(s0,s1,s2,s3,s4))
                    return m_feat_diff2string.at(i) + "=" + s0a + ","
                        + m_feat_diff2string.at(j) + "=" + s0b;// + "---53";
            }
        }

        for (auto rc : r)
            cerr << rc << " ";
        cerr << "INDETERMINATE 4\n";
        return "INDETERMINATE 4";
    }

    
    string diff6(const vector<string>&r) {
        // Look for single ≠ selector
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            string s0 = at(r, m_feat_diffs[0][i]);
            string s1 = at(r, m_feat_diffs[1][i]);
            string s2 = at(r, m_feat_diffs[2][i]);
            string s3 = at(r, m_feat_diffs[3][i]);
            string s4 = at(r, m_feat_diffs[4][i]);
            string s5 = at(r, m_feat_diffs[5][i]);
            
            if (s0!=s1 && s1==s2 && s1==s3 && s1==s4 && s1==s5)
                return m_feat_diff2string.at(i) + "≠" + s1;// + "---61";
        }

        // Indeterminate based on one feature, let's try two features
        for (int i=0; i<m_feat_diffs[0].size(); ++i) {
            for (int j=i+1; j<m_feat_diffs[0].size(); ++j) {
            
                string s0a = at(r, m_feat_diffs[0][i]);
                string s1a = at(r, m_feat_diffs[1][i]);
                string s2a = at(r, m_feat_diffs[2][i]);
                string s3a = at(r, m_feat_diffs[3][i]);
                string s4a = at(r, m_feat_diffs[4][i]);
                string s5a = at(r, m_feat_diffs[5][i]);
                string s0b = at(r, m_feat_diffs[0][j]);
                string s1b = at(r, m_feat_diffs[1][j]);
                string s2b = at(r, m_feat_diffs[2][j]);
                string s3b = at(r, m_feat_diffs[3][j]);
                string s4b = at(r, m_feat_diffs[4][j]);
                string s5b = at(r, m_feat_diffs[5][j]);

                string s0 = s0a + "." + s0b;
                string s1 = s1a + "." + s1b;
                string s2 = s2a + "." + s2b;
                string s3 = s3a + "." + s3b;
                string s4 = s4a + "." + s4b;
                string s5 = s5a + "." + s5b;

                if (alldiff(s0,s1,s2,s3,s4,s5))
                    return m_feat_diff2string.at(i) + "=" + s0a + ","
                        + m_feat_diff2string.at(j) + "=" + s0b;// + "---63";
            }
        }

        for (auto rc : r)
            cerr << rc << " ";
        cerr << "INDETERMINATE 4\n";
        return "INDETERMINATE 4";
    }
    
  private:
    const vector<vector<int>>& m_feat_diffs;
    const vector<string>& m_feat_diff2string;
    set<int> m_twovalues;
    set<int> m_threevalues;

    bool alldiff(const string& s1, const string& s2, const string& s3, const string& s4) {
        return s1!=s2 && s1!=s3 && s1!=s4;// && s2!=s3 && s2!=s4 && s3!=s4;
    }

    bool alldiff(const string& s1, const string& s2, const string& s3, const string& s4, const string& s5) {
        return s1!=s2 && s1!=s3 && s1!=s4 && s1!=s5; // && s2!=s3 && s2!=s4 && s2!=s5 && s3!=s4 && s3!=s5 && s4!=s5;
    }

    bool alldiff(const string& s1, const string& s2, const string& s3, const string& s4, const string& s5, const string& s6) {
        return s1!=s2 && s1!=s3 && s1!=s4 && s1!=s5 && s1!=s6;
    }
};



void tr_gender(string& s)
{
    static map<string,string> tr{
        { "",  "" },
        { "f", "feminine" },
        { "m", "masculine" },
        { "n", "neuter" },
    };

    s = tr.at(s);
}

void tr_number(string& s)
{
    static map<string,string> tr{
        { "",  "" },
        { "sg", "singular" },
        { "pl", "plural" },
    };

    s = tr.at(s);
}

void tr_case(string& s)
{
    static map<string,string> tr{
        { "",  "" },
        { "acc", "accusative" },
        { "dat", "dative" },
        { "gen", "genitive" },
        { "nom", "nominative" },
        { "voc", "vocative" },
    };

    s = tr.at(s);
}

void tr_na(string& s)
{
    if (s=="N/A")
        s = "NA";
}
    

int main(int argc, char **argv)
{
    if (argc!=3) {
        cerr << "usage: hintsdb <emdrosfile> <sqlfile>" << endl;
        return 1;
    }

    ofstream sqlfile{argv[2]};
    sqlfile << "CREATE TABLE hints (self integer primary key, hint text);\n";
    sqlfile << "BEGIN TRANSACTION;\n";

    
    EmdrosEnv EE{kOKConsole,
            kCSUTF8,
            "localhost",
            "",
            "",
            argv[1],
            kSQLite3};

    bool bResult{false};

    //======================================================================
    // Handle nouns
    //======================================================================
    
    {
        selector noun_selector{noun_feat_diffs, noun_feat_diff2string,{1},{0}};

        string csvfile = "GREEK_BibleOL_nominal-ambiguity-project_v1.21.csv";
    
        rapidcsv::Document csv;
        int csv_row = 0;
        
        try {
            csv.Load(csvfile);
        }
        catch (const ios_base::failure& e) {
            cerr << e.what() << "\n";
            cerr << "Cannot open " << csvfile << "\n";
            return 1;
        }

        string mql_request{"SELECT ALL OBJECTS WHERE [word psp=noun GET self,monad_num,surface,"
            "gender,number,case] GO"};
    
        if (!EE.executeString(mql_request, bResult, false, true))
            return 1;

        if (!EE.isSheaf()) {
            cerr << "ERROR: Result is not sheaf\n";
            return 1;
        }

        for (StrawOk str : SheafOk{EE.getSheaf()}) {
            long self = 0;
            int monad_num;
            string surface;
            string features[3]; //gender,number,case
        
            for (const MatchedObject mo : str) {
                self = mo.getFeatureAsLong(0);
                monad_num = mo.getFeatureAsLong(1);
                surface = mo.getFeatureAsString(2);
                for (int i=0; i<3; ++i)
                    features[i] = mo.getFeatureAsString(3+i);
            }

            vector<string> row;

            do {
                row = csv.GetRow<string>(csv_row++);
            } while (stoi(at(row,cols_nouns::bol_monad_num)) < monad_num);

        
            if (surface != at(row,cols_nouns::bol_surface)) {
                cerr << "Inconsistency between database and spreadsheet at monad " << at(row,cols_nouns::bol_monad_num) << '\n'
                     << "Spreadsheet has " << at(row,cols_nouns::bol_surface)
                     << " Emdros has " << surface << '\n';
                return 1;
            }

            for (int i=0; i<3; ++i) {
                if (features[i] != at(row,int(cols_nouns::gn0) + i)) {
                    cerr << "Inconsistency between database and spreadsheet at monad " << at(row,cols_nouns::bol_monad_num) << " word is " << surface << '\n'
                         << "Spreadsheet has feature " << i << "=" << at(row,int(cols_nouns::gn0)+i)
                         << " Emdros has " << features[i] << '\n';
//                return 1;
                }
            }

            tr_gender(at(row,cols_nouns::gn1));
            tr_gender(at(row,cols_nouns::gn2));
            tr_gender(at(row,cols_nouns::gn3));
            tr_gender(at(row,cols_nouns::gn4));
            tr_gender(at(row,cols_nouns::gn5));

            tr_number(at(row,cols_nouns::nu1));
            tr_number(at(row,cols_nouns::nu2));
            tr_number(at(row,cols_nouns::nu3));
            tr_number(at(row,cols_nouns::nu4));
            tr_number(at(row,cols_nouns::nu5));

            tr_case(at(row,cols_nouns::ca1));
            tr_case(at(row,cols_nouns::ca2));
            tr_case(at(row,cols_nouns::ca3));
            tr_case(at(row,cols_nouns::ca4));
            tr_case(at(row,cols_nouns::ca5));


            switch (count_var_nouns(row)) {
              case 6:
                    sqlfile << "INSERT INTO hints VALUES(" << self << ",'" << noun_selector.diff6(row) << "');\n";
                    break;
              case 5:
                    sqlfile << "INSERT INTO hints VALUES(" << self << ",'" << noun_selector.diff5(row) << "');\n";
                    break;
              case 4:
                    sqlfile << "INSERT INTO hints VALUES(" << self << ",'" << noun_selector.diff4(row) << "');\n";
                    break;
              case 3:
                    sqlfile << "INSERT INTO hints VALUES(" << self << ",'" << noun_selector.diff3(row) << "');\n";
                    break;
              case 2:
                    sqlfile << "INSERT INTO hints VALUES(" << self << ",'" << noun_selector.diff2(row) << "');\n";
                    break;
            }
        }
    }

    //======================================================================
    // Handle verbs
    //======================================================================

    {
         selector verb_selector{verb_feat_diffs, verb_feat_diff2string, {0}, {1}};

         string csvfile = "AmbigiousVerbalForms20221021_BibleOL-export.csv";
    
        rapidcsv::Document csv;
        int csv_row = 0;
        
        try {
            csv.Load(csvfile);
        }
        catch (const ios_base::failure& e) {
            cerr << e.what() << "\n";
            cerr << "Cannot open " << csvfile << "\n";
            return 1;
        }

        string mql_request{"SELECT ALL OBJECTS WHERE [word psp=verb GET self,monad_num,surface,"
            "number,person,tense,voice,mood] GO"};
    
        if (!EE.executeString(mql_request, bResult, false, true))
            return 1;

        if (!EE.isSheaf()) {
            cerr << "ERROR: Result is not sheaf\n";
            return 1;
        }

        for (StrawOk str : SheafOk{EE.getSheaf()}) {
            long self = 0;
            int monad_num;
            string surface;
            string features[5]; //number,person,tense,voice,mood
        
            for (const MatchedObject mo : str) {
                self = mo.getFeatureAsLong(0);
                monad_num = mo.getFeatureAsLong(1);
                surface = mo.getFeatureAsString(2);
                for (int i=0; i<5; ++i)
                    features[i] = mo.getFeatureAsString(3+i);
            }

            vector<string> row;

            do {
                row = csv.GetRow<string>(csv_row++);
            } while (stoi(at(row,cols_verbs::bol_monad_num)) < monad_num);

        
            if (surface != at(row,cols_verbs::bol_surface)) {
                cerr << "Inconsistency between database and spreadsheet at monad " << at(row,cols_verbs::bol_monad_num) << '\n'
                     << "Spreadsheet has " << at(row,cols_verbs::bol_surface)
                     << " Emdros has " << surface << '\n';
                return 1;
            }

            tr_na(at(row,cols_verbs::nu0));
            tr_na(at(row,cols_verbs::nu1));
            tr_na(at(row,cols_verbs::pe0));
            tr_na(at(row,cols_verbs::pe1));

            for (int i=0; i<5; ++i) {
                if (features[i] != at(row,int(cols_verbs::nu0) + i)) {
                    cerr << "Inconsistency between database and spreadsheet at monad " << at(row,cols_verbs::bol_monad_num) << " word is " << surface << '\n'
                         << "Spreadsheet has feature " << i << "=" << at(row,int(cols_verbs::nu0)+i)
                         << " Emdros has " << features[i] << '\n';
//                return 1;
                }
            }

            switch (count_var_verbs(row)) {
              case 2:
                    sqlfile << "INSERT INTO hints VALUES(" << self << ",'" << verb_selector.diff2(row) << "');\n";
                    break;
            }
        }
    }
    
    sqlfile << "COMMIT;\n";
}
