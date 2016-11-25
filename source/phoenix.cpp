#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>               // dealing with time
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
#include "hash.h"



///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////

int32_t main (int argc, char *argv[])
{


/***********************************************************
    test
************************************************************/
//    std::ifstream myFile("a.fa", std::ios::in);   // open file
//    std::ifstream myFile("b.fa", std::ios::in);   // open file
    std::ifstream myFile("c.fa", std::ios::in);   // open file

    if (!myFile)
    {
        std::cerr << "File could not be opened.\n";
    }

    std::string strLine;    // keep each line

    while (!myFile.eof())
    {
        std::getline(myFile, strLine);

        std::cout << strLine << "\n";
    }

    if(myFile.eof())
        std::cout << "end\n";


    myFile.close(); // close file








//    // for calculating execution time
//    typedef std::chrono::high_resolution_clock highResClock;
//
//    // Record start time
//    highResClock::time_point exeStartTime = highResClock::now();
//
//
//
//
//    Functions function; // for access to Functions (object 'function' on memory stack)
//    function.commandLineParser(argc, argv);
//
//
//
//
//    // Record end time
//    highResClock::time_point exeFinishTime = highResClock::now();
//
//    // calculate and show duration in seconds
//    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
//    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    
    
    return 0;
}