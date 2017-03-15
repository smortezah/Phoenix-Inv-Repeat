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
    
    FCM mixModel;       /// object on memory stack
    
    /// parse the command line
    commandLineParser(argc, argv, mixModel);

    thread *arrThread;  /// array of threads
    
    /// build reference(s) model(s) -- multithreaded
    U8 n_models   = mixModel.getN_models();
    U8 n_threads  = mixModel.getN_threads();
    U8 arrThrSize = (n_models > n_threads) ? n_threads : n_models;  /// size of threads array
    arrThread     = new thread[ arrThrSize ];
    for (U8 i = 0; i < n_models; i += arrThrSize)
    {
        for (U8 j = 0; j < arrThrSize && i+j < n_models; ++j)
            arrThread[ j ] = thread( &FCM::buildModel, &mixModel,
                                     mixModel.getRefAddr(), mixModel.getIR()[ i+j ],
                                     mixModel.getCtxDepth()[ i+j ], i + j );
        for (U8 j = 0; j < arrThrSize && i+j < n_models; ++j)
            arrThread[ j ].join();
    }
    delete[] arrThread;
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
    U8 n_targets = (U8) mixModel.getTarAddr().size();                   /// up to 2^8=256 targets
    arrThrSize   = (n_targets > n_threads) ? n_threads : n_targets;     /// modify threads array size
    arrThread    = new thread[ arrThrSize ];                            /// resize threads array
    for (U8 i = 0; i < n_targets; i += arrThrSize)
    {
        for (U8 j = 0; j < arrThrSize && i+j < n_targets; ++j)
            arrThread[ j ] = thread(&FCM::compress, &mixModel, mixModel.getTarAddr()[ i+j ] );
        for (U8 j = 0; j < arrThrSize && i+j < n_targets; ++j)
            arrThread[ j ].join();
    }
    delete[] arrThread;

    /// decompress target(s) using reference(s) model(s) -- multithreaded
    if ( mixModel.getDecompFlag() )
    {
        FCM decModel;
//mixModel.clearModel();
        /// extract header information
        decModel.extractHeader( mixModel.getTarAddr()[ 0 ] );
//        vector<string> refAddr = mixModel.getRefAddr();
        for(string s : mixModel.getRefAddr())   decModel.pushRefAddr(s);
//        cout<<decModel.getGamma();
        
        /// build reference(s) model(s) -- multithreaded
        n_models   = mixModel.getN_models();
        n_threads  = mixModel.getN_threads();                       /// set based on command line
        arrThrSize = (n_models > n_threads) ? n_threads : n_models; /// size of threads array
    
    
        
//        cout<<mixModel.getRefAddr()[0]<<mixModel.getIR()[0]<<(int)mixModel.getCtxDepth()[0];
//        decModel.buildModel(decModel.getRefAddr(), decModel.getIR()[ 0 ], decModel.getCtxDepth()[ 0 ], 0, decModel);
        
//        mixModel.buildModel(mixModel.getRefAddr(), mixModel.getIR()[ 0 ], mixModel.getCtxDepth()[ 0 ], 0);
//        mixModel.buildModel(mixModel.getRefAddr(), mixModel.getIR()[ 1 ], mixModel.getCtxDepth()[ 1 ], 1);
        
        arrThread  = new thread[ arrThrSize ];
        for (U8 i = 0; i < n_models; i += arrThrSize)
        {
            for (U8 j = 0; j < arrThrSize && i+j < n_models; ++j)
                arrThread[ j ] = thread( &FCM::buildModel, &decModel,
                                         decModel.getRefAddr(), decModel.getIR()[ i+j ],
                                         decModel.getCtxDepth()[ i+j ], i + j );
            for (U8 j = 0; j < arrThrSize && i+j < n_models; ++j)
                arrThread[ j ].join();
        }
        delete[] arrThread;
    
    
    
//        cout << (int) decModel.getIR()[ 0 ] << (int) decModel.getIR()[ 1 ];
//        cout << (int) decModel.getCtxDepth()[ 0 ] << (int) decModel.getCtxDepth()[ 1 ];
//        cout << (int) mixModel.getCtxDepth()[ 0 ] << (int) mixModel.getCtxDepth()[ 1 ];
        
        for (int i = 0; i < 36; i++)
            cout << decModel.getTables()[ 0 ][ i ] << ' ';
            cout<<'\n';
//        for (int i = 0; i < 36; i++)
//            cout << mixModel.getTables()[ 1 ][ i ] << ' ';
        
        
        
        
//        arrThread = new thread[ arrThrSize ];                           /// threads array
//        for (U8 i = 0; i < n_targets; i += arrThrSize)
//        {
//            for (U8 j = 0; j < arrThrSize && i+j < n_targets; ++j)
//                arrThread[ j ] = thread(&FCM::decompress, &mixModel,
//                                        mixModel.getTarAddr()[ i+j ], mixModel.getRefAddr());
//            for (U8 j = 0; j < arrThrSize && i+j < n_targets; ++j)
//                arrThread[ j ].join();
//        }
//        delete[] arrThread;
    }

    
//    /// check if decompressed file and target file are exactly the same (check: lossless compression)
//    cout<<areFilesEqual("y","DECOMP.de");

    
    
    
    
    
    
    
    
    
    /// Record end time
    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now();
    /// calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    
    cout << "Elapsed time: " << std::fixed << setprecision(3) << elapsed.count() << '\n';
    
    return 0;
}
