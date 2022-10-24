#include <map>
#include "util.hpp"
#include "strip.hpp"

using namespace std;

// Maps Greek characters to their lower case unaccented variant.
// An 'X' value indicates that the character should be removed from the word.
static map<char32_t, char32_t> stripmap {
    { U'\u0020', U'\u0020' },   // space
    { U'\u0028', U'X' },        // (
    { U'\u0029', U'X' },        // )
    { U'\u002c', U'X' },        // ,
    { U'\u002e', U'X' },        // .
    { U'\u005b', U'X' },        // [
    { U'\u005d', U'X' },        // ]

    { U'\u037e', U'X' },        // ;
    { U'\u0387', U'X' },        // ·

    { U'\u0391', U'\u03b1' },   // Α
    { U'\u0392', U'\u03b2' },   // Β
    { U'\u0393', U'\u03b3' },   // Γ
    { U'\u0394', U'\u03b4' },   // Δ
    { U'\u0395', U'\u03b5' },   // Ε
    { U'\u0396', U'\u03b6' },   // Ζ
    { U'\u0397', U'\u03b7' },   // Η
    { U'\u0398', U'\u03b8' },   // Θ
    { U'\u0399', U'\u03b9' },   // Ι
    { U'\u039a', U'\u03ba' },   // Κ
    { U'\u039b', U'\u03bb' },   // Λ
    { U'\u039c', U'\u03bc' },   // Μ
    { U'\u039d', U'\u03bd' },   // Ν
    { U'\u039e', U'\u03be' },   // Ξ
    { U'\u039f', U'\u03bf' },   // Ο

    { U'\u03a0', U'\u03c0' },   // Π
    { U'\u03a1', U'\u03c1' },   // Ρ
    { U'\u03a3', U'\u03c3' },   // Σ
    { U'\u03a4', U'\u03c4' },   // Τ
    { U'\u03a5', U'\u03c5' },   // Υ
    { U'\u03a6', U'\u03c6' },   // Φ
    { U'\u03a7', U'\u03c7' },   // Χ
    { U'\u03a8', U'\u03c8' },   // Ψ
    { U'\u03a9', U'\u03c9' },   // Ω

    { U'\u03b1', U'\u03b1' },   // α
    { U'\u03b2', U'\u03b2' },   // β
    { U'\u03b3', U'\u03b3' },   // γ
    { U'\u03b4', U'\u03b4' },   // δ
    { U'\u03b5', U'\u03b5' },   // ε
    { U'\u03b6', U'\u03b6' },   // ζ
    { U'\u03b7', U'\u03b7' },   // η
    { U'\u03b8', U'\u03b8' },   // θ
    { U'\u03b9', U'\u03b9' },   // ι
    { U'\u03ba', U'\u03ba' },   // κ
    { U'\u03bb', U'\u03bb' },   // λ
    { U'\u03bc', U'\u03bc' },   // μ
    { U'\u03bd', U'\u03bd' },   // ν
    { U'\u03be', U'\u03be' },   // ξ
    { U'\u03bf', U'\u03bf' },   // ο

    { U'\u03c0', U'\u03c0' },   // π
    { U'\u03c1', U'\u03c1' },   // ρ
    { U'\u03c2', U'\u03c2' },   // ς
    { U'\u03c3', U'\u03c3' },   // σ
    { U'\u03c4', U'\u03c4' },   // τ
    { U'\u03c5', U'\u03c5' },   // υ
    { U'\u03c6', U'\u03c6' },   // φ
    { U'\u03c7', U'\u03c7' },   // χ
    { U'\u03c8', U'\u03c8' },   // ψ
    { U'\u03c9', U'\u03c9' },   // ω
    { U'\u03ca', U'\u03b9' },   // ϊ
    { U'\u03cb', U'\u03c5' },   // ϋ


    { U'\u1f00', U'\u03b1' },   // ἀ
    { U'\u1f01', U'\u03b1' },   // ἁ
    { U'\u1f02', U'\u03b1' },   // ἂ
    { U'\u1f03', U'\u03b1' },   // ἃ
    { U'\u1f04', U'\u03b1' },   // ἄ
    { U'\u1f05', U'\u03b1' },   // ἅ
    { U'\u1f06', U'\u03b1' },   // ἆ
    { U'\u1f07', U'\u03b1' },   // ἇ
    { U'\u1f08', U'\u03b1' },   // Ἀ
    { U'\u1f09', U'\u03b1' },   // Ἁ
    { U'\u1f0a', U'\u03b1' },   // Ἂ
    { U'\u1f0b', U'\u03b1' },   // Ἃ
    { U'\u1f0c', U'\u03b1' },   // Ἄ
    { U'\u1f0d', U'\u03b1' },   // Ἅ
    { U'\u1f0e', U'\u03b1' },   // Ἆ
    { U'\u1f0f', U'\u03b1' },   // Ἇ

    { U'\u1f10', U'\u03b5' },   // ἐ
    { U'\u1f11', U'\u03b5' },   // ἑ
    { U'\u1f12', U'\u03b5' },   // ἒ
    { U'\u1f13', U'\u03b5' },   // ἓ
    { U'\u1f14', U'\u03b5' },   // ἔ
    { U'\u1f15', U'\u03b5' },   // ἕ
//  { U'\u1f16', U'\u03b5' },   //
//  { U'\u1f17', U'\u03b5' },   //
    { U'\u1f18', U'\u03b5' },   // Ἐ
    { U'\u1f19', U'\u03b5' },   // Ἑ
    { U'\u1f1a', U'\u03b5' },   // Ἒ
    { U'\u1f1b', U'\u03b5' },   // Ἓ
    { U'\u1f1c', U'\u03b5' },   // Ἔ
    { U'\u1f1d', U'\u03b5' },   // Ἕ
//  { U'\u1f1e', U'\u03b5' },   //
//  { U'\u1f1f', U'\u03b5' },   //

    { U'\u1f20', U'\u03b7' },   // ἠ
    { U'\u1f21', U'\u03b7' },   // ἡ
    { U'\u1f22', U'\u03b7' },   // ἢ
    { U'\u1f23', U'\u03b7' },   // ἣ
    { U'\u1f24', U'\u03b7' },   // ἤ
    { U'\u1f25', U'\u03b7' },   // ἥ
    { U'\u1f26', U'\u03b7' },   // ἦ
    { U'\u1f27', U'\u03b7' },   // ἧ
    { U'\u1f28', U'\u03b7' },   // Ἠ
    { U'\u1f29', U'\u03b7' },   // Ἡ
    { U'\u1f2a', U'\u03b7' },   // Ἢ
    { U'\u1f2b', U'\u03b7' },   // Ἣ
    { U'\u1f2c', U'\u03b7' },   // Ἤ
    { U'\u1f2d', U'\u03b7' },   // Ἥ
    { U'\u1f2e', U'\u03b7' },   // Ἦ
    { U'\u1f2f', U'\u03b7' },   // Ἧ

    { U'\u1f30', U'\u03b9' },   // ἰ
    { U'\u1f31', U'\u03b9' },   // ἱ
    { U'\u1f32', U'\u03b9' },   // ἲ
    { U'\u1f33', U'\u03b9' },   // ἳ
    { U'\u1f34', U'\u03b9' },   // ἴ
    { U'\u1f35', U'\u03b9' },   // ἵ
    { U'\u1f36', U'\u03b9' },   // ἶ
    { U'\u1f37', U'\u03b9' },   // ἷ
    { U'\u1f38', U'\u03b9' },   // Ἰ
    { U'\u1f39', U'\u03b9' },   // Ἱ
    { U'\u1f3a', U'\u03b9' },   // Ἲ
    { U'\u1f3b', U'\u03b9' },   // Ἳ
    { U'\u1f3c', U'\u03b9' },   // Ἴ
    { U'\u1f3d', U'\u03b9' },   // Ἵ
    { U'\u1f3e', U'\u03b9' },   // Ἶ
    { U'\u1f3f', U'\u03b9' },   // Ἷ

    { U'\u1f40', U'\u03bf' },   // ὀ
    { U'\u1f41', U'\u03bf' },   // ὁ
    { U'\u1f42', U'\u03bf' },   // ὂ
    { U'\u1f43', U'\u03bf' },   // ὃ
    { U'\u1f44', U'\u03bf' },   // ὄ
    { U'\u1f45', U'\u03bf' },   // ὅ
//  { U'\u1f46', U'\u03bf' },   //
//  { U'\u1f47', U'\u03bf' },   //
    { U'\u1f48', U'\u03bf' },   // Ὀ
    { U'\u1f49', U'\u03bf' },   // Ὁ
    { U'\u1f4a', U'\u03bf' },   // Ὂ
    { U'\u1f4b', U'\u03bf' },   // Ὃ
    { U'\u1f4c', U'\u03bf' },   // Ὄ
    { U'\u1f4d', U'\u03bf' },   // Ὅ
//  { U'\u1f4e', U'\u03bf' },   //
//  { U'\u1f4f', U'\u03bf' },   //

    { U'\u1f50', U'\u03c5' },   // ὐ
    { U'\u1f51', U'\u03c5' },   // ὑ
    { U'\u1f52', U'\u03c5' },   // ὒ
    { U'\u1f53', U'\u03c5' },   // ὓ
    { U'\u1f54', U'\u03c5' },   // ὔ
    { U'\u1f55', U'\u03c5' },   // ὕ
    { U'\u1f56', U'\u03c5' },   // ὖ
    { U'\u1f57', U'\u03c5' },   // ὗ
//  { U'\u1f58', U'\u03c5' },   //
    { U'\u1f59', U'\u03c5' },   // Ὑ
//  { U'\u1f5a', U'\u03c5' },   //
    { U'\u1f5b', U'\u03c5' },   // Ὓ
//  { U'\u1f5c', U'\u03c5' },   //
    { U'\u1f5d', U'\u03c5' },   // Ὕ
//  { U'\u1f5e', U'\u03c5' },   //
    { U'\u1f5f', U'\u03c5' },   // Ὗ

    { U'\u1f60', U'\u03c9' },   // ὠ
    { U'\u1f61', U'\u03c9' },   // ὡ
    { U'\u1f62', U'\u03c9' },   // ὢ
    { U'\u1f63', U'\u03c9' },   // ὣ
    { U'\u1f64', U'\u03c9' },   // ὤ
    { U'\u1f65', U'\u03c9' },   // ὥ
    { U'\u1f66', U'\u03c9' },   // ὦ
    { U'\u1f67', U'\u03c9' },   // ὧ
    { U'\u1f68', U'\u03c9' },   // Ὠ
    { U'\u1f69', U'\u03c9' },   // Ὡ
    { U'\u1f6a', U'\u03c9' },   // Ὢ
    { U'\u1f6b', U'\u03c9' },   // Ὣ
    { U'\u1f6c', U'\u03c9' },   // Ὤ
    { U'\u1f6d', U'\u03c9' },   // Ὥ
    { U'\u1f6e', U'\u03c9' },   // Ὦ
    { U'\u1f6f', U'\u03c9' },   // Ὧ

    { U'\u1f70', U'\u03b1' },   // ὰ
    { U'\u1f71', U'\u03b1' },   // ά
    { U'\u1f72', U'\u03b5' },   // ὲ
    { U'\u1f73', U'\u03b5' },   // έ
    { U'\u1f74', U'\u03b7' },   // ὴ
    { U'\u1f75', U'\u03b7' },   // ή
    { U'\u1f76', U'\u03b9' },   // ὶ
    { U'\u1f77', U'\u03b9' },   // ί
    { U'\u1f78', U'\u03bf' },   // ὸ
    { U'\u1f79', U'\u03bf' },   // ό
    { U'\u1f7a', U'\u03c5' },   // ὺ
    { U'\u1f7b', U'\u03c5' },   // ύ
    { U'\u1f7c', U'\u03c9' },   // ὼ
    { U'\u1f7d', U'\u03c9' },   // ώ
//  { U'\u1f7e', U'X' },   //
//  { U'\u1f7f', U'X' },   //

    { U'\u1f80', U'\u03c9' },   // ᾀ
    { U'\u1f81', U'\u03c9' },   // ᾁ
    { U'\u1f82', U'\u03c9' },   // ᾂ
    { U'\u1f83', U'\u03c9' },   // ᾃ
    { U'\u1f84', U'\u03c9' },   // ᾄ
    { U'\u1f85', U'\u03c9' },   // ᾅ
    { U'\u1f86', U'\u03c9' },   // ᾆ
    { U'\u1f87', U'\u03c9' },   // ᾇ
    { U'\u1f88', U'\u03c9' },   // ᾈ
    { U'\u1f89', U'\u03c9' },   // ᾉ
    { U'\u1f8a', U'\u03c9' },   // ᾊ
    { U'\u1f8b', U'\u03c9' },   // ᾋ
    { U'\u1f8c', U'\u03c9' },   // ᾌ
    { U'\u1f8d', U'\u03c9' },   // ᾍ
    { U'\u1f8e', U'\u03c9' },   // ᾎ
    { U'\u1f8f', U'\u03c9' },   // ᾏ

    { U'\u1f90', U'\u03b7' },   // ᾐ
    { U'\u1f91', U'\u03b7' },   // ᾑ
    { U'\u1f92', U'\u03b7' },   // ᾒ
    { U'\u1f93', U'\u03b7' },   // ᾓ
    { U'\u1f94', U'\u03b7' },   // ᾔ
    { U'\u1f95', U'\u03b7' },   // ᾕ
    { U'\u1f96', U'\u03b7' },   // ᾖ
    { U'\u1f97', U'\u03b7' },   // ᾗ
    { U'\u1f98', U'\u03b7' },   // ᾘ
    { U'\u1f99', U'\u03b7' },   // ᾙ
    { U'\u1f9a', U'\u03b7' },   // ᾚ
    { U'\u1f9b', U'\u03b7' },   // ᾛ
    { U'\u1f9c', U'\u03b7' },   // ᾜ
    { U'\u1f9d', U'\u03b7' },   // ᾝ
    { U'\u1f9e', U'\u03b7' },   // ᾞ
    { U'\u1f9f', U'\u03b7' },   // ᾟ

    { U'\u1fa0', U'\u03c9' },   // ᾠ
    { U'\u1fa1', U'\u03c9' },   // ᾡ
    { U'\u1fa2', U'\u03c9' },   // ᾢ
    { U'\u1fa3', U'\u03c9' },   // ᾣ
    { U'\u1fa4', U'\u03c9' },   // ᾤ
    { U'\u1fa5', U'\u03c9' },   // ᾥ
    { U'\u1fa6', U'\u03c9' },   // ᾦ
    { U'\u1fa7', U'\u03c9' },   // ᾧ
    { U'\u1fa8', U'\u03c9' },   // ᾨ
    { U'\u1fa9', U'\u03c9' },   // ᾩ
    { U'\u1faa', U'\u03c9' },   // ᾪ
    { U'\u1fab', U'\u03c9' },   // ᾫ
    { U'\u1fac', U'\u03c9' },   // ᾬ
    { U'\u1fad', U'\u03c9' },   // ᾭ
    { U'\u1fae', U'\u03c9' },   // ᾮ
    { U'\u1faf', U'\u03c9' },   // ᾯ

    { U'\u1fb0', U'\u03b1' },   // ᾰ
    { U'\u1fb1', U'\u03b1' },   // ᾱ
    { U'\u1fb2', U'\u03b1' },   // ᾲ
    { U'\u1fb3', U'\u03b1' },   // ᾳ
    { U'\u1fb4', U'\u03b1' },   // ᾴ
//  { U'\u1fb5', U'\u03b1' },   //
    { U'\u1fb6', U'\u03b1' },   // ᾶ
    { U'\u1fb7', U'\u03b1' },   // ᾷ
    { U'\u1fb8', U'\u03b1' },   // Ᾰ
    { U'\u1fb9', U'\u03b1' },   // Ᾱ
    { U'\u1fba', U'\u03b1' },   // Ὰ
    { U'\u1fbb', U'\u03b1' },   // Ά
    { U'\u1fbc', U'\u03b1' },   // ᾼ
    { U'\u1fbd', U'X' },   // ᾽
    { U'\u1fbe', U'X' },   // ι
    { U'\u1fbf', U'X' },   // ᾿

    { U'\u1fc0', U'X' },   // ῀
    { U'\u1fc1', U'X' },   // ῁
    { U'\u1fc2', U'\u03b7' },   // ῂ
    { U'\u1fc3', U'\u03b7' },   // ῃ
    { U'\u1fc4', U'\u03b7' },   // ῄ
//  { U'\u1fc5', U'\u03b7' },   //
    { U'\u1fc6', U'\u03b7' },   // ῆ
    { U'\u1fc7', U'\u03b7' },   // ῇ
    { U'\u1fc8', U'\u03b5' },   // Ὲ
    { U'\u1fc9', U'\u03b5' },   // Έ
    { U'\u1fca', U'\u03b7' },   // Ὴ
    { U'\u1fcb', U'\u03b7' },   // Ή
    { U'\u1fcc', U'\u03b7' },   // ῌ
    { U'\u1fcd', U'X' },        // ῍
    { U'\u1fce', U'X' },        // ῎
    { U'\u1fcf', U'X' },        // ῏

    { U'\u1fd0', U'\u03b9' },   // ῐ
    { U'\u1fd1', U'\u03b9' },   // ῑ
    { U'\u1fd2', U'\u03b9' },   // ῒ
    { U'\u1fd3', U'\u03b9' },   // ΐ
//  { U'\u1fd4', U'\u03b9' },   // ῔
//  { U'\u1fd5', U'\u03b9' },   // ῕
    { U'\u1fd6', U'\u03b9' },   // ῖ
    { U'\u1fd7', U'\u03b9' },   // ῗ
    { U'\u1fd8', U'\u03b9' },   // Ῐ
    { U'\u1fd9', U'\u03b9' },   // Ῑ
    { U'\u1fda', U'\u03b9' },   // Ὶ
    { U'\u1fdb', U'\u03b9' },   // Ί
//  { U'\u1fdc', U'\u03b9' },   //
    { U'\u1fdd', U'X' },        // ῝
    { U'\u1fde', U'X' },        // ῞
    { U'\u1fdf', U'X' },        // ῟

    { U'\u1fe0', U'\u03c5' },   // ῠ
    { U'\u1fe1', U'\u03c5' },   // ῡ
    { U'\u1fe2', U'\u03c5' },   // ῢ
    { U'\u1fe3', U'\u03c5' },   // ΰ
    { U'\u1fe4', U'\u03c1' },   // ῤ
    { U'\u1fe5', U'\u03c1' },   // ῥ
    { U'\u1fe6', U'\u03c5' },   // ῦ
    { U'\u1fe7', U'\u03c5' },   // ῧ
    { U'\u1fe8', U'\u03c5' },   // Ῠ
    { U'\u1fe9', U'\u03c5' },   // Ῡ
    { U'\u1fea', U'\u03c5' },   // Ὺ
    { U'\u1feb', U'\u03c5' },   // Ύ
    { U'\u1fec', U'\u03c1' },   // Ῥ
    { U'\u1fed', U'X' },        // ῭
    { U'\u1fee', U'X' },        // ΅
    { U'\u1fef', U'X' },        // `

//  { U'\u1ff0', U'\u03c9' },   //
//  { U'\u1ff1', U'\u03c9' },   //
    { U'\u1ff2', U'\u03c9' },   // ῲ
    { U'\u1ff3', U'\u03c9' },   // ῳ
    { U'\u1ff4', U'\u03c9' },   // ῴ
//  { U'\u1ff5', U'\u03c9' },   //
    { U'\u1ff6', U'\u03c9' },   // ῶ
    { U'\u1ff7', U'\u03c9' },   // ῷ
    { U'\u1ff8', U'\u03bf' },   // Ὸ
    { U'\u1ff9', U'\u03bf' },   // Ό
    { U'\u1ffa', U'\u03c9' },   // Ὼ
    { U'\u1ffb', U'\u03c9' },   // Ώ
    { U'\u1ffc', U'\u03c9' },   // ῼ


    { U'\u2014', U'X' },        // —
    { U'\u2019', U'X' },        // ’
};


// Removes non-letters from string and replaces characters with lower case unaccented variants.
// Argument:
//   s: Source string
// Returns:
//   Modified string

string strip_string(const string& s)
{
    u32string source {u8_to_u32(s)};
    u32string result;

    // Remove trailing " (I)" and " (II)"
    replace_string_in_place(source, U" (I)", U"");
    replace_string_in_place(source, U" (II)", U"");

    // Replace with unaccented lower case characters
    for (char32_t c : source) {
        char32_t r {stripmap.at(c)};
        if (r!=U'X')
            result.push_back(r);
    }

    return u32_to_u8(result);
}
