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
    high_resolution_clock::time_point exeStartTime =    /// Record start time
            high_resolution_clock::now();
    
    FCM mixModel;       /// object on memory stack
    
    /// parse the command line
    commandLineParser(argc, argv, mixModel);
    
    thread *arrThread;  /// array of threads
    
    /// build reference(s) model(s) -- multithreaded
    U8 n_models   = mixModel.getN_models();
    U8 n_threads  = mixModel.getN_threads();
    U8 arrThrSize = (n_models > n_threads)
                    ? n_threads : n_models;  /// size of threads array
    arrThread = new thread[arrThrSize];
    for (U8 i = 0; i < n_models; i += arrThrSize)
    {
        //TODO: aya jaygozini vase sharte "i+j < n_models" hast?
        for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
            arrThread[ j ] = thread( &FCM::buildModel, &mixModel,
                                     mixModel.getRefAddr(),
                                     mixModel.getIR()[i+j],
                                     mixModel.getCtxDepth()[i+j], i + j );
        for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
            arrThread[ j ].join();
    }
    delete[] arrThread;
    /*
    /// compress target(s) using reference(s) model -- multithreaded
    /// max cores in current machine
    U8 MAX_N_THREADS = (U8) thread::hardware_concurrency();
    /// N_FREE_THREADS considered for other jobs in current system
    U8 n_threads_available = (U8) ( !MAX_N_THREADS
                                    ? DEFAULT_N_THREADS - N_FREE_THREADS
                                    : MAX_N_THREADS - N_FREE_THREADS );
    U8 n_targets = (U8) mixModel.getTarAddr().size(); /// up to 2^8=256 targets
 
    U8 arrThrSize = (n_targets > n_threads_available)
                    ? n_threads_available : n_targets;
    thread *arrThread = new thread[arrThrSize];       /// array of threads
    */
    
    /// compress target(s) using reference(s) model(s) -- multithreaded
    U8 n_targets = (U8) mixModel.getTarAddr().size();  /// up to 2^8=256 targets
    /// modify threads array size
    arrThrSize = (n_targets > n_threads) ? n_threads : n_targets;
    arrThread = new thread[arrThrSize];                /// resize threads array
    for (U8 i = 0; i < n_targets; i += arrThrSize)
    {
        for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
            arrThread[ j ] = thread( &FCM::compress, &mixModel,
                                     mixModel.getTarAddr()[i+j] );
        for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
            arrThread[ j ].join();
    }
    delete[] arrThread;
    
    /// decompress
    if (mixModel.getDecompFlag())
    {
        FCM decModel;
//        /// reference(s) and target(s) address(es)
//        for (string s : mixModel.getRefAddr())  decModel.pushRefAddr(s);
//        for (string s : mixModel.getTarAddr())  decModel.pushTarAddr(s);
//
//        /// extract header information
//        decModel.extractHeader(decModel.getTarAddr()[ 0 ]);
//
//        /// build reference(s) model(s) -- multithreaded
//        n_models   = mixModel.getN_models();
//        n_threads  = mixModel.getN_threads();      /// set based on command line
//        arrThrSize = (n_models > n_threads)
//                     ? n_threads : n_models;       /// size of threads array
//        arrThread = new thread[arrThrSize];
//        for (U8 i = 0; i < n_models; i += arrThrSize)
//        {
//            for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
//                arrThread[ j ] = thread( &FCM::buildModel, &decModel,
//                                         decModel.getRefAddr(),
//                                         decModel.getIR()[i+j],
//                                         decModel.getCtxDepth()[i+j], i + j );
//            for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
//                arrThread[ j ].join();
//        }
//        delete[] arrThread;
//
//        /// decompress target(s) using reference(s) model(s) -- multithreaded
//        /// modify thread array size
//        arrThrSize = (n_targets > n_threads) ? n_threads : n_targets;
//        arrThread = new thread[arrThrSize];     /// threads array
//        for (U8 i = 0; i < n_targets; i += arrThrSize)
//        {
//            for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
//                arrThread[ j ] = thread( &FCM::decompress, &decModel,
//                                        decModel.getTarAddr()[i+j] );
//            for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
//                arrThread[ j ].join();
//        }
//        delete[] arrThread;
//
//        /// check equality of decompressed & tar. files (check lossless comp.)
//        for (string s : decModel.getTarAddr())
//            if (!areFilesEqual(s, s + DECOMP_FILETYPE))
//            {
//                cerr << "Lossless compression/decompression of '" << s
//                     << "' failed.\n";
//                exit(1);
//            }
//        U8 tarsNo = (U8) decModel.getTarAddr().size();
//        size_t lastSlashPos;
//        string tarNamesPure[tarsNo];
//        for (U8 i = tarsNo; i--;)
//        {
//            lastSlashPos = decModel.getTarAddr()[ i ].find_last_of("/");
//            tarNamesPure[i] = decModel.getTarAddr()[i].substr(lastSlashPos + 1);
//        }
//        cout << "Lossless compression and decompression of '";
//        for (int i = 0; i < tarsNo-1; ++i) cout << tarNamesPure[ i ] << "', '";
//        cout << tarNamesPure[ tarsNo - 1 ] << "' was successful.\n";
    }   /// end decompress
    
    
    
    
    
    
    
    
    high_resolution_clock::time_point exeFinishTime =   /// Record end time
            high_resolution_clock::now();
    /// calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    
    cout << "Elapsed time: "
         << std::fixed << setprecision(3) << elapsed.count() << '\n';
    
    return 0;
}
