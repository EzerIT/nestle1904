#ifndef _MQL_ITEM_HPP
#define _MQL_ITEM_HPP

#include <string>
#include <vector>

// This class aids in the generation of MQL code for object definition. An object of this class
// represents the definition of a single feature of an MQL object.
// A static function, define_obj, is provided to generate an MQL object definition from a vector of
// feature specifications.
class obj_definition {
  public:
    // Constructor.
    // Parameters:
    //    name: MQL object name
    //    type: MQL object type
    //    from_set: True if the feature is specified with "FROM SET"
    //    with_index: True if the feature is specified with "WITH INDEX"
    //    default_value: The default value of the feature. If omitted, it is set to "".
    obj_definition(const std::string& name, const std::string& type,
                   bool from_set, bool with_index,
                   const std::string& default_value = "")
        : m_name{name}, m_type{type}, m_from_set{from_set}, m_with_index{with_index}, m_default_value{default_value}
        {}

    // Constructor.
    // Parameters:
    //    name: MQL object name
    //    type: MQL object type
    obj_definition(const std::string& name, const std::string& type)
        : obj_definition(name, type, false, false)
        {}

    // Constructor.
    // Parameters:
    //    name: MQL object name
    //    type: MQL object type
    //    default_value: The default value of the feature
    obj_definition(const std::string& name, const std::string& type, const std::string& default_value)
        : obj_definition(name, type, false, false, default_value)
        {}

    // Static that generates the definition of an MQL object.
    // Parameters:
    //    output: The output stream where the MQL code will be written
    //    objtype: MQL object type
    //    single_monad: True if the object is a single monad
    //    defs: A vector of feature specfications
    static void define_obj(std::ostream& output, const std::string& objtype,
                           bool single_monad, const std::vector<obj_definition>& defs);

  private:
    std::string m_name;
    std::string m_type;
    bool        m_from_set;
    bool        m_with_index;
    std::string m_default_value;
};


// Models a contiguous monad range
class range {
  public:
    // Constructor.
    // Parameter:
    //    monad: The first monad in the range
    range(int monad) : m_first{monad}, m_last{monad} {}

    // Adds a monad to the range.
    // Parameter:
    //    monad: The monad to add to the range. This monad must be at most 1 larger than the previous last monad.
    void add(int monad);

    // Retrieves the first monad of the range
    int get_first() const { return m_first; }

    // Retrieves the last monad of the range
    int get_last() const { return m_last; }

  private:
    int m_first;
    int m_last;
};

//////////////////////////////////////////////////////////////////////

// Superclass for all MQL objects.
// All MQL objects have contiguous monad ranges.
class mql_item {
  public:
    // Constructor
    // Parameter:
    //    monad: The first monad in the range
    mql_item(int monad) : m_range(monad) {}

    // Adds a monad to the range.
    // Parameter:
    //    monad: The monad to add to the range. This monad must be at most 1 larger than the previous last monad.
    void range_add(int i) { m_range.add(i); }

    // Writes an object to the specified output stream.
    virtual void generate_object(std::ostream& output) const = 0;

  protected:
    // Writes MQL code required before the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    //    objtype: The type of the MQL object.
    static void trans_start(std::ostream& output, const std::string& objtype);

    // Writes MQL code required after the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    //    objtype: The type of the MQL object.
    static void trans_end(std::ostream& output, const std::string& objtype);

    range m_range;
};

//////////////////////////////////////////////////////////////////////

// Represents a book MQL object
class mql_book : public mql_item {
  public:
    // Constructor
    // Parameter:
    //    monad: The first monad in the range
    //    book: The name of the book
    mql_book(int monad, const std::string& book)
        : mql_item{monad}, m_book{book} {}

    // Writes MQL code required before the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    static void trans_start(std::ostream& output) { mql_item::trans_start(output, "book"); }

    // Writes MQL code required after the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    static void trans_end(std::ostream& output) { mql_item::trans_end(output, "book"); }

    // Writes CREATE OBJECT TYPE code
    // Parameters:
    //    output: Output stream for MQL commands.
    static void define_obj(std::ostream& output);

