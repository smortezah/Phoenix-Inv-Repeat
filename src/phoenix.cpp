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
    
    /// decompress
    if ( mixModel.getDecompFlag() )
    {
        FCM decModel;
        
        /// extract header information
        decModel.extractHeader( mixModel.getTarAddr()[ 0 ] );
        
        for (string s : mixModel.getRefAddr())   decModel.pushRefAddr(s);   /// reference(s) address(es)
        for (string s : mixModel.getTarAddr())   decModel.pushTarAddr(s);   /// target(s) address(es)
        
        /// build reference(s) model(s) -- multithreaded
        n_models   = mixModel.getN_models();
        n_threads  = mixModel.getN_threads();                       /// set based on command line
        arrThrSize = (n_models > n_threads) ? n_threads : n_models; /// size of threads array
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
        
        
////        for (int i = 0; i < 36; i++)    cout << mixModel.getTables()[ 0 ][ i ] << ' ';  cout<<"\n+++++\n";
////        for (int i = 0; i < 36; i++)    cout << decModel.getTables()[ 0 ][ i ] << ' ';  cout<<'\n';
////        for (int i = 0; i < 18; i++)    cout << mixModel.getTables()[ 1 ][ i ] << ' ';  cout<<"\n+++++\n";
////        for (int i = 0; i < 18; i++)    cout << decModel.getTables()[ 1 ][ i ] << ' ';  cout<<'\n';
//
////        for (int i = 0; i < 18; i++)    cout << mixModel.getTables()[ 0 ][ i ] << ' ';  cout<<"\n+++++\n";
////        for (int i = 0; i < 18; i++)    cout << decModel.getTables()[ 0 ][ i ] << ' ';  cout<<'\n';
//
////        decModel.printHashTable(0);
////        cout<<'\n';
////        decModel.printHashTable(1);
////cout<<"\n+++\n";
////        mixModel.printHashTable(0);
////        cout<<'\n';
////        mixModel.printHashTable(1);
        
        
        /// decompress target(s) using reference(s) model(s) -- multithreaded
        arrThrSize = (n_targets > n_threads) ? n_threads : n_targets;     /// modify threads array size
        arrThread  = new thread[ arrThrSize ];                           /// threads array
        for (U8 i = 0; i < n_targets; i += arrThrSize)
        {
            for (U8 j = 0; j < arrThrSize && i+j < n_targets; ++j)
                arrThread[ j ] = thread(&FCM::decompress, &decModel, decModel.getTarAddr()[ i+j ]);
            for (U8 j = 0; j < arrThrSize && i+j < n_targets; ++j)
                arrThread[ j ].join();
        }
        delete[] arrThread;
        
        /// check equality of decompressed and target files (check: lossless compression)
        string tarAddr;
        for (U8 i = n_targets; i--;)
        {
            tarAddr = decModel.getTarAddr()[ i ];
            
            cout << "Lossless compression of '" << tarAddr << "' was ";
            if ( areFilesEqual(tarAddr, tarAddr+DECOMP_FILETYPE) )  cout << "successful.\n";
            else cout << "not successful.\n";
        }
    }
    
    
    
    
    
    
    
    /// Record end time
    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now();
    /// calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    
    cout << "Elapsed time: " << std::fixed << setprecision(3) << elapsed.count() << '\n';
    
    return 0;
}
