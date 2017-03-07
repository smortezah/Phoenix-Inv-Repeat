#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>       /// time
#include <iomanip>      /// setw, setprecision
#include <thread>
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
#include "FCM.h"

using std::cout;
using std::chrono::high_resolution_clock;
using std::setprecision;
using std::thread;


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////
int main (int argc, char *argv[])
{
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();  /// Record start time
    
    /// objects on memory stack
//    Functions funcObj;
    
    FCM       mixModel;
    
    /// parse the command line
    commandLineParser(argc, argv, mixModel);
//    funcObj.commandLineParser(argc, argv, mixModel);
    
    const U8 n_models = mixModel.getN_models();
    const U8 n_threads = mixModel.getN_threads();
    
    
//    /// build reference(s) model(s) -- multithreaded
////    /// set compression mode: 't'=table, 'h'=hash table -- 5^k_1 + 5^k_2 + ... > 5^12 ==> mode: hash table
////    U64 cmpModeSum = 0;
////    for (U8 k : mixModel.getContextDepths())    cmpModeSum = cmpModeSum + (U64) pow(ALPH_SIZE, k);
////    const char compressionMode = (cmpModeSum > pow(ALPH_SIZE, TABLE_MAX_CTX)) ? 'h' : 't';
////    mixModel.setCompressionMode( compressionMode );
////
////    /// initialize vector of tables or hash tables
////    compressionMode == 'h' ? mixModel.initHashTables() : mixModel.initTables();
//
//
//
////    /// initialize vector of tables or hash tables
////    mixModel.getCompressionMode() == 'h' ? mixModel.initHashTables() : mixModel.initTables();
//
//
//    U8 arrThrSize = (n_models > n_threads) ? n_threads : n_models;/// size of array of threads
//    thread arrThread[ arrThrSize ];                               /// array of threads
//
//    for (U8 i = 0; i < n_models; i += arrThrSize)
//    {
//        for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
//            arrThread[ j ] = thread( &FCM::buildModel, &mixModel,
//                                     mixModel.getInvertedRepeats()[ i + j ],
//                                     mixModel.getContextDepths()[ i + j ],
//                                     i + j );
//
//        for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
//            arrThread[ j ].join();
//    }
    /*
    /// compress target(s) using reference(s) model -- multithreaded
    U8 MAX_N_THREADS = (U8) thread::hardware_concurrency(); /// max cores in current machine
    /// N_FREE_THREADS considered for other jobs in current system
    U8 n_threads_available = (U8) (!MAX_N_THREADS ? DEFAULT_N_THREADS - N_FREE_THREADS
                                                            : MAX_N_THREADS - N_FREE_THREADS);
    U8 n_targets = (U8) model.getTarAddresses().size();     /// up to 2^8=256 targets

    U8 arrThrSize = (n_targets > n_threads_available) ? n_threads_available : n_targets;
    thread *arrThread = new thread[arrThrSize];             /// array of threads
    */
    
//    /// compress target(s) using reference(s) model(s) -- multithreaded
////    mixModel.setGamma(gamma);                                     /// set gamma
//    U8 n_targets = (U8) mixModel.getTarAddresses().size();        /// up to 2^8=256 targets
//    arrThrSize = (n_targets > n_threads) ? n_threads : n_targets; /// modify the size of array of threads
//    for (U8 i = 0; i < n_targets; i += arrThrSize)
//    {
//        for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
//            arrThread[ j ] = thread( &FCM::compressTarget, &mixModel, mixModel.getTarAddresses()[ i + j ] );
//        for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
//            arrThread[ j ].join();
//    }

//    /// decompress target(s) using reference(s) model(s) -- multithreaded
//    if ( funcObj.getDecompressFlag() )
//    {
//        for (U8 i = 0; i < n_targets; i += arrThrSize)
//        {
//            for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
//                arrThread[ j ] = thread(&FCM::decompressTarget, &mixModel, mixModel.getTarAddresses()[ i + j ]);
//            for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
//                arrThread[ j ].join();
//        }
//    }
//
//    /// check if decompressed file is exactly the same as target file

    
    
    
    
    
    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now(); /// Record end time
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime; /// calculate and show duration in seconds
    
    cout << "Elapsed time: " << std::fixed << setprecision(3) << elapsed.count() << '\n';
    
    
    return 0;
}
