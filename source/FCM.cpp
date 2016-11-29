#include "FCM.h"


/***********************************************************
    constructor
************************************************************/
FCM::FCM () {}


/***********************************************************
    getters and setters
************************************************************/
uint8_t FCM::getContextDepth () const { return contextDepth; }
void FCM::setContextDepth (uint8_t contextDepth) { FCM::contextDepth = contextDepth; }
uint32_t FCM::getAlphaDenom () const { return alphaDenom; }
void FCM::setAlphaDenom (uint32_t alphaDenom) { FCM::alphaDenom = alphaDenom; }
const hashTable_t &FCM::getHTable () const { return hTable; }
void FCM::setHTable (const hashTable_t &hTable) { FCM::hTable = hTable; }
