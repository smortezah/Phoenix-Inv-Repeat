#ifndef PHOENIX_FCM_H
#define PHOENIX_FCM_H


#include <iostream>

#include "def.h"


class FCM
{
public:
    FCM ();                                     // constructor
    
    void buildHashTable ();                     // build hash table
    void printHashTable (hashTable_t) const;    // print hash table
    
    uint8_t getContextDepth () const;           // getter of context depth
    void setContextDepth (uint8_t);             // setter of context depth
    uint32_t getAlphaDenom () const;            // getter of alpha denominator
    void setAlphaDenom (uint32_t);              // setter of alpha denominator
    bool getInvertedRepeat () const;            // getter of inverted repeat
    void setInvertedRepeat (bool);              // setter of inverted repeat
    const hashTable_t &getHashTable () const;   // getter of hash table
    void setHashTable (const hashTable_t&);     // setter of hash table
    const std::string &getFileAddress () const; // getter of file address
    void setFileAddress (const std::string&);   // setter of file address

private:
    uint8_t contextDepth;                       // context depth
    uint32_t alphaDenom;                        // alpha denominator
    bool invertedRepeat;                        // inverted repeat
    hashTable_t hashTable;                      // hash table
    std::string fileAddress;                    // file address
};


#endif //PHOENIX_FCM_H