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
    
    void   buildModel     (const vector<string>&,
                           bool, U8, U8);           /// build reference(s) model
    void   compress       (const string&);          /// compress target file
    void   extractHeader  (const string &);         /// extract header information for decompression
    void   decompress     (const string&,
                           const vector<string>&);  /// decompress target file
    
    inline char   symIntToChar (U8)       const;    /// 01234 -> ACNGT
    inline U8     symCharToInt (char)     const;    /// ACNGT -> 01234
    inline double fastPow      (double, double);    /// fast power
    inline U64    fileSize     (const string&);     /// size of file
    
//    void buildHashTable_str ();                      /// build hash table (string key)
    void   printHashTable (U8)      const;    /// print hash table
    
    bool   getDecompFlag  ()              const;    /// get decompress flag
    void   setDecompFlag  (bool);                   /// set decompress flag
    U8     getN_threads   ()              const;    /// get number of threads
    void   setN_threads   (U8);                     /// set number of threads
    void   setCompMode    (char);                   /// set compression mode
    void   setN_models    (U8);                     /// set number of models
    U8     getN_models    ()              const;    /// get number of models
    void   setGamma       (double);                 /// set gamma
    const  vector<bool>   &getIR       () const;    /// get inverted repeat(s)
    const  vector<U8>     &getCtxDepth () const;    /// get context depth(es)
    void   pushParams     (bool, U8, U16);          /// push back model(s) params (ir, ctx_depth, alpha_denom)
    const  vector<string> &getTarAddr  () const;    /// get target files addresses
    void   pushTarAddr    (const string&);          /// push back target files addresses
    const  vector<string> &getRefAddr  () const;    /// get reference files addresses
    void   pushRefAddr    (const string&);          /// push back reference files addresses
    void   setTable       (U64*, U8);               /// set table(s)
    void   setHashTable   (const htable_t&, U8);    /// set hash table(s)
    
    
    
    const  vector<U64*> &getTables ()  const  { return this->tables;}
    const  vector<htable_t> &getHashTables ()  const  { return this->hashTables;}
    
    double getGamma ()    { return gamma;}
    vector<U16> getAlpha ()    { return alphaDens;}
    
    char getCompmode ()    { return compMode;}
    
    
    
    
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
