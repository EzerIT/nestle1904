#include <iostream>
#include "morph.hpp"


using namespace std;



/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<psp_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<psp_t> psp_morph("psp_t");

template<>
map<psp_t, string> morph_info<psp_t>::m_T2string {
    { psp_t::NA,                                   "NA" },
    { psp_t::adverb,                               "adverb" },
    { psp_t::conjunction,                          "conjunction" },
    { psp_t::cond,                                 "cond" },
    { psp_t::particle,                             "particle" },
    { psp_t::preposition,                          "preposition" },
    { psp_t::interjection,                         "interjection" },
    { psp_t::aramaic,                              "aramaic" },
    { psp_t::hebrew,                               "hebrew" },
    { psp_t::proper_noun_indeclinable,             "proper_noun_indeclinable" },
    { psp_t::numeral_indeclinable,                 "numeral_indeclinable" },
    { psp_t::letter_indeclinable,                  "letter_indeclinable" },
    { psp_t::noun_other_type_indeclinable,         "noun_other_type_indeclinable" },
    { psp_t::noun,                                 "noun" },
    { psp_t::adjective,                            "adjective" },
    { psp_t::relative_pronoun,                     "relative_pronoun" },
    { psp_t::reciprocal_pronoun,                   "reciprocal_pronoun" },
    { psp_t::demonstrative_pronoun,                "demonstrative_pronoun" },
    { psp_t::article,                              "article" },
    { psp_t::correlative_pronoun,                  "correlative_pronoun" },
    { psp_t::interrogative_pronoun,                "interrogative_pronoun" },
    { psp_t::indefinite_pronoun,                   "indefinite_pronoun" },
    { psp_t::correlative_or_interrogative_pronoun, "correlative_or_interrogative_pronoun" },
    { psp_t::reflexive_pronoun,                    "reflexive_pronoun" },
    { psp_t::possessive_pronoun,                   "possessive_pronoun" },
    { psp_t::personal_pronoun,                     "personal_pronoun" },
    { psp_t::verb,                                 "verb" },
};

template<>
vector<pair<string, psp_t>> morph_info<psp_t>::m_str2T {
    // Note: A shorter string must follow a longer string of which it is a substring.
    // E.g. "N-" must come after "N-PRI"
    { "ADV",   psp_t::adverb },
    { "CONJ",  psp_t::conjunction },
    { "COND",  psp_t::cond },
    { "PRT",   psp_t::particle },
    { "PREP",  psp_t::preposition },
    { "INJ",   psp_t::interjection },
    { "ARAM",  psp_t::aramaic },
    { "HEB",   psp_t::hebrew },
    { "N-PRI", psp_t::proper_noun_indeclinable },
    { "A-NUI", psp_t::numeral_indeclinable },
    { "N-LI",  psp_t::letter_indeclinable },
    { "N-OI",  psp_t::noun_other_type_indeclinable },
    { "N-",    psp_t::noun },
    { "A-",    psp_t::adjective },
    { "R-",    psp_t::relative_pronoun },
    { "C-",    psp_t::reciprocal_pronoun },
    { "D-",    psp_t::demonstrative_pronoun },
    { "T-",    psp_t::article },
    { "K-",    psp_t::correlative_pronoun },
    { "I-",    psp_t::interrogative_pronoun },
    { "X-",    psp_t::indefinite_pronoun },
    { "Q-",    psp_t::correlative_or_interrogative_pronoun },
    { "F-",    psp_t::reflexive_pronoun },
    { "S-",    psp_t::possessive_pronoun },
    { "P-",    psp_t::personal_pronoun },
    { "V-",    psp_t::verb },
};

/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<case_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<case_t> case_morph("case_t");

template<>
map<case_t, string> morph_info<case_t>::m_T2string {
    { case_t::NA,         "NA" },
    { case_t::nominative, "nominative" },
    { case_t::vocative,   "vocative" },
    { case_t::genitive,   "genitive" },
    { case_t::dative,     "dative" },
    { case_t::accusative, "accusative" },
};

template<>
vector<pair<string, case_t>> morph_info<case_t>::m_str2T {
    { "N", case_t::nominative },
    { "V", case_t::vocative },
    { "G", case_t::genitive },
    { "D", case_t::dative },
    { "A", case_t::accusative },
};


/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<number_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<number_t> number_morph("number_t");

template<>
map<number_t, string> morph_info<number_t>::m_T2string {
    { number_t::NA,       "NA" },
    { number_t::singular, "singular" },
    { number_t::plural,   "plural" },
};

template<>
vector<pair<string, number_t>> morph_info<number_t>::m_str2T {
    { "S", number_t::singular },
    { "P", number_t::plural },
};


