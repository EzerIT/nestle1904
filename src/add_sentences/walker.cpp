#include <cassert>
#include <stdexcept>

#include "nodeid2monad.hpp"
#include "walker.hpp"

using namespace std;

static const char* node_types[] =
{
    "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
};

static bool all_uc(const string& s)
{
    for (char c : s)
        if (c>='a' && c<='z')
            return false;
    return true;
}


bool simple_walker::for_each(pugi::xml_node& node)
{
    int dep = depth();

    if (node.type()==pugi::node_element &&
        dep>=1 && dep<=_numlev) {
        _haslev[dep-1] = true;
        for (int d=dep; d<_numlev; ++d)
            _haslev[d] = false;

        string cat{node.attribute("Cat").value()};

        _chands[dep-1].push(all_uc(cat), cat);
    }

    if (node.type()==pugi::node_pcdata) {
        pugi::xml_node p = node.parent();
        assert(!p.empty());

        pugi::xml_attribute a = p.attribute("nodeId");
        assert(!a.empty());

        int monad = nodeid2monad(a.value());

        _shand->new_monad(monad);

        for (int d=0; d<_numlev; ++d) {
            if (_haslev[d])
                _chands[d].new_monad(monad);
        }
    }

    return true; // continue traversal
}
