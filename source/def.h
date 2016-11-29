#ifndef PHOENIX_DEF_H
#define PHOENIX_DEF_H


#include <chrono>           // time
#include <unordered_map>    // hash table


/***********************************************************
    About PHOENIX
************************************************************/
#define VERSION_PHOENIX 1
#define RELEASE_PHOENIX 0


/***********************************************************
    constants
************************************************************/
#define ALPHABET_SIZE   5     // alphabet {A, C, T, G, N}
#define CONTEXT_DEPTH   2


//#define ALPHA_NUMERATOR     1
//#define ALPHA_DENUMERATOR   1
//#define COL                 4   // number of columns of the table


/***********************************************************
    typedef
************************************************************/
// for calculating execution time
typedef std::chrono::high_resolution_clock highResClock;

// TODO
// vaghti bishtar az 65,535 (uint16_t max) shod, hameye adad ha nesf mishan.
// ye variable ro ham tanzim kon ke maloom she chand bar nesf kardim
typedef std::unordered_map< std::string, std::array<uint16_t, ALPHABET_SIZE> > hashTable_t;



#endif //PHOENIX_DEF_H
