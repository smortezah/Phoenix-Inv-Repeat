#ifndef PHOENIX_DEF_H
#define PHOENIX_DEF_H


#include <unordered_map>    /// hash table

using std::string;
using std::unordered_map;
using std::array;

/********************************************************************************************
    About PHOENIX
********************************************************************************************/
#define VERSION_PHOENIX 1
#define RELEASE_PHOENIX 0


/********************************************************************************************
    constants
********************************************************************************************/
#define ALPH_SIZE         5               /// alphabet {A, C, N, G, T} size
#define ALPH_SUM_SIZE     6               /// ALPHABET_SIZE+1: one more column for 'sum'
#define LOG2_ALPH_SIZE    2.321928095     /// log2 of 5 (ALPHABET_SIZE)
#define TABLE_MAX_CTX     12              /// max context depth for building table
//#define TABLE_MAX_CTX     0              /// max context depth for building table


#define DEFAULT_N_THREADS 2               /// default number of threads
#define N_FREE_THREADS    1
#define DEFAULT_GAMMA     0.95            /// default gamma (for mixture of FCMs)

#define COMP_FILETYPE     ".co"           /// compressed file type
#define DECOMP_FILETYPE   ".de"           /// decompressed file type
#define WATERMARK         20170324        /// used in compression/decompression
#define BUFFER_SIZE       262144          /// used in decompression

#define DOUBLE_TO_INT     65535           /// for converting double to integer
#define MAX_INT           2147483647      /// maximum possible integer (32 bit)
/// max number of bases allowed for each cell of table = (MAX_INT-1)/65535 = 2^15
#define MAX_NO_BASE       32768


/********************************************************************************************
    typedefs
********************************************************************************************/
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


/********************************************************************************************
    lookup tables
********************************************************************************************/
static const U64 POWER5[28] =             /// 5^0 to 5^27, which needs less than 64 bits
        {
                                  1,                  5,                  25,
                                125,                625,                3125,
                              15625,              78125,              390625,
                            1953125,            9765625,            48828125,
                          244140625,         1220703125,          6103515625,
                        30517578125,       152587890625,        762939453125,
                      3814697265625,     19073486328125,      95367431640625,
                    476837158203125,   2384185791015625,   11920928955078100,
                  59604644775390600, 298023223876953000, 1490116119384770000,
                7450580596923830000
        };


#endif //PHOENIX_DEF_H