/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<gender_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<gender_t> gender_morph("gender_t");

template<>
map<gender_t, string> morph_info<gender_t>::m_T2string {
    { gender_t::NA,        "NA" },
    { gender_t::masculine, "masculine" },
    { gender_t::feminine,  "feminine" },
    { gender_t::neuter,    "neuter" },
};

template<>
vector<pair<string, gender_t>> morph_info<gender_t>::m_str2T {
    { "M", gender_t::masculine },
    { "F", gender_t::feminine },
    { "N", gender_t::neuter },
};


/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<person_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<person_t> person_morph("person_t");

template<>
map<person_t, string> morph_info<person_t>::m_T2string {
    { person_t::NA,            "NA" },
    { person_t::first_person,  "first_person" },
    { person_t::second_person, "second_person" },
    { person_t::third_person,  "third_person" },
};

template<>
vector<pair<string, person_t>> morph_info<person_t>::m_str2T {
    { "1", person_t::first_person },
    { "2", person_t::second_person },
    { "3", person_t::third_person },
};


/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<tense_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<tense_t> tense_morph("tense_t");

template<>
map<tense_t, string> morph_info<tense_t>::m_T2string {
    { tense_t::NA,               "NA" },
    { tense_t::present,          "present" },
    { tense_t::imperfect,        "imperfect" },
    { tense_t::future,           "future" },
    { tense_t::second_future,    "second_future" },
    { tense_t::aorist,           "aorist" },
    { tense_t::second_aorist,    "second_aorist" },
    { tense_t::perfect,          "perfect" },
    { tense_t::second_perfect,   "second_perfect" },
    { tense_t::pluperfect,       "pluperfect" },
    { tense_t::second_pluperfect,"second_pluperfect" },
};

template<>
vector<pair<string, tense_t>> morph_info<tense_t>::m_str2T {
    { "P", tense_t::present },
    { "I", tense_t::imperfect },
    { "F", tense_t::future },
    { "2F", tense_t::second_future },
    { "A", tense_t::aorist },
    { "2A", tense_t::second_aorist },
    { "R", tense_t::perfect },
    { "2R", tense_t::second_perfect },
    { "L", tense_t::pluperfect },
    { "2L", tense_t::second_pluperfect },
};


/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<voice_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<voice_t> voice_morph("voice_t");

template<>
map<voice_t, string> morph_info<voice_t>::m_T2string {
    { voice_t::NA,                         "NA" },
    { voice_t::active,                     "active" },
    { voice_t::middle,                     "middle" },
    { voice_t::passive,                    "passive" },
    { voice_t::middle_or_passive,          "middle_or_passive" },
    { voice_t::middle_deponent,            "middle_deponent" },
    { voice_t::passive_deponent,           "passive_deponent" },
    { voice_t::middle_or_passive_deponent, "middle_or_passive_deponent" },
    { voice_t::impersonal_active,          "impersonal_active" },
};

template<>
vector<pair<string, voice_t>> morph_info<voice_t>::m_str2T {
    { "A", voice_t::active },
    { "M", voice_t::middle },
    { "P", voice_t::passive },
    { "E", voice_t::middle_or_passive },
    { "D", voice_t::middle_deponent },
    { "O", voice_t::passive_deponent },
    { "N", voice_t::middle_or_passive_deponent },
    { "Q", voice_t::impersonal_active },
};


/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<mood_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<mood_t> mood_morph("mood_t");

template<>
map<mood_t, string> morph_info<mood_t>::m_T2string {
    { mood_t::NA,                     "NA" },
    { mood_t::indicative,             "indicative" },
    { mood_t::subjunctive,            "subjunctive" },
    { mood_t::optative,               "optative" },
    { mood_t::imperative,             "imperative" },
    { mood_t::infinitive,             "infinitive" },
    { mood_t::participle,             "participle" },
    { mood_t::imperative_participle,  "imperative_participle" },
};

template<>
vector<pair<string, mood_t>> morph_info<mood_t>::m_str2T {
    { "I", mood_t::indicative },
    { "S", mood_t::subjunctive },
    { "O", mood_t::optative },
    { "M", mood_t::imperative },
    { "N", mood_t::infinitive },
    { "P", mood_t::participle },
    { "R", mood_t::imperative_participle },
};


/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<suffix_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<suffix_t> suffix_morph("suffix_t");

template<>
map<suffix_t, string> morph_info<suffix_t>::m_T2string {
    { suffix_t::NA,                "NA" },
    { suffix_t::superlative,       "superlative" },
    { suffix_t::comparative,       "comparative" },
//  { suffix_t::abbreviated,       "abbreviated" },
    { suffix_t::interrogative,     "interrogative" },
    { suffix_t::negative,          "negative" },
//  { suffix_t::contracted_form,   "contracted_form" },
    { suffix_t::attic,             "attic" },
    { suffix_t::particle_attached, "particle_attached" },
    { suffix_t::crasis,            "crasis" },
};

