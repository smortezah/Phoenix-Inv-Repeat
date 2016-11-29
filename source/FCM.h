#ifndef PHOENIX_FCM_H
#define PHOENIX_FCM_H


#include <iostream>

#include "def.h"


class FCM
{
public:
    FCM ();
    
    uint8_t getContextDepth () const;
    void setContextDepth (uint8_t contextDepth);
    uint32_t getAlphaDenom () const;
    void setAlphaDenom (uint32_t alphaDenom);
    const hashTable_t& getHTable () const;
    void setHTable (const hashTable_t &hTable);

private:
    uint8_t     contextDepth;   // context depth
    uint32_t    alphaDenom;     // alpha denominator
    hashTable_t hTable;         // hash table
};


#endif //PHOENIX_FCM_H
