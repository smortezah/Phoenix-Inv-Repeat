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
#include <algorithm>

int32_t main (int argc, char *argv[])
{
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();  /// Record start time

    /// for access to Functions (object 'function' on memory stack)
    Functions function;
//    function.commandLineParser(argc, argv); /// parse the command line
    
    
    
    int tableSize=30;
//    double *table = new double[ tableSize ];                        /// already initialized with 0's
//    uint64_t *table = new uint64_t[ tableSize ];                        /// already initialized with 0's
    int table[ tableSize ];                        /// already initialized with 0's
//            /*
    /// initialize table with 0's
//    memset(table, 1, sizeof(table[ 0 ]) * tableSize);
            std::fill(table,table+30,1);
//            */

//            cout<<tableSize;
    for (int j = 0; j < tableSize; ++j)
    {
//        table[tableSize]=1;
        
        cout<<table[tableSize]<<' ';
    }
    
    
    
    
    
    
    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now(); /// Record end time
    
    /// calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    
//    cout << "Elapsed time: " << std::fixed << setprecision(3) << elapsed.count() << '\n';
    
    
    return 0;
}
