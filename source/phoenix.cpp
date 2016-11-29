#include <iostream>
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
#include "hash.h"
#include "FCM.h"



///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////

int32_t main (int argc, char *argv[])
{
    // Record start time
    highResClock::time_point exeStartTime = highResClock::now();
    
    
    
    
//    // for access to Functions (object 'function' on memory stack)
//    Functions function;
//    function.commandLineParser(argc, argv); // parse the command line
    
    
    uint8_t contextDepth=2;                       // context depth
    uint32_t alphaDenom=1;                        // alpha denominator
    Hash h;
    std::string a ("c.fa");
    hashTable_t hashTable = h.hashTableBuild(a, false);
    std::string fileAddress=a;                    // file address
    
    FCM f (contextDepth, alphaDenom, hashTable, fileAddress);
    std::cout<< f.getContextDepth();
    
    
    
    
    // Record end time
    highResClock::time_point exeFinishTime = highResClock::now();

    // calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    std::cout << "\nElapsed time: " << elapsed.count() << " s\n";

    
    return 0;
}
