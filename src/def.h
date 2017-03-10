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
#define ALPH_SIZE         5           /// alphabet {A, C, N, G, T}
#define ALPH_SUM_SIZE     6           /// ALPHABET_SIZE+1: one more column for 'sum'
#define LOG2_ALPH_SIZE    2.321928095 /// log2 of 5 (ALPHABET_SIZE)
#define TABLE_MAX_CTX     12          /// max context depth for building table
#define DEFAULT_N_THREADS 2           /// default number of threads
#define N_FREE_THREADS    1
#define DEFAULT_GAMMA     0.95        /// default gamma (for mixture of FCMs)

#define WATERMARK         20170308    /// used in compression/decompression
#define BUFFER_SIZE       262144      /// used in decompression
#define DOUBLE_TO_INT     65535       /// for converting double to integer


/***********************************************************
    typedefs
************************************************************/
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef int8_t   I8;
typedef int16_t  I16;
typedef int32_t  I32;
typedef int64_t  I64;

/// vaghti bishtar az 65,535 (uint16_t max) shod, hameye adad ha nesf mishan.
/// ye variable ro ham tanzim kon ke maloom she chand bar nesf kardim
//typedef unordered_map< string, array< U64, ALPH_SIZE > > htable_str_t;
typedef unordered_map< U64 , array< U64, ALPH_SIZE > > htable_t;
//typedef unordered_map< U64 , U64[ALPH_SIZE] > htable_t;


/***********************************************************
    lookup tables
************************************************************/
static U64 POWER5[] =
        {
                1,  5,  25, 125,    625,    3125,   15625,
                78125,  390625
        };


#endif //PHOENIX_DEF_H