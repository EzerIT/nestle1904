#ifndef _MQL_WORD_HPP
#define _MQL_WORD_HPP

#include "mql_item.hpp"
#include "morph.hpp"



// Represents a word MQL object
class mql_word : public mql_item {
  public:
    // Constructor
    // Parameter:
    //    monad: The first monad in the range
    //    line: The line read from the bible text file
    mql_word(int monad, const std::string& line);

    // Writes MQL code required before the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    static void trans_start(std::ostream& output) { mql_item::trans_start(output, "word"); }

    // Writes MQL code required after the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    static void trans_end(std::ostream& output) { mql_item::trans_end(output, "word"); }

    // Writes CREATE OBJECT TYPE code
    // Parameters:
    //    output: Output stream for MQL commands.
    static void define_obj(std::ostream& output);

    // Writes CREATE OBJECT code
    // Parameters:
    //    output: Output stream for MQL commands.
    virtual void generate_object(std::ostream& output) const override;

    // Retrieves the Bible reference
    std::string get_ref() const { return m_ref; }

    // Generates occurrences and frequency rank
    static void set_freq(std::vector<mql_word>& words);

    // Generates inflection information
    static void set_inflection(std::vector<mql_word>& words);

  private:
    // Decodes the morphology string.
    void decode_morphology();

    std::string m_ref;
    std::string m_surface;
    std::string m_functional_tag;
    std::string m_form_tag;
    int         m_strongs;
    bool        m_strongs_unreliable;
    std::string m_lemma;
    std::string m_normalized;
    psp_t       m_psp;          // Part of speech
    case_t      m_case;
    number_t    m_number;
    number_t    m_possessor_number;
    gender_t    m_gender;
    person_t    m_person;
    tense_t     m_tense;
    voice_t     m_voice;
    mood_t      m_mood;
    suffix_t    m_suffix;
    verb_type_t m_verb_type;
    noun_stem_t m_noun_stem;
    noun_declension_t m_noun_declension;

    int         m_lexeme_occurrences;
    int         m_frequency_rank;
};

#endif // _MQL_WORD_HPP
