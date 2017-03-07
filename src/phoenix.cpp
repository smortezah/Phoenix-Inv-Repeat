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
    /// Record start time
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();
    
    FCM mixModel;                               /// object on memory stack
    
    commandLineParser(argc, argv, mixModel);    /// parse the command line
    
    /// build reference(s) model(s) -- multithreaded
    const U8 n_models  = mixModel.getN_models();
    const U8 n_threads = mixModel.getN_threads();
    U8 arrThrSize = (n_models > n_threads) ? n_threads : n_models;/// size of array of threads
    thread arrThread[ arrThrSize ];                               /// array of threads
    for (U8 i = 0; i < n_models; i += arrThrSize)
    {
        for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
            arrThread[ j ] = thread( &FCM::buildModel, &mixModel, mixModel.getIR()[ i + j ],
                                     mixModel.getCtxDepth()[ i + j ], i + j );
        for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
            arrThread[ j ].join();
    }
    /*
    /// compress target(s) using reference(s) model -- multithreaded
    U8 MAX_N_THREADS = (U8) thread::hardware_concurrency(); /// max cores in current machine
    /// N_FREE_THREADS considered for other jobs in current system
    U8 n_threads_available = (U8) (!MAX_N_THREADS ? DEFAULT_N_THREADS - N_FREE_THREADS
                                                            : MAX_N_THREADS - N_FREE_THREADS);
    U8 n_targets = (U8) model.getTarAddr().size();     /// up to 2^8=256 targets

    U8 arrThrSize = (n_targets > n_threads_available) ? n_threads_available : n_targets;
    thread *arrThread = new thread[arrThrSize];             /// array of threads
    */
    
    /// compress target(s) using reference(s) model(s) -- multithreaded
    U8 n_targets = (U8) mixModel.getTarAddr().size();   /// up to 2^8=256 targets
    /// modify the size of array of threads
    arrThrSize = (n_targets > n_threads) ? n_threads : n_targets;
    for (U8 i = 0; i < n_targets; i += arrThrSize)
    {
        for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
            arrThread[ j ] = thread(&FCM::compress, &mixModel, mixModel.getTarAddr()[ i + j ] );
        for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
            arrThread[ j ].join();
    }
    
    /// decompress target(s) using reference(s) model(s) -- multithreaded
    if (mixModel.getDecompFlag() )
    {
        for (U8 i = 0; i < n_targets; i += arrThrSize)
        {
            for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
                arrThread[ j ] = thread(&FCM::decompress, &mixModel, mixModel.getTarAddr()[ i + j ]);
            for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
                arrThread[ j ].join();
        }
    }
    
    /// check if decompressed file is exactly the same as target file
    
    
    
    
    
    /// Record end time
    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now();
    /// calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    
    cout << "Elapsed time: " << std::fixed << setprecision(3) << elapsed.count() << '\n';
    
    return 0;
}
