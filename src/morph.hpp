#ifndef _MORPH_HPP
#define _MORPH_HPP

#include <vector>
#include <utility>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <cassert>

/////////////////////////////////////////////////////////////////////////////
// class morph_info
/////////////////////////////////////////////////////////////////////////////

// This class handles MQL enumeration types that represent word morphology.
// The class handles generation of MQL enumeration definition, conversion of an enumeration value to
// its string representation, and decoding of a morphology string.
// Each MQL enumeration type must correspond to a C++ enumeration class. The template parameter T is
// the enumeration class.

template <typename T>
class morph_info {
  public:
    // Constructor.
    // Parameter:
    //     name: The MQL name of the enumeration.
    morph_info(const std::string& name) : m_name{name} {}

    // Prints an MQL definition of the enumeration type t the specified output stream
    void create_enum(std::ostream& output) const;

    // Converts an enumeration value to its string representation.
    // Parameter:
    //   t: The enumeration value to convert.
    static std::string T2string(T t);

    // Decodes a morphology string.
    // The program aborts if the morphology string does not represent a known enumeration value.
    // Parameter:
    //    morph: The morphology string. The intersting characters must be at the start of this
    //           string. When decoding is done, the converted characters have been stripped from
    //           morph.
    // Returns:
    //    The enumeration value.
    static T decode_string(std::string& morph);

  private:
    std::string m_name; // MQL name of enumeration
    static std::map<T, std::string> m_T2string; // Table for converting enum value to string representation
    static std::vector<std::pair<std::string, T>> m_str2T; // Table for converting morphology string to enum value
};


/////////////////////////////////////////////////////////////////////////////
// Implementation of template class functions
/////////////////////////////////////////////////////////////////////////////

template<typename T>
T morph_info<T>::decode_string(std::string& morph)
{
    auto found = find_if(begin(m_str2T), end(m_str2T),
                         [morph](const std::pair<std::string,T>& p) {
                             return morph.substr(0,p.first.size())==p.first;
                         });
    assert(found!=end(m_str2T));

    morph = morph.substr(found->first.size());
    return found->second;
}

template<typename T>
void morph_info<T>::create_enum(std::ostream& output) const
{
    output << "CREATE ENUMERATION " << m_name << " = {\n";

    bool first{true};
    for (const auto& x : m_T2string) {
        if (first)
            first = false;
        else
            output << ",\n";

        output << "    " << x.second;
    }
    output <<
        "\n"
        "}\n"
        "GO\n";
}


template<typename T>
std::string morph_info<T>::T2string(T t)
{
    return m_T2string.at(t);
}


/////////////////////////////////////////////////////////////////////////////
// Morphology enumerations used in the program
/////////////////////////////////////////////////////////////////////////////

enum class psp_t {
    NA,
    adverb,
    conjunction,
    cond,
    particle,
    preposition,
    interjection,
    aramaic,
    hebrew,
    proper_noun_indeclinable,
    numeral_indeclinable,
    letter_indeclinable,
    noun_other_type_indeclinable,
    noun,
    adjective,
    relative_pronoun,
    reciprocal_pronoun,
    demonstrative_pronoun,
    article,
    correlative_pronoun,
    interrogative_pronoun,
    indefinite_pronoun,
    correlative_or_interrogative_pronoun,
    reflexive_pronoun,
    possessive_pronoun,
    personal_pronoun,
    verb,
};

enum class case_t  {
    NA,
    nominative,
    vocative,
    genitive,
    dative,
    accusative,
};

enum class number_t {
    NA,
    singular,
    plural,
};

enum class gender_t {
    NA,
    masculine,
    feminine,
    neuter,
};

enum class person_t {
    NA,
    first_person,
    second_person,
    third_person,
};

enum class tense_t {
    NA,
    present,
    imperfect,
    future,
    second_future,
    aorist,
    second_aorist,
    perfect,
    second_perfect,
    pluperfect,
    second_pluperfect,
};

enum class voice_t {
    NA,
    active,
    middle,
    passive,
    middle_or_passive,
    middle_deponent,
    passive_deponent,
    middle_or_passive_deponent,
    impersonal_active,
};

enum class mood_t {
    NA,
    indicative,
    subjunctive,
    optative,
    imperative,
    infinitive,
    participle,
    imperative_participle,
};

enum class suffix_t {
    NA,
    superlative,
    comparative,
    interrogative,
    negative,
    attic,
    particle_attached,
    crasis,
};

enum class verb_type_t {
    NA,
    irregular,
    alpha,
    beta,
    gamma,
    delta,
    epsilon,
    epsilon_upsilon,
    zeta,
    eta,
    theta,
    iota,
    kappa,
    lambda,
    mu,
    mu_iota,
    nu,
    omicron,
    pi,
    rho,
    sigma_kappa,
    tau,
    upsilon,
    phi,
    khi
};

enum class noun_stem_t {
    NA,
    indeclinable,
    irregular,
    alpha,
    beta,
    gamma,
    delta,
    epsilon_upsilon,
    iota,
    kappa,
    kappa_tau,
    nu,
    nu_tau,
    omicron,
    omicron_upsilon,
    pi,
    rho,
    sigma,
    tau,
    upsilon,
    khi,
    omega
};

enum class noun_declension_t {
    NA,
    first_eta,
    first_alpha_breve,
    first_alpha_macron,
    first_alpha_macron_doric,
    second_d,
    second_attic,
    third_d,
    indeclinable,
    irregular
};

/////////////////////////////////////////////////////////////////////////////
// morph_info objects for the respective enumerations
/////////////////////////////////////////////////////////////////////////////

extern morph_info<psp_t>             psp_morph;
extern morph_info<case_t>            case_morph;
extern morph_info<number_t>          number_morph;
extern morph_info<gender_t>          gender_morph;
extern morph_info<person_t>          person_morph;
extern morph_info<tense_t>           tense_morph;
extern morph_info<voice_t>           voice_morph;
extern morph_info<mood_t>            mood_morph;
extern morph_info<suffix_t>          suffix_morph;
extern morph_info<verb_type_t>       verb_type_morph;
extern morph_info<noun_stem_t>       noun_stem_morph;
extern morph_info<noun_declension_t> noun_declension_morph;



#endif // _MORPH_HPP
