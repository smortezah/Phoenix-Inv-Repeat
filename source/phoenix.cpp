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
#include "functions.h"
#include "FCM.h"
#include "messages.h"


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////
#include <bitset>
int32_t main (int argc, char *argv[])
{
    // Record start time
    highResClock::time_point exeStartTime = highResClock::now();
    

    // for access to Functions (object 'function' on memory stack)
    Functions function;
    function.commandLineParser(argc, argv); // parse the command line

    
//    std::bitset<2> x (0b01);
////    std::cout<<~x;
//    std::array< std::bitset< 2 >, 2 > arr = {0b01, 0b11};
//    std::cout<<arr[0b11];
    
//    std::cout<<(int) 'A'<<' '<<(int) 'C'<<' '<<(int) 'G'<<' '<<(int) 'T'<<' '<<(int) 'N';
    
    
    // Record end time
    highResClock::time_point exeFinishTime = highResClock::now();

    // calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    std::cout << "\nElapsed time: " << elapsed.count() << " s\n";

    
    return 0;
}
