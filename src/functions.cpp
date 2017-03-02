#include <iostream>
#include <getopt.h>     /// parsing command line
#include <fstream>
#include <vector>
#include <thread>
#include <cmath>

#include "functions.h"
#include "messages.h"
#include "FCM.h"

using std::cout;
using std::cerr;
using std::string;
using std::stoi;
using std::stof;
using std::vector;
using std::size_t;
using std::ifstream;
using std::ios;
using std::invalid_argument;
using std::thread;


/***********************************************************
    constructor
************************************************************/
Functions::Functions () {}


/***********************************************************
    parse command line
************************************************************/
void Functions::commandLineParser (int argc, char **argv)
{
    Messages messageObj;                /// object for showing messages
    FCM      mixModel;                  /// mixture of FCM models
    
    U8 n_threads = DEFAULT_N_THREADS;   /// number of threads
    double gamma = DEFAULT_GAMMA;       /// gamma
    
    /// using these flags, if both short and long arguments
    /// are entered, just one of them is considered
    static int h_flag;                  /// option 'h' (help)
    static int A_flag;                  /// option 'A' (about)
    static int v_flag;                  /// option 'v' (verbose)
    
    /// mandatory arguments
    bool m_flag = false;                /// model(s) parameters entered
    bool r_flag = false;                /// reference(s) file name entered
    bool t_flag = false;                /// target(s) file name entered
    string strModelsParameters = "";    /// argument of option 'm'
    string refFilesNames = "";          /// argument of option 'r'
    string tarFilesNames = "";          /// argument of option 't'
    
    int c;                              /// deal with getopt_long()
    int option_index;                   /// option index stored by getopt_long()
                                        
    opterr = 0;                         /// force getopt_long() to remain silent when it finds a problem
    
    static struct option long_options[] =
            {
                    {"help",      no_argument,       &h_flag, (int) 'h'},   /// help
                    {"about",     no_argument,       &A_flag, (int) 'A'},   /// About
                    {"verbose",   no_argument,       &v_flag, (int) 'v'},   /// verbose
                    {"model",     required_argument, 0,             'm'},   /// model(s)
                    {"reference", required_argument, 0,             'r'},   /// reference(s) file(s)
                    {"target",    required_argument, 0,             't'},   /// target(s) file(s)
                    {"n_threads", required_argument, 0,             'n'},   /// number of threads >= 1
                    {"gamma",     required_argument, 0,             'g'},   /// 0 <= gamma (float) < 1
                    {0,           0,                 0,               0}
            };
    
    while (1)
    {
        /// getopt_long() stores the option index here.
        option_index = 0;
        
        c = getopt_long(argc, argv, ":hAvm:r:t:n:g:", long_options, &option_index);
        
        /// Detect the end of the options.
        if (c == -1)
            break;
        
        switch (c)
        {
            case 0:
                /// If this option set a flag, do nothing else now.
                if (long_options[ option_index ].flag != 0) break;
                
                cout << "option '" << long_options[ option_index ].name << "'\n";
                if (optarg) cout << " with arg " << optarg << '\n';
                break;
            
            case 'h':   /// show usage guide
                h_flag = 1;
                messageObj.help();
                break;
            
            case 'A':   /// show About Phoenix
                A_flag = 1;
                messageObj.about();
                break;
            
            case 'v':   /// verbose mode
                v_flag = 1;
                messageObj.verbose();
                break;
            
            case 'm':   /// needs model(s) parameters
                m_flag = true;
                strModelsParameters = (string) optarg;  /// keep argument = model(s) parameters
                break;
                
            case 'r':   /// needs reference file(s) name(s)
                r_flag = true;
                refFilesNames = (string) optarg;        /// keep argument = reference files names
                break;
                
            case 't':   /// needs target file(s) name(s)
                t_flag = true;
                tarFilesNames = (string) optarg;        /// keep argument = target files names
                break;
                
            case 'n':   /// needs an integer argument
                try
                {
                    n_threads = (U8) stoi((string) optarg);
                    if (n_threads < 1)  n_threads = DEFAULT_N_THREADS;
                }
                catch (const invalid_argument &ia)
                {
                    cerr << "Option 'n' ('n_threads') has an invalid argument.\n";
                    return;
                }
                break;
            
            case 'g':   /// needs a double argument
                try
                {
                    gamma = stod((string) optarg);
                    if (gamma < 0 || gamma >= 1)    gamma = DEFAULT_GAMMA;
                }
                catch (const invalid_argument &ia)
                {
                    cerr << "Option 'g' ('gamma') has an invalid argument.\n";
                    return;
                }
                break;
                
            case ':':   /// missing option argument
                cerr << "Option '" << (char) optopt << "' requires an argument.\n";
                break;
            
            case '?':   /// invalid option
            default:
                cerr << "Option '" << (char) optopt << "' is invalid.\n";
                break;
        }
    }
    
    /// save target file(s) name(s)
    if (t_flag)
    {
        string::iterator begIter = tarFilesNames.begin(),   endIter = tarFilesNames.end();
        for (string::iterator it = begIter; it != endIter; ++it)      /// all target files names but the last one
            if (*it == ',')
            {
                mixModel.pushBackTarAddresses( string(begIter, it) );
                begIter = it + 1;
            }
        mixModel.pushBackTarAddresses( string(begIter, endIter) );    /// last target file name
        
        /*  slower
        U8 tarIndex = (U8) tarFilesNames.size();
        /// save all target files names except the last one
        for (U8 i = tarIndex; i--;)
        {
            if (tarFilesNames[ i ] == ',')
            {
                mixModel.pushBackTarAddresses(tarFilesNames.substr(i + 1, tarIndex - i - 1));
                tarIndex = i;
            }
        }
        /// save last target file name
        mixModel.pushBackTarAddresses(tarFilesNames.substr(0, tarIndex));
        */
    }
    
    /// save reference file(s) name(s)
    if (r_flag)
    {
        string::iterator begIter = refFilesNames.begin(),   endIter = refFilesNames.end();
        for (string::iterator it = begIter; it != endIter; ++it)      /// all reference files names but the last one
            if (*it == ',')
            {
                mixModel.pushBackRefAddresses( string(begIter, it) );
                begIter = it + 1;
            }
        mixModel.pushBackRefAddresses( string(begIter, endIter) );    /// last reference file name

        /*  slower
        U8 refIndex = (U8) refFilesNames.size();
        /// save all reference files names except the last one
        for (U8 i = refIndex; i--;)
        {
            if (refFilesNames[ i ] == ',')
            {
                mixModel.pushBackRefAddresses(refFilesNames.substr(i + 1, refIndex - i - 1));
                refIndex = i;
            }
        }
        /// save last reference file name
        mixModel.pushBackRefAddresses(refFilesNames.substr(0, refIndex));
        */
    }
    
    /// save model(s) parameters and process the model(s)
    if (m_flag)
    {
        vector< string > vecModelsParams;                             /// parameters for different models
        string::iterator begIter = strModelsParameters.begin(),   endIter = strModelsParameters.end();
        for (string::iterator it = begIter; it != endIter; ++it)      /// all models parameters but the last one
            if (*it == ':')
            {
                vecModelsParams.push_back( string(begIter, it) );
                begIter = it + 1;
            }
        vecModelsParams.push_back( string(begIter, endIter) );        /// last model parameters
        
        vector< string > modelParams;                                 /// parameters for each model
        U8 n_models = (U8) vecModelsParams.size();                    /// number of models
        mixModel.setN_models(n_models);                               /// set number of models
    
        for (U8 n = n_models; n--;)
        {
            modelParams.clear();                                      /// reset vector modelParams

            begIter = vecModelsParams[ n ].begin(), endIter = vecModelsParams[ n ].end();
            for (string::iterator it = begIter; it != endIter; ++it)/// all paramaeters for each model but the last one
                if (*it == ',')
                {
                    modelParams.push_back( string(begIter, it) );
                    begIter = it + 1;
                }
            modelParams.push_back( string(begIter, endIter) );        /// parameters for the last model
            
            /// set model(s) parameters
            mixModel.pushBackParams( (bool) stoi( modelParams[0] ),   /// inverted repeat
                                     (U8)   stoi( modelParams[1] ),   /// context depth
                                     (U16)  stoi( modelParams[2] ) ); /// alpha denominator
        }
        
        /// build reference(s) model(s) -- multithreaded
        /// set compression mode: 't'=table, 'h'=hash table -- 5^k_1 + 5^k_2 + ... > 5^12 ==> mode: hash table
        U64 cmpModeSum = 0;
        for (U8 k : mixModel.getContextDepths())    cmpModeSum = cmpModeSum + (U64) pow(ALPH_SIZE, k);
        const char compressionMode = (cmpModeSum > pow(ALPH_SIZE, TABLE_MAX_CTX)) ? 'h' : 't';
        mixModel.setCompressionMode( compressionMode );
        
        /// initialize vector of tables or hash tables
        compressionMode == 'h' ? mixModel.initHashTables() : mixModel.initTables();
        
        U8 arrThrSize = (n_models > n_threads) ? n_threads : n_models;/// size of array of threads
//        thread *arrThread = new thread[ arrThrSize ];                 /// array of threads
        thread arrThread[ arrThrSize ];                               /// array of threads
        
        for (U8 i = 0; i < n_models; i += arrThrSize)
        {
            for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
                arrThread[ j ] = thread( &FCM::buildModel, &mixModel,
                                         mixModel.getInvertedRepeats()[ i + j ],
                                         mixModel.getContextDepths()[ i + j ],
                                         i + j );

            for (U8 j = 0; j < arrThrSize && i + j < n_models; ++j)
                arrThread[ j ].join();
        }
        
//        delete[] arrThread;                                           /// free up the memory for array of threads

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

        /// compress target(s) using reference(s) model(s) -- multithreaded
        mixModel.setGamma(gamma);                                     /// set gamma
        U8 n_targets = (U8) mixModel.getTarAddresses().size();        /// up to 2^8=256 targets

        arrThrSize = (n_targets > n_threads) ? n_threads : n_targets;
//        arrThread  = new thread[ arrThrSize ];                        /// array of threads

        for (U8 i = 0; i < n_targets; i += arrThrSize)
        {
            for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
                arrThread[ j ] = thread( &FCM::compressTarget, &mixModel, mixModel.getTarAddresses()[ i + j ] );

            for (U8 j = 0; j < arrThrSize && i + j < n_targets; ++j)
                arrThread[ j ].join();
        }

//        delete[] arrThread;                                           /// free up the memory for array of threads
    }
    
    /// Print any remaining command line arguments (not options).
    if (optind < argc)
    {
        cerr << "non-option ARGV-element(s): ";
        while (optind < argc)   cerr << argv[ optind++ ] << " ";
        cerr << '\n';
    }
    
}   /// end commandLineParser


///***********************************************************
//    check if file opened correctly
//************************************************************/
////bool Functions::isFileCorrect (const string &fileName)
//bool Functions::isFileCorrect (ifstream &fileIn)
//{
//    // check if file doesn't exist or is empty
//    if (!fileIn)
//    {
////        cerr << "The file cannot be opened, or it is empty.\n";
//        fileIn.close(); // close file
//        return false;   // error occurred while opening file
//    }
////    // check if file is empty
////    else if (fileIn.peek() == ifstream::traits_type::eof())
////    {
////        cerr << "The File is empty.\n";
////        fileIn.close(); // close file
////        return false;   // error occurred while opening file
////    }
//
//    return true;    // file opened correctly
//}
