#ifndef PHOENIX_FCM_H
#define PHOENIX_FCM_H


#include <iostream>
#include <vector>
#include <mutex>

#include "def.h"

using std::string;
using std::vector;


class FCM
{
public:
    FCM ();                                          /// constructor
    
    void   buildModel ();                            /// build reference(s) model
    inline void updateTable (U64, U64);              /// update table, including 'sum' column
    
    void   compressTarget (string);                  /// compress target file
    inline U8 symCharToInt (char) const;             /// ACNGT -> 01234
                                                     
//    void buildHashTable_str ();                             /// build hash table (string key)
    void     printHashTable () const;                /// print hash table
                                                     
    char   getCompressionMode () const;              /// getter of compression mode
    void   setCompressionMode (char);                /// setter of compression mode
    U8     getContextDepth () const;                 /// getter of context depth
    void   setContextDepth (U8);                     /// setter of context depth
    U16    getAlphaDenom () const;                   /// getter of alpha denominator
    void   setAlphaDenom (U16);                      /// setter of alpha denominator
    bool   getInvertedRepeat () const;               /// getter of inverted repeat
    void   setInvertedRepeat (bool);                 /// setter of inverted repeat
    U64    *getTable () const;                       /// getter of table
    void   setTable (U64 *table);                    /// setter of table
    const  htable_t &getHashTable () const;          /// getter of hash table (int key)
    void   setHashTable (const htable_t&);           /// setter of hash table (int key)
//    const htable_str_t &getHashTable_str () const;     /// getter of hash table (string key)
//    void setHashTable_str (const htable_str_t&);       /// setter of hash table (string key)
    const  vector<string> &getTarAddresses () const; /// getter of target files addresses
    void   pushBackTarAddresses (string);            /// pushBacker of target files addresses
    const  vector<string> &getRefAddresses () const; /// getter of reference files addresses
    void   pushBackRefAddresses (string);            /// pushBacker of reference files addresses
    
private:
    std::mutex mut;                                  /// mutex
    
    char     compressionMode;                        /// compression mode (table / hash table)
    U8       contextDepth;                           /// context depth (SIZE <= 255)
    U16      alphaDenom;                             /// alpha denominator
    bool     invertedRepeat;                         /// inverted repeat
    U64      *table;                                 /// table
    htable_t hashTable;                              /// hash table (int key)
//    htable_str_t hashTable_str;                             /// hash table (string key)
    vector<string> tarAddresses;                     /// target files addresses
    vector<string> refAddresses;                     /// reference files addresses
};


#endif //PHOENIX_FCM_H