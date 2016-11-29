#include "FCM.h"


/***********************************************************
    constructor
************************************************************/
FCM::FCM () {}


/***********************************************************
    getters and setters
************************************************************/
// getter of context depth
uint8_t FCM::getContextDepth () const { return contextDepth; }
// setter of context depth
void FCM::setContextDepth (uint8_t contextDepth) { FCM::contextDepth = contextDepth; }
// getter of alpha denominator
uint32_t FCM::getAlphaDenom () const { return alphaDenom; }
// setter of alpha denominator
void FCM::setAlphaDenom (uint32_t alphaDenom) { FCM::alphaDenom = alphaDenom; }
// getter of hash table
const hashTable_t& FCM::getHTable () const { return hTable; }
// setter of hash table
void FCM::setHTable (const hashTable_t& hTable) { FCM::hTable = hTable; }
