for psp in "NA" "adjective" "adverb" "aramaic" "article" "cond" "conjunction" "correlative_or_interrogative_pronoun" "correlative_pronoun" "demonstrative_pronoun" "hebrew" "indefinite_pronoun" "interjection" "interrogative_pronoun" "letter_indeclinable" "noun" "noun_other_type_indeclinable" "numeral_indeclinable" "particle" "personal_pronoun" "possessive_pronoun" "preposition" "proper_noun_indeclinable" "reciprocal_pronoun" "reflexive_pronoun" "relative_pronoun" "verb"; do
    echo $psp
    echo "select all objects where [word psp=$psp get strongs, strongs_unreliable, lemma]" | mql -d nestle1904 | grep word | sed -e 's/.*strongs="\(.*\)",strongs_unreliable="\(.*\)",lemma="\(.*\)").*/\3,\1,\2/' | x2c | sort | uniq > $psp.txt
done
# < [ word 113642 { 113642 } false (strongs="5278",strongs_unreliable="false",lemma="ὑπομένω") //  <  > 
