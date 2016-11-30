#include "FCM.h"


/***********************************************************
    constructor
************************************************************/
FCM::FCM () {}


/***********************************************************
    getters and setters
************************************************************/
uint8_t FCM::getContextDepth () const { return contextDepth; }
void FCM::setContextDepth (uint8_t ctxDp) { FCM::contextDepth = ctxDp; }
uint32_t FCM::getAlphaDenom () const { return alphaDenom; }
void FCM::setAlphaDenom (uint32_t alphaDen) { FCM::alphaDenom = alphaDen; }
bool FCM::getInvertedRepeat () const { return invertedRepeat; }
void FCM::setInvertedRepeat (bool invRep) { FCM::invertedRepeat = invRep; }
const hashTable_t &FCM::getHashTable () const { return hashTable; }
void FCM::setHashTable (const hashTable_t &hT) { FCM::hashTable = hT; }
const std::string &FCM::getFileAddress () const { return fileAddress; }
void FCM::setFileAddress (const std::string &fA) { FCM::fileAddress = fA; }
