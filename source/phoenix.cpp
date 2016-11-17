#include <iostream>
#include <getopt.h>
#include <fstream>
#include <cstring>
#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#if defined(_MSC_VER)
#include <io.h>
#else
#include <unistd.h>
#endif

#include "def.h"
#include "messages.h"
#include "functions.h"




/***********************************************************
    for test
************************************************************/
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iterator>

const std::string alphabet{"ACTG"};
const uint8_t CONTEXT_DEPTH = 2;
const uint8_t COL = 4;  // number of columns of the table

static std::vector< std::string > permuteVector;


void permutation (std::string alphabet, const std::string& prefix,
                  const int alphabetSize, int iterCtxDepth)
{
    if (iterCtxDepth == 0)
    {
        permuteVector.push_back(prefix);
        return;
    }
    
    for (size_t i = 0; i < alphabetSize; ++i)
        permutation(alphabet, prefix + alphabet[ i ], alphabetSize, iterCtxDepth - 1);
}


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////

int32_t main (int argc, char *argv[])
{
//    Functions::commandLineParser(argc, argv);


/***********************************************************
    for test
************************************************************/

    // file opened
    std::ifstream myFile("c.fa", std::ios::in);

    if (!myFile)
    {
        std::cerr << "File could not be opened.\n";
        return 1;
    }

    std::string strEachLine;
    std::string strDataset;

    while (std::getline(myFile, strEachLine))
        strDataset += strEachLine;

    std::cout << strDataset << std::endl;

    myFile.close(); // file closed
    
    int alphabetSize = (int) alphabet.size();
    permutation(alphabet, "", alphabetSize, CONTEXT_DEPTH);
    for (std::string s : permuteVector)
        std::cout << s << "\n";
    
    
//    std::vector< std::string >::iterator location;
//    location = std::find( permuteVector.begin(), permuteVector.end(), "GG" );
//
//    if ( location != permuteVector.end() ) // found 16
//        std::cout << "\n\nFound 16 at location " << ( location - permuteVector.begin() ) << "\n";
//    else // 16 not found
//        std::cout << "\n\n16 not found";
    
    
    
    
    int ROW = pow(alphabetSize, CONTEXT_DEPTH);
    int table[ROW][COL];
    memset(table, 0, sizeof(table[ 0 ][ 0 ]) * ROW * COL);
    
    std::string context(CONTEXT_DEPTH, 'A');
    int index = 0;

    strDataset = context + strDataset;
    
    std::vector< std::string >::iterator indexIterator;
    
    for (size_t i = CONTEXT_DEPTH; i < strDataset.size(); ++i)
    {
        switch (strDataset[ i ])
        {
            case 'A':   table[ index ][ 0 ] += 1;   break;
            case 'C':   table[ index ][ 1 ] += 1;   break;
            case 'T':   table[ index ][ 2 ] += 1;   break;
            case 'G':   table[ index ][ 3 ] += 1;   break;
            default:    break;
        }

        context = strDataset.substr(i - CONTEXT_DEPTH + 1, CONTEXT_DEPTH);
    
        indexIterator = std::find( permuteVector.begin(), permuteVector.end(), context );
        index = indexIterator - permuteVector.begin();

        std::cout << context << " " << index << "\n";
    }



    for (size_t i = 0; i < ROW; ++i)
    {
        std::cout << "[" << i << "]:\t";
        for (size_t j = 0; j < COL; ++j)
            std::cout << table[ i ][ j ] << "\t";
        std::cout << "\n";
    }
    std::cout << "*************************************************\n"
              << "*************************************************\n";

    
    return 0;
}