#include <iostream>
#include <tuple>
#include <map>
#include <algorithm>
#include <cassert>
#include "util.hpp"
#include "mql_word.hpp"
#include "strip.hpp"
#include "read_inflection.hpp"

using namespace std;

// See mql_word.hpp for documentation of the functions




mql_word::mql_word(int monad, const string& line)
    : mql_item{monad},
      m_verb_type{verb_type_t::NA}, 
      m_noun_stem{noun_stem_t::NA},
      m_noun_declension{noun_declension_t::NA}
{
    string strongs_string;

    tie(m_ref, m_surface, m_functional_tag, m_form_tag, strongs_string, m_lemma, m_normalized) = split7(line);

    m_strongs = stoi(strongs_string); // Discards optional second value
    m_strongs_unreliable = strongs_string[0]=='0';

    // Error in text:
    if (m_strongs==11391)
        m_strongs = 1391;

    decode_morphology();
}

void mql_word::set_freq(vector<mql_word>& words)
{
    map<tuple<string,int,bool>,int> lemmacount;	 // <lemma,strongs,strongs_unreliable> => count
	map<tuple<string,int,bool>,int> lemmarank;	 // <lemma,strongs,strongs_unreliable> => rank
    multimap<int,tuple<string,int,bool>,greater<int>> freq2lemma; // frequency => <lemma,strongs,strongs_unreliable>,
                                                                  // sorted by descending frequency

    for (const mql_word& w : words)
        ++lemmacount[tuple{w.m_lemma, w.m_strongs, w.m_strongs_unreliable}];

    for (const auto& me : lemmacount)
        freq2lemma.insert(make_pair(me.second, me.first));

    // Use frequencies to generate frequency ranks

    int rank = 1;		// Current rank
    int last_freq = 0;	// Last frequency inserted
    int last_rank = 0;	// Rank of last frequency inserted

    for (const auto& p : freq2lemma) {
        // p.first is frequency, p.second is <lemma,variant,part_of_speech>
        if (p.first==last_freq)
            lemmarank[p.second] = last_rank;
        else {
            lemmarank[p.second] = rank;
            last_freq = p.first;
            last_rank = rank;
        }
        ++rank;
    }

    for (mql_word& w : words) {
        auto key = tuple{w.m_lemma, w.m_strongs, w.m_strongs_unreliable};
        w.m_lexeme_occurrences = lemmacount[key];
        w.m_frequency_rank = lemmarank[key];
    }
}

void mql_word::set_inflection(vector<mql_word>& words)
{
    read_inflection_spreadsheets();

    for (mql_word& w : words) {
        auto key = w.m_lemma + "," + to_string(w.m_strongs) + "," + (w.m_strongs_unreliable ? "true" : "false");

        if (w.m_psp == psp_t::noun) {
            assert(noun_stem_map.contains(key));
            assert(noun_decl_map.contains(key));
            w.m_noun_stem = noun_stem_map[key];
            w.m_noun_declension = noun_decl_map[key];
        }
        else if (w.m_psp == psp_t::verb) {
            assert(verb_type_map.contains(key));
            w.m_verb_type = verb_type_map[key];
        }
    }            
}



void mql_word::define_obj(ostream& output)
{
    psp_morph.create_enum(output);   
    case_morph.create_enum(output);  
    number_morph.create_enum(output);
    gender_morph.create_enum(output);
    person_morph.create_enum(output);
    tense_morph.create_enum(output); 
    voice_morph.create_enum(output); 
    mood_morph.create_enum(output);  
    suffix_morph.create_enum(output);
    verb_type_morph.create_enum(output);
    noun_stem_morph.create_enum(output);
    noun_declension_morph.create_enum(output);

    obj_definition::define_obj(output, "word", true, {{ "ref", "string", true, false },
                                                      { "surface", "string", true, false },
                                                      { "functional_tag", "string", true, false },
                                                      { "form_tag", "string", true, false },
                                                      { "strongs", "integer" },
                                                      { "strongs_unreliable", "boolean_t", "false" },
                                                      { "lemma", "string", true, true },
                                                      { "raw_lemma", "string", true, true },
                                                      { "normalized", "string", true, true },
                                                      { "raw_normalized", "string", true, true },
                                                      { "psp", "psp_t", "NA" },
                                                      { "case", "case_t", "NA" },
                                                      { "number", "number_t", "NA" },
                                                      { "possessor_number", "number_t", "NA" },
                                                      { "gender", "gender_t", "NA" },
                                                      { "person", "person_t", "NA" },
                                                      { "tense", "tense_t", "NA" },
                                                      { "voice", "voice_t", "NA" },
                                                      { "mood", "mood_t", "NA" },
                                                      { "suffix", "suffix_t", "NA" },
                                                      { "verb_type", "verb_type_t", "NA" },
                                                      { "noun_stem", "noun_stem_t", "NA" },
                                                      { "noun_declension", "noun_declension_t", "NA" },
                                                      { "frequency_rank", "integer", "99999" },
                                                      { "lexeme_occurrences", "integer", "0" },
                                                      { "monad_num", "integer", "0" }});
}

