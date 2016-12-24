#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>       // time
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
#include <vector>
int32_t main (int argc, char *argv[])
{
    // Record start time
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();

    // for access to Functions (object 'function' on memory stack)
    Functions function;
    function.commandLineParser(argc, argv); // parse the command line
    
    
//    htable_t hTable;
//    string context(2, '0');
//    hTable.insert({context, {0, 0, 0, 0, 0}});
//    ++hTable[ context ][ 'A' ];
//    cout<<hTable[ context ][ 65 ];
    
//    std::vector< int> v;
//    v.push_back(std::make_pair("q", 3));
////    for(std::vector< int>::iterator i=v.begin();i!=v.end();++i)
////        cout<<*i;
    
    
    
    
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
