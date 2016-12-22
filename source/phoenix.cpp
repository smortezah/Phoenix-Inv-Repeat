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

using std::cout;
using std::chrono::high_resolution_clock;


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////
#include <iomanip>      // setw, setprecision
#include <fstream>

int32_t main (int argc, char *argv[])
{
    // Record start time
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();

    // for access to Functions (object 'function' on memory stack)
    Functions function;
    function.commandLineParser(argc, argv); // parse the command line
    
    
//    const uint8_t currSymInt = (c == 'A') ? (uint8_t) 0 :
//                               (c == 'C') ? (uint8_t) 1 :
//                               (c == 'G') ? (uint8_t) 3 :
//                               (c == 'T') ? (uint8_t) 4 : (uint8_t) 2;
    
//    uint8_t currSymInt = (uint8_t) 1;
//    string context = "1234";
//    string inv = std::to_string(4 - currSymInt);
////    for (int i = context.size() - 1; i != 0; --i)  inv += std::to_string(52-context[ i ]);
//    for (string::iterator i = context.end() - 1; i != context.begin(); --i) inv += std::to_string(52 - *i);
//    cout << inv;
////    cout<<context[ 1 ]-48<<' '<<context[ 0 ]-48;
    
    
    // Record end time
    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now();

    // calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    cout
//            << '\n'
            << '\t'
//            << "Elapsed time: "
            << elapsed.count()
//            << " s"
            << '\n'
            ;
    
    
    return 0;
}
