#ifndef _MQL_HPP
#define _MQL_HPP

#include <vector>
#include <ostream>


// Writes the initial information that goes into the MQL file
void mql_header(std::ostream& output);

// Writes the final information that goes into the MQL file
void mql_trailer(std::ostream& output);


// Writes MQL code that generates objects from the data stored in the specified container.
// Parameter:
//    container: A vector containing objects that are subclassed from mql_item
template <typename T>
void generate_mql_objects(std::ostream& output, const std::vector<T>& container)
{
    T::trans_start(output);
    for (const T& obj : container)
        obj.generate_object(output);
    T::trans_end(output);
}


#endif //  _MQL_HPP
