#ifndef _READ_INFLECTION_HPP_
#define _READ_INFLECTION_HPP_

#include <map>
#include <string>
#include "morph.hpp"

extern std::map<std::string,noun_stem_t> noun_stem_map;       // string{lemma,strongs,unreliable} => stem
extern std::map<std::string,noun_declension_t> noun_decl_map; // string{lemma,strongs,unreliable} => declination
extern std::map<std::string,verb_type_t> verb_type_map;       // string{lemma,strongs,unreliable} => stem

void read_inflection_spreadsheets();

#endif // _READ_INFLECTION_HPP_
