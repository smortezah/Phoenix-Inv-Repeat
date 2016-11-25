#ifndef PHOENIX_HASH_H
#define PHOENIX_HASH_H


#include <iostream>
#include <unordered_map>

#include "def.h"

// vaghti bishtar az 65,535 (uint16_t max) shod, hameye adad ha nesf mishan.
// ye variable ro ham tanzim kon ke maloom she chand bar nesf kardim
typedef std::unordered_map< std::string, std::array<uint16_t, ALPHABET_SIZE> > hashTable_t;


class Hash
{
public:
    Hash ();                                    // constructor
    hashTable_t hashTableBuild (std::string&, bool);   // build hash table
    void hashTablePrint (hashTable_t);          // show hash table



    hashTable_t hashTableUpdate (hashTable_t, std::string, std::string, bool);   // build hash table

};


#endif //PHOENIX_HASH_H
