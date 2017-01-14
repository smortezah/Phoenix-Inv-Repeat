#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>       /// time
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
#include <cmath>
int32_t main (int argc, char *argv[])
{
    /// Record start time
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();
    
    /// for access to Functions (object 'function' on memory stack)
    Functions function;
    function.commandLineParser(argc, argv); /// parse the command line
    
    
    


//    char arr[5] = {'A', 'T', 'C', 'G', 'N'};
//
//    for (int i = 1; i <= 4; ++i)
//    {
////        if (i % 5 != 0)
////        {
//        for (char c : arr)
//        {
//            cout << ((84 - c)*i) % 5<< '\t';
//        }
//        cout << '\n';
////        }
//    }
    
    
    
    
    
    
    
    /// Record end time
    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now();
    
    /// calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    
    cout << "   " << elapsed.count() << '\n';
    
    
    return 0;
}
