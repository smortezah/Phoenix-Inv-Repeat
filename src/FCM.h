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
    void   initTables     ();                           /// initialize vector of tables
    void   initHashTables ();                           /// initialize vector of hash tables
    
    void   buildModel          (bool, U8, U8);          /// build reference(s) model
    void   compressTarget      (const string&);         /// compress target file
    void   decompressTarget    (const string&);         /// decompress target file
    
    inline char   symIntToChar (U8)     const;          /// 01234 -> ACNGT
    inline U8     symCharToInt (char)   const;          /// ACNGT -> 01234
    inline double fastPow      (double, double);        /// fast power
    
//    void buildHashTable_str ();                         /// build hash table (string key)
    void   printHashTable ()             const;         /// print hash table
    
    bool   getDecompressFlag   ()        const;         /// getter - decompress flag
    void   setDecompressFlag   (bool);                  /// getter - decompress flag
    U8     getN_threads        ()        const;         /// getter - number of threads
    void   setN_threads        (U8);                    /// setter - number of threads
    void   setCompressionMode  (char);                  /// setter - compression mode
    char   getCompressionMode  ()        const;         /// getter - compression mode
    void   setN_models         (U8);                    /// setter - number of models
    U8     getN_models         ()        const;         /// getter - number of models
    void   setGamma            (double);                /// setter - gamma
    double getGamma            ()        const;         /// getter - gamma
    const  vector<bool>   &getInvertedRepeats () const; /// getter - inverted repeat(s)
    const  vector<U8>     &getContextDepths   () const; /// getter - context depth(es)
    void   pushBackParams (bool, U8, U16);              /// setter - model(s) params (ir, ctx_depth, alpha_denom)
    const  vector<string> &getTarAddr () const; /// getter - target files addresses
    void   pushBackTarAddresses (const string&);        /// pushBacker - target files addresses
    const  vector<string> &getRefAddresses    () const; /// getter - reference files addresses
    void   pushBackRefAddresses (const string&);        /// pushBacker - reference files addresses
    void   setTable       (U64*, U8);                   /// setter - table(s)
    void   setHashTable   (const htable_t&, U8);        /// setter - hash table(s)
    
private:
    std::mutex       mut;                               /// mutex
    
    U8               n_threads;                         /// number of threads
    bool             decompressFlag;                    /// decompress flag
    
    char             compressionMode;                   /// compression mode (table / hash table)
    U8               n_models;                          /// number of models
    double           gamma;                             /// gamma (for mixture of FCMs)
    vector<bool>     invertedRepeats;                   /// inverted repeat(s)
    vector<U8>       contextDepths;                     /// context depthe(s) (SIZE <= 255)
    vector<U16>      alphaDenoms;                       /// alpha denominator(s)
    vector<string>   tarAddresses;                      /// target files addresses
    vector<string>   refAddresses;                      /// reference files addresses
    vector<U64*>     tables;                            /// table(s)
    vector<htable_t> hashTables;                        /// hash table(s)
//    htable_str_t   hashTable_str;                       /// hash table (string key)
};


#endif //PHOENIX_FCM_H
