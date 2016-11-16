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

int baseCharToNum(std::string s)
{
    std::string intStr(s);
    
    for (size_t i = 0; i < s.size(); ++i)
        switch (s[ i ])
        {
            case 'A':   intStr[ i ] = '0';    break;
            case 'C':   intStr[ i ] = '1';    break;
            case 'T':   intStr[ i ] = '2';    break;
            case 'G':   intStr[ i ] = '3';    break;
            default:    break;
        }
    
    return std::stoi(intStr);
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

        index = baseCharToNum(context);
        
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