    // Writes CREATE OBJECT code
    // Parameters:
    //    output: Output stream for MQL commands.
    virtual void generate_object(std::ostream& output) const override;

    // Determines if a new word belongs to this object.
    // Parameter:
    //    b: Name of the book containing the new word
    // Returns:
    //    True if the new word belongs to this object
    bool same_object(const std::string& b) const { return m_book==b; }

  private:
    std::string m_book;
};


// Represents a chapter MQL object
class mql_chapter : public mql_item {
  public:
    // Constructor
    // Parameter:
    //    monad: The first monad in the range
    //    book: The name of the book
    //    chapter: The number of the chapter
    mql_chapter(int monad, const std::string& book, int chapter)
        : mql_item{monad}, m_book{book}, m_chapter{chapter} {}

    // Writes MQL code required before the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    static void trans_start(std::ostream& output) { mql_item::trans_start(output, "chapter"); }

    // Writes MQL code required after the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    static void trans_end(std::ostream& output) { mql_item::trans_end(output, "chapter"); }

    // Writes CREATE OBJECT TYPE code
    // Parameters:
    //    output: Output stream for MQL commands.
    static void define_obj(std::ostream& output);

    // Writes CREATE OBJECT code
    // Parameters:
    //    output: Output stream for MQL commands.
    virtual void generate_object(std::ostream& output) const override;

    // Determines if a new word belongs to this object.
    // Parameter:
    //    b: Name of the book containing the new word
    //    c: Number of the chapter containing the new word
    // Returns:
    //    True if the new word belongs to this object
    bool same_object(const std::string& b, int c) const { return m_book==b && m_chapter==c; }

  private:
    std::string m_book;
    int m_chapter;
};


// Represents a verse MQL object
class mql_verse : public mql_item {
  public:
    // Constructor
    // Parameter:
    //    monad: The first monad in the range
    //    book: The name of the book
    //    chapter: The number of the chapter
    //    verse: The number of the verse
    mql_verse(int monad, const std::string& book, int chapter, int verse)
        : mql_item{monad}, m_book{book}, m_chapter{chapter}, m_verse{verse} {}

    // Writes MQL code required before the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    static void trans_start(std::ostream& output) { mql_item::trans_start(output, "verse"); }

    // Writes MQL code required after the creation of objects
    // Parameters:
    //    output: Output stream for MQL commands.
    static void trans_end(std::ostream& output) { mql_item::trans_end(output, "verse"); }

    // Writes CREATE OBJECT TYPE code
    // Parameters:
    //    output: Output stream for MQL commands.
    static void define_obj(std::ostream& output);

    // Writes CREATE OBJECT code
    // Parameters:
    //    output: Output stream for MQL commands.
    virtual void generate_object(std::ostream& output) const override;

    // Determines if a new word belongs to this object.
    // Parameter:
    //    b: Name of the book containing the new word
    //    c: Number of the chapter containing the new word
    //    v: Number of the verse containing the new word
    // Returns:
    //    True if the new word belongs to this object
    bool same_object(const std::string& b, int c, int v) const { return m_book==b && m_chapter==c && m_verse==v; }

  private:
    std::string m_book;
    int m_chapter;
    int m_verse;
};




// This function is called whenever information about a new monad (word) is read.
// The argument vec is a container that contains existing objects of a particular type (e.g. chapters).
//
// The same_object() function of the last object in the container is called to determine if the
// new monad should cause a new object to be created or if the monad range of the existing object
// should be extended.
//
// Parameters:
// vec: A container of objects.
// a...: Arguments for same_object and for the constructor function for the objects.

template<typename CONTAINER, typename ... Args>
void add_monad(CONTAINER& vec, int monad, Args ... a)
{
    if (vec.empty()) // Container is empty: Create new object
        vec.emplace_back(monad, a...);
    else {
        auto& v = vec.back();
        if (v.same_object(a...)) // Should we use existing object
            v.range_add(monad); // Yes, extend the monad range of the existing object
        else
            vec.emplace_back(monad, a...); // No, push it onto the container
    }
}


#endif // _MQL_ITEM_HPP
