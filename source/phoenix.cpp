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
#include <vector>

int32_t main (int argc, char *argv[])
{
    // Record start time
    highResClock::time_point exeStartTime = highResClock::now();
    

//    // for access to Functions (object 'function' on memory stack)
//    Functions function;
//    function.commandLineParser(argc, argv); // parse the command line
    
    
    
    
    
    
    
    std::vector< int > v(4, 0);
//    std::vector< int > v2;
//    v2.reserve(1);
//    v2.push_back(1);
//    v.insert(v.end(), v2.begin(), v2.end());
//    std::array<int,1> a;
//
//    std::string s="ACT";
//    std::cout<<"\n"<<s.size();
//
    std::array<int, 2> a2 ={1,2};
//    std::array<int, 1> a3={2};
    
    v.insert(v.end(), a2.begin(), a2.end());
    
    
    
    for(int i:v)
        std::cout<<i<<' '
//                 << sizeof(v2) <<' '
//                 << sizeof(a)<<' '
                ;
    
    
    
    // Record end time
    highResClock::time_point exeFinishTime = highResClock::now();

    // calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    std::cout << "\nElapsed time: " << elapsed.count() << " s\n";

    
    return 0;
}
