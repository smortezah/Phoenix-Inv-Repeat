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
    FCM ();                                         /// constructor
    void   initTables     ();                       /// initialize vector of tables
    void   initHashTables ();                       /// initialize vector of hash tables
    
    void   buildModel     (bool, U8, U8);           /// build reference(s) model
    void   compress       (const string &);         /// compress target file
    void   decompress     (const string &);         /// decompress target file
    
    inline char   symIntToChar (U8)       const;    /// 01234 -> ACNGT
    inline U8     symCharToInt (char)     const;    /// ACNGT -> 01234
    inline double fastPow      (double, double);    /// fast power
    
//    void buildHashTable_str ();                      /// build hash table (string key)
    void   printHashTable ()              const;    /// print hash table
                                          
    bool   getDecompFlag  ()              const;    /// getter - decompress flag
    void   setDecompFlag  (bool);                   /// getter - decompress flag
    U8     getN_threads   ()              const;    /// getter - number of threads
    void   setN_threads   (U8);                     /// setter - number of threads
    void   setCompMode    (char);                   /// setter - compression mode
    void   setN_models    (U8);                     /// setter - number of models
    U8     getN_models    ()              const;    /// getter - number of models
    void   setGamma       (double);                 /// setter - gamma
    const  vector<bool>   &getIR       () const;    /// getter - inverted repeat(s)
    const  vector<U8>     &getCtxDepth () const;    /// getter - context depth(es)
    void   pushParams     (bool, U8, U16);          /// setter - model(s) params (ir, ctx_depth, alpha_denom)
    const  vector<string> &getTarAddr  () const;    /// getter - target files addresses
    void   pushTarAddr    (const string &);         /// pushBacker - target files addresses
    const  vector<string> &getRefAddr  () const;    /// getter - reference files addresses
    void   pushRefAddr    (const string &);         /// pushBacker - reference files addresses
    void   setTable       (U64*, U8);               /// setter - table(s)
    void   setHashTable   (const htable_t&, U8);    /// setter - hash table(s)
    
private:
    std::mutex       mut;                           /// mutex
    
    U8               n_threads;                     /// number of threads
    bool             decompFlag;                    /// decompress flag
    
    char             compMode;                      /// compression mode (table / hash table)
    U8               n_models;                      /// number of models
    double           gamma;                         /// gamma (for mixture of FCMs)
    vector<bool>     invRepeats;                    /// inverted repeat(s)
    vector<U8>       ctxDepths;                     /// context depthe(s) (SIZE <= 255)
    vector<U16>      alphaDens;                     /// alpha denominator(s)
    vector<string>   tarAddr;                       /// target files addresses
    vector<string>   refAddr;                       /// reference files addresses
    vector<U64*>     tables;                        /// table(s)
    vector<htable_t> hashTables;                    /// hash table(s)
//    htable_str_t   hashTable_str;                   /// hash table (string key)
};


#endif //PHOENIX_FCM_H
