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
    std::ifstream myFile("a.fa", std::ios::in);
    
    if (!myFile)
    {
        std::cerr << "File could not be opened.\n";
        return 1;
    }
    
    std::string strEachLine;
    std::string strDataset;
    
    while (std::getline(myFile, strEachLine))
        strDataset += strEachLine;

//    std::cout << strDataset << std::endl;
    
    myFile.close();
    // file closed
    
    uint8_t CONTEXT_DEPTH = 2;
    uint8_t ROW = 16;
    uint8_t COL = 9;
    
    int table[ROW][COL];
    
    for (size_t i = 0; i < ROW; ++i)
        for (size_t j = 0; j < COL; ++j)
            table[ i ][ j ] = 0;
    
    
    return 0;
}