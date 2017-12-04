#ifndef PHOENIXIR_FCM_H
#define PHOENIXIR_FCM_H

#include <iostream>
#include <vector>
#include <mutex>

#include "def.h"

using std::string;
using std::vector;
using std::chrono::high_resolution_clock;


class FCM
{
public:
    FCM ();                                // constructor
    void   initTables          ();         // initialize vector of tables
    void   initHashTables      ();         // initialize vector of hash tables
                                                       
    void   buildModel          (const vector<string>&,
                                bool, U8, U16);     // build reference(s) model
    void   compress            (const string&);     // compress target file
    void   extractHeader       (const string&);     // header inf. for decomp.
    void   decompress          (const string&);     // decompress target file
    
    inline char   symIntToChar (U8)        const;   // 01234 -> ACNGT
    inline U8     symCharToInt (char)      const;   // ACNGT -> 01234
    inline double fastPow      (double, double);    // fast power
    inline U64    fileSize     (const string&);     // size of file
    inline U64    countSymbols (const string&);     // count no. syms in a file
           
//    void buildHashTable_str ();             // build hash table (string key)
    inline void   printHashTable (U8)      const;   // print hash table
    
    // getters and setters
    const vector<bool>&   getIR         () const;   // get inverted repeat(s)
    const vector<U8>&     getCtxDepth   () const;   // get context depth(es)
    const vector<string>& getTarAddr    () const;   // get tar. files addresses
    const vector<string>& getRefAddr    () const;   // get ref. files addresses
    U64**                 getTables     () const;  // get table(s)
//    U16**                 getTables     () const;   // get table(s)
    htable_t*             getHashTables () const;   // get hash table(s)
    bool  getDecompFlag                 () const;   // get decompress flag
    U16   getN_threads                  () const;   // get number of threads
    U16   getN_models                   () const;   // get number of models
//    U32   getN_div                      () const; // get number of divisions
    void  setStartTime    (const high_resolution_clock::time_point&);
    void  setDecompFlag   (bool);                   // set decompress flag
    void  setN_threads    (U16);                    // set number of threads
    void  setCompMode     (char);                   // set compression mode
    void  setN_models     (U16);                    // set number of models
    void  setGamma        (double);                 // set gamma
//    void  setN_div        (U32);                   // set number of divisions
    void  pushTarAddr     (const string&);          // push back tars. addr.
    void  pushRefAddr     (const string&);          // push back refs. addr.
    void  setTable        (U64*, U8);               // set table(s)
//    void  setTable        (U16*, U8);               // set table(s)
    void  setHashTable    (const htable_t&, U8);    // set hash table(s)
    // push back model(s) params (ir, ctx_depth, alpha_denom)
    void  pushParams      (bool, U8, U16);
   
   
//   double getGamma ()    { return gamma; }
//   vector<U16> getAlpha ()    { return alphaDens; }
//
//   char getCompmode ()    { return compMode; }


private:
    std::mutex     mut;                // mutex
    high_resolution_clock::time_point startTime;   // start time
    
    U16            n_threads;          // number of threads
    bool           decompFlag;         // decompress flag
    
    char           compMode;           // compression mode (table / hash table)
    U16            n_models;           // number of models
    double         gamma;              // gamma (for mixture of FCMs)
    vector<bool>   invRepeats;         // inverted repeat(s)
    vector<U8>     ctxDepths;          // context depthe(s) (SIZE <= 255)
    vector<U16>    alphaDens;          // alpha denominator(s)
//    U32            n_div;              // no. divisions done for no. syms
    vector<string> tarAddr;            // target files addresses
    vector<string> refAddr;            // reference files addresses
    U64**          tables;             // table(s)
//    U16**          tables;             // table(s)
    htable_t*      hashTables;         // hash table(s)
//    htable_str_t hashTable_str;      // hash table (string key)
};

#endif //PHOENIXIR_FCM_H