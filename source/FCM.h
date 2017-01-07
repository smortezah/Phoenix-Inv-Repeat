#ifndef PHOENIX_FCM_H
#define PHOENIX_FCM_H


#include <iostream>

#include "def.h"

using std::string;


class FCM
{
public:
    FCM ();                                         /// constructor
    
    void buildTable ();                             /// build table
    void buildHashTable ();                         /// build hash table (int key)
    //    void buildHashTable_str ();                     /// build hash table (string key)
    void printHashTable () const;                   /// print hash table
                                                    
    char getTargetOrReference () const;             /// getter of target or reference
    void setTargetOrReference (char);               /// setter of target or reference
    uint8_t getContextDepth () const;               /// getter of context depth
    void setContextDepth (uint8_t);                 /// setter of context depth
    uint16_t getAlphaDenom () const;                /// getter of alpha denominator
    void setAlphaDenom (uint16_t);                  /// setter of alpha denominator
    bool getInvertedRepeat () const;                /// getter of inverted repeat
    void setInvertedRepeat (bool);                  /// setter of inverted repeat
    uint64_t *getTable () const;                    /// getter of table
    void setTable (uint64_t *table);                /// setter of table
    const htable_t &getHashTable () const;          /// getter of hash table (int key)
    void setHashTable (const htable_t&);            /// setter of hash table (int key)
//    const htable_str_t &getHashTable_str () const;  /// getter of hash table (string key)
//    void setHashTable_str (const htable_str_t&);    /// setter of hash table (string key)
    const string &getTarFileAddress () const;       /// getter of target file address
    void setTarFileAddress (const string&);         /// setter of target file address
    const string &getRefFileAddress () const;       /// getter of reference file address
    void setRefFileAddress (const string&);         /// setter of reference file address

private:
    char     targetOrReference;                     /// target or reference
    uint8_t  contextDepth;                          /// context depth (SIZE <= 255)
    uint16_t alphaDenom;                            /// alpha denominator
    bool     invertedRepeat;                        /// inverted repeat
    uint64_t *table;                                /// table
    htable_t hashTable;                             /// hash table (int key)
//    htable_str_t hashTable_str;                     /// hash table (string key)
    string   tarFileAddress;                        /// target file address
    string   refFileAddress;                        /// reference file address
};


#endif //PHOENIX_FCM_H