template<>
vector<pair<string, suffix_t>> morph_info<suffix_t>::m_str2T {
    { "-S", suffix_t::superlative },
    { "-C", suffix_t::comparative },
//  { "-ABB", suffix_t::abbreviated },
    { "-I", suffix_t::interrogative },
    { "-N", suffix_t::negative },
    { "-ATT", suffix_t::attic },
    { "-P", suffix_t::particle_attached },
    { "-K", suffix_t::crasis },
};


/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<verb_type_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<verb_type_t> verb_type_morph("verb_type_t");

template<>
map<verb_type_t, string> morph_info<verb_type_t>::m_T2string {
    { verb_type_t::NA,              "NA" },
    { verb_type_t::irregular,       "irregular" },
    { verb_type_t::alpha,           "alpha" },
    { verb_type_t::beta,            "beta" },
    { verb_type_t::gamma,           "gamma" },
    { verb_type_t::delta,           "delta" },
    { verb_type_t::epsilon,         "epsilon" },
    { verb_type_t::epsilon_upsilon, "epsilon_upsilon" },
    { verb_type_t::zeta,            "zeta" },
    { verb_type_t::eta,             "eta" },
    { verb_type_t::theta,           "theta" },
    { verb_type_t::iota,            "iota" },
    { verb_type_t::kappa,           "kappa" },
    { verb_type_t::lambda,          "lambda" },
    { verb_type_t::mu,              "mu" },
    { verb_type_t::mu_iota,         "mu_iota" },
    { verb_type_t::nu,              "nu" },
    { verb_type_t::omicron,         "omicron" },
    { verb_type_t::pi,              "pi" },
    { verb_type_t::rho,             "rho" },
    { verb_type_t::sigma_kappa,     "sigma_kappa" },
    { verb_type_t::tau,             "tau" },
    { verb_type_t::upsilon,         "upsilon" },
    { verb_type_t::phi,             "phi" },
    { verb_type_t::khi,             "khi" }
};




/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<noun_stem_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<noun_stem_t> noun_stem_morph("noun_stem_t");

template<>
map<noun_stem_t, string> morph_info<noun_stem_t>::m_T2string {
    { noun_stem_t::NA,              "NA" },
    { noun_stem_t::indeclinable,    "indeclinable" },
    { noun_stem_t::irregular,       "irregular" },
    { noun_stem_t::alpha,           "alpha" },
    { noun_stem_t::beta,            "beta" },
    { noun_stem_t::gamma,           "gamma" },
    { noun_stem_t::delta,           "delta" },
    { noun_stem_t::epsilon_upsilon, "epsilon_upsilon" },
    { noun_stem_t::iota,            "iota" },
    { noun_stem_t::kappa,           "kappa" },
    { noun_stem_t::kappa_tau,       "kappa_tau" },
    { noun_stem_t::nu,              "nu" },
    { noun_stem_t::nu_tau,          "nu_tau" },
    { noun_stem_t::omicron,         "omicron" },
    { noun_stem_t::omicron_upsilon, "omicron_upsilon" },
    { noun_stem_t::pi,              "pi" },
    { noun_stem_t::rho,             "rho" },
    { noun_stem_t::sigma,           "sigma" },
    { noun_stem_t::tau,             "tau" },
    { noun_stem_t::upsilon,         "upsilon" },
    { noun_stem_t::khi,             "khi" },
    { noun_stem_t::omega,           "omega" }
};

/////////////////////////////////////////////////////////////////////////////
// Implementation of morph_info<noun_declension_t>
/////////////////////////////////////////////////////////////////////////////

morph_info<noun_declension_t> noun_declension_morph("noun_declension_t");

template<>
map<noun_declension_t, string> morph_info<noun_declension_t>::m_T2string {
    { noun_declension_t::NA,                       "NA" },
    { noun_declension_t::first_eta,                "first_eta" },
    { noun_declension_t::first_alpha_breve,        "first_alpha_breve" },
    { noun_declension_t::first_alpha_macron,       "first_alpha_macron" },
    { noun_declension_t::first_alpha_macron_doric, "first_alpha_macron_doric" },
    { noun_declension_t::second_d,                 "second_d" },
    { noun_declension_t::second_attic,             "second_attic" },
    { noun_declension_t::third_d,                  "third_d" },
    { noun_declension_t::indeclinable,             "indeclinable" },
    { noun_declension_t::irregular,                "irregular" },
};


