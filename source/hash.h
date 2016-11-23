#ifndef PHOENIX_HASH_H
#define PHOENIX_HASH_H

#include <iostream>
#include <unordered_map>


typedef std::unordered_map< std::string, std::array<int, 4> > hashTable_t;


class Hash
{
public:
    Hash ();                                    // constructor
    hashTable_t hashTableBuild (std::string);   // build hash table
    void hashTablePrint (hashTable_t);          // show hash table
};


#endif //PHOENIX_HASH_H
