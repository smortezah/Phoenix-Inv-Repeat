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

#include "bitio.h"
#include "arith.h"
#include "arith_aux.h"


const std::string alphabet{"ACTG"};
const uint8_t CONTEXT_DEPTH = 2;
const uint8_t COL = 4;  // number of columns of the table

static std::vector< std::string > permuteVector;

// produces all permutations of the alphabet with the CONTEXT_SIZE size
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
    std::ifstream myFile("b.fa", std::ios::in);
    
    if (!myFile)
    {
        std::cerr << "File could not be opened.\n";
        return 1;
    }
    
    std::string strEachLine;
    std::string strDataset;
    
    while (std::getline(myFile, strEachLine))
        strDataset += strEachLine;
    
    // show the dataset
    std::cout << "Dataset:\n" << strDataset << std::endl;
    
    myFile.close();
    // file closed
    
    // all permutations of the alphabet with the CONTEXT_SIZE size
    int alphabetSize = (int) alphabet.size();
    permutation(alphabet, "", alphabetSize, CONTEXT_DEPTH);
//    for (std::string s : permuteVector)
//        std::cout << s << "\n";
    
    // a table to save the number of occurrences
    int ROW = pow(alphabetSize, CONTEXT_DEPTH);
    int table[ROW][COL];
    memset(table, 0, sizeof(table[ 0 ][ 0 ]) * ROW * COL);
    
    // context, that slides in the dataset
    std::string context(CONTEXT_DEPTH, 'A');
    int index = 0;
    
    // add "AA..." (with the size of CONTEXT_DEPTH) to the beginning of the dataset
    strDataset = context + strDataset;
    
    // an iterator for iterating in the permuteVector
    std::vector< std::string >::iterator indexIterator;
    
    // add 1 to the number of occurrences if a specific symbol is seen
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
    
        // slides in the dataset
        context = strDataset.substr(i - CONTEXT_DEPTH + 1, CONTEXT_DEPTH);
        
        // index of the context
        indexIterator = std::find(permuteVector.begin(), permuteVector.end(), context);
        index = indexIterator - permuteVector.begin();
    }
    
    // show the table
    for (size_t i = 0; i < ROW; ++i)
    {
        std::cout << "[" << i << "]:\t";
        for (size_t j = 0; j < COL; ++j)
            std::cout << table[ i ][ j ] << "\t";
        std::cout << "\n";
    }
    std::cout << "*************************************************\n";
    
    
    return 0;
}