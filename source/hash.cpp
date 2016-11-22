#include "hash.h"

#include <unordered_map>
#include <iostream>



const uint8_t CONTEXT_DEPTH = 2;
//const uint8_t ALPHA_NUMERATOR = 1;
//const uint8_t ALPHA_DENUMERATOR = 1;
//const uint8_t COL = 4;  // number of columns of the table


//#include <array>
//#include <vector>
//#include <algorithm>
//#include <cmath>
//#include <iterator>

//#include "bitio.h"
//#include "arith.h"
//#include "arith_aux.h"


/***********************************************************
    constructor
************************************************************/
Hash::Hash () {}


/***********************************************************
    build hash table
************************************************************/
void Hash::hashTableBuild (std::string strDataset)
{
    std::unordered_map< std::string, std::array< int, 4> > hashTable;

    // context, that slides in the dataset
    std::string context(CONTEXT_DEPTH, 'A');

    // add "AA..." (with the size of CONTEXT_DEPTH) to the beginning of the dataset
    strDataset = context + strDataset;


//    int counters[4];
//    memset(counters, 0, sizeof(counters[ 0 ]) * 4);
//    int totalCount = 0;
//    int symbol = 0;
//    FILE *writer = fopen("mori", "w");


    hashTable.insert( {context, {0, 0, 0, 0}} );

    // fill hash table by number of occurrences of symbols A, C, T, G
    for (size_t i = CONTEXT_DEPTH; i < strDataset.size(); ++i)
    {
        switch (strDataset[ i ])
        {
            case 'A':
                // incrementing number of 'A's
                ++(hashTable[ context ])[ 0 ];  // order: {A, C, T, G}


//                counters[ 0 ] += ALPHA_DENUMERATOR * table[ index ][ 0 ] + ALPHA_NUMERATOR;
//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
////                AESym(3, counters, totalCount, writer);
                break;

            case 'C':
                // incrementing number of 'C's
                ++(hashTable[ context ])[ 1 ];  // order: {A, C, T, G}


//                counters[ 1 ] += ALPHA_DENUMERATOR * table[ index ][ 1 ] + ALPHA_NUMERATOR;
//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
////                AESym(3, counters, totalCount, writer);
                break;

            case 'T':
                // incrementing number of 'T's
                ++(hashTable[ context ])[ 2 ];  // order: {A, C, T, G}


//                counters[ 2 ] += ALPHA_DENUMERATOR * table[ index ][ 2 ] + ALPHA_NUMERATOR;
//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
////                AESym(3, counters, totalCount, writer);
                break;

            case 'G':
                // incrementing number of 'G's
                ++(hashTable[ context ])[ 3 ];  // order: {A, C, T, G}


//                counters[ 3 ] += ALPHA_DENUMERATOR * table[ index ][ 3 ] + ALPHA_NUMERATOR;
//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
////                AESym(3, counters, totalCount, writer);
                break;

            default:
                break;
        }

//        totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                           table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                     (4 * ALPHA_NUMERATOR);
//
//        memset(counters, 0, sizeof(counters[ 0 ]) * 4);

        // slides in the dataset
        context = strDataset.substr(i - CONTEXT_DEPTH + 1, CONTEXT_DEPTH);
    }
    
    /* show hash table */
    // type definition: unordered_map iterator
    typedef std::unordered_map< std::string, std::array< int, 4 > >::iterator umit;

    int ind = 1;  // for test
    std::cout << "\t\tA\tC\tT\tG\n"
              << "        --------------------------------------\n";
    for (umit it = hashTable.begin(); it != hashTable.end(); ++it)
    {
        std::cout << ind << ":\t" << it->first << "\t";
        for (int i : it->second)    std::cout << i << "\t";
        std::cout << "\n";
        ++ind;
    }
    std::cout << "\nbucket size = " << hashTable.bucket_count() << "\n\n";  // for test
}