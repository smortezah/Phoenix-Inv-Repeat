#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>       /// time
#include <iomanip>      /// setw, setprecision
#include <cmath>
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
using std::setprecision;


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////

#include <vector>

int main (int argc, char *argv[])
{
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();  /// Record start time

    Functions function;                     /// object 'function' on memory stack
//    function.commandLineParser(argc, argv); /// parse the command line
    
    
//    std::vector<string> v;v.push_back("hi");v.push_back("bye");
//    for(std::vector<string>::iterator i=v.begin();i!=v.end();++i)
//        if (*i == 'i')
//            {
//                cout <<*i<<' ';
////                model.pushBackTarAddresses(tarFilesNames.substr(i + 1, tarIndex - it - 1));
////                tarIndex = it;
//            }
    
    
    
    
    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now(); /// Record end time
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime; /// calculate and show duration in seconds
    
    cout << "Elapsed time: " << std::fixed << setprecision(3) << elapsed.count() << '\n';
    
    
    return 0;
}
