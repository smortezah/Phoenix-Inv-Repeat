#ifndef PHOENIX_HASH_H
#define PHOENIX_HASH_H


#include <iostream>

#include "def.h"


class Hash
{
public:
    Hash ();                                                        // constructor

    hashTable_t hashTableBuild (const std::string&, const bool);    // build hash table
    void hashTablePrint (hashTable_t);                              // show hash table
};


#endif //PHOENIX_HASH_H