void mql_word::generate_object(ostream& output) const
{
    output <<
        "CREATE OBJECT\n"
        "FROM MONADS= { " << m_range.get_first() << " }\n"
        "[\n"
        "    ref := \""              << m_ref                                              << "\";\n"
        "    surface := \""          << m_surface                                          << "\";\n"
        "    functional_tag := \""   << m_functional_tag                                   << "\";\n"
        "    form_tag := \""         << m_form_tag                                         << "\";\n"
        "    strongs := "            << m_strongs                                          << ";\n"
        "    strongs_unreliable := " <<(m_strongs_unreliable ? "true" : "false")           << ";\n"
        "    lemma := \""            << m_lemma                                            << "\";\n"
        "    raw_lemma := \""        << strip_string(m_lemma)                              << "\";\n"
        "    normalized := \""       << m_normalized                                       << "\";\n"
        "    raw_normalized := \""   << strip_string(m_normalized)                         << "\";\n"
        "    psp := "                << psp_morph.T2string(m_psp)                          << ";\n"
        "    case := "               << case_morph.T2string(m_case)                        << ";\n"
        "    number := "             << number_morph.T2string(m_number)                    << ";\n"
        "    possessor_number := "   << number_morph.T2string(m_possessor_number)          << ";\n"
        "    gender := "             << gender_morph.T2string(m_gender)                    << ";\n"
        "    person := "             << person_morph.T2string(m_person)                    << ";\n"
        "    tense := "              << tense_morph.T2string(m_tense)                      << ";\n"
        "    voice := "              << voice_morph.T2string(m_voice)                      << ";\n"
        "    mood := "               << mood_morph.T2string(m_mood)                        << ";\n"
        "    suffix := "             << suffix_morph.T2string(m_suffix)                    << ";\n"
        "    verb_type := "          << verb_type_morph.T2string(m_verb_type)              << ";\n"
        "    noun_stem := "          << noun_stem_morph.T2string(m_noun_stem)              << ";\n"
        "    noun_declension := "    << noun_declension_morph.T2string(m_noun_declension)  << ";\n"
        "    frequency_rank := "     << m_frequency_rank                                   << ";\n"
        "    lexeme_occurrences := " << m_lexeme_occurrences                               << ";\n"
        "    monad_num := "          << m_range.get_first()                                << ";\n"
        "]\n";
}


void mql_word::decode_morphology()
{
    string morph{m_form_tag};

    m_psp = psp_morph.decode_string(morph);
  
    switch (m_psp) {
      case psp_t::personal_pronoun:
            if (morph[0]=='1' || morph[0]=='2') {
                // Person specified, gender unspecified
                m_person = person_morph.decode_string(morph);
                m_case = case_morph.decode_string(morph);
                m_number = number_morph.decode_string(morph);
                m_gender = gender_t::NA;
            }
            else {
                // 3rd person assumed, gender specified
                m_person = person_t::third_person;
                m_case = case_morph.decode_string(morph);
                m_number = number_morph.decode_string(morph);
                m_gender = gender_morph.decode_string(morph);
            }

            if (!morph.empty() && morph[0] == '-')
                m_suffix = suffix_morph.decode_string(morph);
            break;


      case psp_t::possessive_pronoun:
            m_person = person_morph.decode_string(morph);
            m_possessor_number = number_morph.decode_string(morph);
            m_case = case_morph.decode_string(morph);
            m_number = number_morph.decode_string(morph);
            m_gender = gender_morph.decode_string(morph);
            break;

      case psp_t::reflexive_pronoun:
            m_person = person_morph.decode_string(morph);
            m_case = case_morph.decode_string(morph);
            m_number = number_morph.decode_string(morph);
            m_gender = gender_morph.decode_string(morph);
            break;

      case psp_t::noun:
      case psp_t::adjective:
      case psp_t::article:
      case psp_t::reciprocal_pronoun:
      case psp_t::demonstrative_pronoun:
      case psp_t::correlative_pronoun:
      case psp_t::interrogative_pronoun:
      case psp_t::relative_pronoun:
      case psp_t::correlative_or_interrogative_pronoun:
      case psp_t::indefinite_pronoun:
            m_case = case_morph.decode_string(morph);
            m_number = number_morph.decode_string(morph);
            m_gender = gender_morph.decode_string(morph);

            if (!morph.empty() && morph[0] == '-')
                m_suffix = suffix_morph.decode_string(morph);
            break;

      case psp_t::conjunction:
      case psp_t::cond:
      case psp_t::adverb:
      case psp_t::particle:
            if (!morph.empty() && morph[0] == '-')
                m_suffix = suffix_morph.decode_string(morph);
            break;

      case psp_t::verb:
            m_tense = tense_morph.decode_string(morph);
            m_voice = voice_morph.decode_string(morph);
            m_mood = mood_morph.decode_string(morph);

            switch (m_mood) {
              case mood_t::indicative:
              case mood_t::subjunctive:
              case mood_t::optative:
              case mood_t::imperative:
                    assert(morph[0]=='-');
                    morph = morph.substr(1); // Skip '-'
                    m_person = person_morph.decode_string(morph);
                    m_number = number_morph.decode_string(morph);
                    break;

              case mood_t::infinitive:
                    break;

              case mood_t::participle:
              case mood_t::imperative_participle:
                    assert(morph[0]=='-');
                    morph = morph.substr(1); // Skip '-'
                    m_case = case_morph.decode_string(morph);
                    m_number = number_morph.decode_string(morph);
                    m_gender = gender_morph.decode_string(morph);
                    break;
            }

            if (!morph.empty() && morph[0] == '-')
                m_suffix = suffix_morph.decode_string(morph);
            break;
    }

    assert(morph.empty());
}
