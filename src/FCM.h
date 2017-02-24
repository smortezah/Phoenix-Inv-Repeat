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
    FCM ();                                             /// constructor
    
    void   buildModel ();                               /// build reference(s) model
    void   compressTarget (string);                     /// compress target file
    inline U8   symCharToInt (char) const;              /// ACNGT -> 01234
    inline void updateTable (U64, U64);                 /// update table, including 'sum' column
                                                        
//    void buildHashTable_str ();                                /// build hash table (string key)
    void   printHashTable () const;                     /// print hash table
    
    void   setGamma (double gamma);                     /// setter of gamma
    void   pushBackParams (bool, U8, U16);              /// setter of model(s) parameters (ir, ctx_depth, alpha_denom)
    const  vector<string> &getTarAddresses () const;    /// getter of target files addresses
    void   pushBackTarAddresses (const string&);        /// pushBacker of target files addresses
    const  vector<string> &getRefAddresses () const;    /// getter of reference files addresses
    void   pushBackRefAddresses (const string&);        /// pushBacker of reference files addresses
    
private:
    double         gamma;                               /// gamma (for mixture of FCMs)
    vector<bool>   invertedRepeats;                     /// inverted repeat(s)
    vector<U8>     contextDepths;                       /// context depthe(s) (SIZE <= 255)
    vector<U16>    alphaDenoms;                         /// alpha denominator(s)
    vector<string> tarAddresses;                        /// target files addresses
    vector<string> refAddresses;                        /// reference files addresses
    
    std::mutex     mut;                                 /// mutex
    char           compressionMode;                     /// compression mode (table / hash table)
    U64            *table;                              /// table
    htable_t       hashTable;                           /// hash table (int key)
//    htable_str_t hashTable_str;                                /// hash table (string key)
};


#endif //PHOENIX_FCM_H