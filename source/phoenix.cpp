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


void print_permutations (char alphabet[], char prefix[], int ALPHABET_SIZE, int k)
{
    int count = 0;
    
    int i, j, prefixLength = strlen(prefix);
    char newprefix[ prefixLength + 2 ];
    
    if (k == 0)
    {
        std::cout << ++count << "\t" << prefix << "\n";
        return;
    }
    
    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        for (j = 0; j < prefixLength; j++)
            newprefix[ j ] = prefix[ j ];
        
        newprefix[ prefixLength ] = alphabet[ i ];
        newprefix[ prefixLength + 1 ] = '\0';
        
        print_permutations(alphabet, newprefix, ALPHABET_SIZE, k - 1);
    }
}




//int baseCharToNum(std::string s)
//{

//}
    



///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////

int32_t main (int argc, char *argv[])
{
//    Functions::commandLineParser(argc, argv);


/***********************************************************
    for test
************************************************************/
    int context_size = 2;
    char alphabet[5] = "ACTG";
    
//    print_permutations(alphabet, "", ALPHABET_SIZE, context_size);

    

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

    
    const int ALPHABET_SIZE = 4;  // alphabet = A, C, T, G
    const int CONTEXT_DEPTH = 2;
    const uint8_t ROW = 16;
    const uint8_t COL = 4;

    int table[ROW][COL];
    memset(table, 0, sizeof(table[ 0 ][ 0 ]) * ROW * COL);


    char DNAbase[4] = {'A', 'C', 'T', 'G'};

    std::string context (CONTEXT_DEPTH, '0');
    int index = 0;

    strDataset = context + strDataset;

    for (size_t i = CONTEXT_DEPTH; i < strDataset.size()-1; ++i)
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

//        index = baseCharToNum(context);

        std::cout << context << " " << index << "\n";
    }



    for (size_t i = 0; i < ROW; ++i)
    {
        std::cout << "[" << i << "]:\t";
        for (size_t j = 0; j < COL; ++j)
            std::cout << table[ i ][ j ] << "\t";

        std::cout << "\n";
    }

    
    return 0;
}