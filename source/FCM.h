#ifndef PHOENIX_FCM_H
#define PHOENIX_FCM_H


#include <iostream>

#include "def.h"


class FCM
{
public:
    FCM ();                                         // constructor
    
    uint8_t getContextDepth () const;               // getter of context depth
    void setContextDepth (uint8_t contextDepth);    // setter of context depth
    uint32_t getAlphaDenom () const;                // getter of alpha denominator
    void setAlphaDenom (uint32_t alphaDenom);       // setter of alpha denominator
    const hashTable_t& getHTable () const;          // getter of hash table
    void setHTable (const hashTable_t& hTable);     // setter of hash table

private:
    uint8_t contextDepth;                           // context depth
    uint32_t alphaDenom;                            // alpha denominator
    hashTable_t hTable;                             // hash table
};


#endif //PHOENIX_FCM_H
