#ifndef _OBJECTS_HPP
#define _OBJECTS_HPP

#include <climits>
#include <set>
#include <vector>
#include <utility>
#include <fstream>
#include <map>
#include <string>

class monads {
  public:
    monads(bool usethis) : _min{INT_MAX}, _max{0}, _usethis{usethis} {}

    void new_monad(int monad);

    bool is_consecutive(int& init) const;
    bool is_covering(int& init) const;

    const std::set<int>& get_monads() const { return _si; }
    int get_min() const { return _min; }
    int get_max() const { return _max; }
    bool useit() const { return _usethis; }

    const std::vector<std::pair<int,int>>& get_segments() const;
    std::map<std::string, std::string>& features() { return _features; }
    const std::map<std::string, std::string>& features() const { return _features; }

  private:
    int _min;
    int _max;
    std::set<int> _si;
    bool _usethis;
    mutable std::vector<std::pair<int,int>> _segments;
    std::map<std::string, std::string> _features;
};


class object_handler {
  public:
    void mql_end_obj(std::ostream& output) const;
    const std::vector<monads>& get_monads() const { return _objects; }
    void push(bool usethis);
    void mql_one_obj(std::ostream& output, const monads& m) const;
    virtual void mql_fields(std::ostream& output, const monads &m) const;
    void new_monad(int monad);

  protected:
    std::vector<monads> _objects; // All objects of this type

  private:
    std::set<int> _allmonads; // Keeps track of which monads have been used and should be omitted from subsequent objects
};

class sentence_handler : public object_handler {
  public:
    void mql_head(std::ostream& output) const;
    void mql_start_obj(std::ostream& output) const;
};

class clause_handler : public object_handler {
  public:
    clause_handler(int level) : _level{level} {}
    void push(bool usethis, const std::string& clause_type);
    virtual void mql_fields(std::ostream& output, const monads &m) const;
    void mql_head(std::ostream& output) const;
    void mql_start_obj(std::ostream& output) const;
  private:
    int _level;
};


#endif // _OBJECTS_HPP
