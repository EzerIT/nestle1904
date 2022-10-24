#ifndef _WALKER_HPP
#define _WALKER_HPP

#include <set>

#include "pugixml.hpp"
#include "objects.hpp"


class simple_walker : public pugi::xml_tree_walker {
  public:
    simple_walker(sentence_handler *shand, clause_handler *chands, int numlev)
        : _shand{shand}, _chands{chands}, _numlev{numlev}, _haslev{new bool[_numlev]} {}

    virtual bool for_each(pugi::xml_node& node);

  private:
    sentence_handler *_shand;
    clause_handler *_chands;
    int _numlev;
    bool *_haslev;
};


#endif // _WALKER_HPP
