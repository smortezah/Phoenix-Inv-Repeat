#ifndef PHOENIX_HASH_H
#define PHOENIX_HASH_H

#include <iostream>
#include <unordered_map>


typedef std::unordered_map< std::string, std::array< int, 4> > mori;

class Hash
{
public:
    Hash ();// constructor
//    void hashTableBuild (std::string);
    mori hashTableBuild (std::string);// build hash table
    void hashTablePrint (mori);// show hash table
};


#endif //PHOENIX_HASH_H
