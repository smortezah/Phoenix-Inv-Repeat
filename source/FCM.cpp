#include "FCM.h"


/***********************************************************
    constructors
************************************************************/
FCM::FCM () {}

// another constructor
FCM::FCM (uint8_t contextDepth, uint32_t alphaDenom,
          const hashTable_t &hashTable, const std::string &fileAddress)
        : contextDepth(contextDepth), alphaDenom(alphaDenom),
          hashTable(hashTable), fileAddress(fileAddress) {}


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
const hashTable_t &FCM::getHashTable () const { return hashTable; }
// setter of hash table
void FCM::setHashTable (const hashTable_t &hashTable) { FCM::hashTable = hashTable; }
// getter of file address
const std::string &FCM::getFileAddress () const { return fileAddress; }
// setter of file address
void FCM::setFileAddress (const std::string &fileAddress) { FCM::fileAddress = fileAddress; }
