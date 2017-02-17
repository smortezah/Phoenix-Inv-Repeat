#ifndef PHOENIX_DEF_H
#define PHOENIX_DEF_H


#include <unordered_map>    /// hash table

using std::string;
using std::unordered_map;
using std::array;


/***********************************************************
    About PHOENIX
************************************************************/
#define VERSION_PHOENIX 1
#define RELEASE_PHOENIX 0


/***********************************************************
    constants
************************************************************/
#define ALPHABET_SIZE       5           /// alphabet {A, C, N, G, T}
#define ALPH_SUM_SIZE       6           /// ALPHABET_SIZE+1: one more column for 'sum'
#define LOG2_ALPHABET_SIZE  2.321928095 /// log2 of 5 (ALPHABET_SIZE)
/// max context depth for building table. 12 is possible but not cost effective
#define TABLE_MAX_CONTEXT   11

#define DEFAULT_N_THREADS   4
#define N_FREE_THREADS      2


/***********************************************************
    typedef
************************************************************/
/// vaghti bishtar az 65,535 (uint16_t max) shod, hameye adad ha nesf mishan.
/// ye variable ro ham tanzim kon ke maloom she chand bar nesf kardim
//typedef unordered_map< string, array< uint64_t, ALPHABET_SIZE > > htable_str_t;
typedef unordered_map< uint64_t , array< uint64_t, ALPHABET_SIZE > > htable_t;


#endif //PHOENIX_DEF_H