#include <iostream>
#include <getopt.h>     /// parsing command line
#include <fstream>
#include <vector>
#include <thread>

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
    Messages messageObj;            /// object for showing messages
    FCM model;                      /// model
    
    /// using these flags, if both short and long arguments
    /// are entered, just one of them is considered
    static int h_flag;              /// option 'h' (help)
    static int A_flag;              /// option 'A' (about)
    static int v_flag;              /// option 'v' (verbose)
                                    
    bool m_flag = false;            /// model parameters entered
    bool t_flag = false;            /// target(s) file name entered
    bool r_flag = false;            /// reference(s) file name entered
    string modelParameters = "";    /// argument of option 'm'
    string tarFilesNames = "";      /// argument of option 't'
    string refFilesNames = "";      /// argument of option 'r'
    
    uint8_t n_threads = DEFAULT_N_THREADS;  /// number of threads
    
    int c;                         /// deal with getopt_long()
    int option_index;              /// option index stored by getopt_long()
                                   
    opterr = 0;                    /// force getopt_long() to remain silent when it finds a problem
    
    static struct option long_options[] =
            {
                    {"help",      no_argument,       &h_flag, (int) 'h'},   /// help
                    {"about",     no_argument,       &A_flag, (int) 'A'},   /// About
                    {"verbose",   no_argument,       &v_flag, (int) 'v'},   /// verbose
                    {"model",     required_argument, 0,             'm'},   /// model
                    {"reference", required_argument, 0,             'r'},   /// reference file
                    {"target",    required_argument, 0,             't'},   /// target file
                    {"n_threads", required_argument, 0,             'n'},   /// number of threads
                    {"fnumber",   required_argument, 0,             'd'},   /// number (float)
                    {0,           0,                 0,               0}
            };
    
    while (1)
    {
        /// getopt_long() stores the option index here.
        option_index = 0;
        
        c = getopt_long(argc, argv, ":hAvm:r:t:n:d:", long_options, &option_index);
        
        /// Detect the end of the options.
        if (c == -1)
            break;
        
        switch (c)
        {
            case 0:
                /// If this option set a flag, do nothing else now.
                if (long_options[ option_index ].flag != 0)
                    break;
                
                cout << "option '" << long_options[ option_index ].name << "'\n";
                if (optarg)
                    cout << " with arg " << optarg << '\n';
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
            
            case 'm':   /// needs model parameters
                m_flag = true;
                modelParameters = (string) optarg;  /// keep argument = model parameters
                break;
                
            case 'r':   /// needs reference file name
                r_flag = true;
                refFilesNames = (string) optarg;    /// keep argument = reference files names
                break;
                
            case 't':   /// needs target files names
                t_flag = true;
                tarFilesNames = (string) optarg;    /// keep argument = target files names
                break;
                
            case 'n':   /// needs an integer argument
                try
                {
                    n_threads = (uint8_t) stoi((string) optarg);
                    if (n_threads < 1)  n_threads = DEFAULT_N_THREADS;
                }
                catch (const invalid_argument &ia)
                {
                    cerr << "Option 'n' ('n_threads') has an invalid argument.\n";
                    return;
                }
                break;
            
            case 'd':   /// needs a float argument
                try
                {
                    messageObj.fnumber(stof((string) optarg));   /// TODO for test
                }
                catch (const invalid_argument &ia)
                {
                    cerr << "Option 'd' ('fnumber') has an invalid argument.\n";
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
        uint8_t tarIndex = (uint8_t) tarFilesNames.size();
        /// save all target files names except the last one
        for (uint8_t i = tarIndex; i--;)
        {
            if (tarFilesNames[ i ] == ',')
            {
                model.pushBackTarFilesAddresses( tarFilesNames.substr(i+1, tarIndex-i-1) );
                tarIndex = i;
            }
        }
        /// save last target file name
        model.pushBackTarFilesAddresses( tarFilesNames.substr(0, tarIndex) );
    }
    
    /// save reference file(s) name(s)
    if (r_flag)
    {
        uint8_t refIndex = (uint8_t) refFilesNames.size();
        /// save all reference files names except the last one
        for (uint8_t i = refIndex; i--;)
        {
            if (refFilesNames[ i ] == ',')
            {
                model.pushBackRefFilesAddresses( refFilesNames.substr(i+1, refIndex-i-1) );
                refIndex = i;
            }
        }
        /// save last reference file name
        model.pushBackRefFilesAddresses( refFilesNames.substr(0, refIndex) );
    }
    
    /// save model parameters and process the model
    if (m_flag)
    {
        vector< string > vecParameters; /// to save model parameters (ir, ctx_size, alpha)
        uint8_t vecParamIndex = 0;     /// to traverse vecParameters
        
        uint8_t parIndex = (uint8_t) modelParameters.size();
        /// save all model parameters except the last model
        for (uint8_t i = parIndex; i--;)
        {
            if (modelParameters[ i ] == ',')
            {
                vecParameters.push_back( modelParameters.substr(i+1, parIndex-i-1) );
                parIndex = i;
            }
        }
        /// save last model parameter
        vecParameters.push_back(modelParameters.substr(0, parIndex));
        
        /// set the alpha denominator of the model
        model.setAlphaDenom((uint16_t) stoi(vecParameters[ vecParamIndex++ ]));
//        model.setAlphaDenom(stod(vecParameters[ vecParamIndex++ ]));
        /// set the context depth of the model
        model.setContextDepth((uint8_t) stoi(vecParameters[ vecParamIndex++ ]));
        /// set the inverted repeat condition of the model
        !stoi(vecParameters[ vecParamIndex ]) ? model.setInvertedRepeat(false)
                                              : model.setInvertedRepeat(true);
        
        /// build a model based on reference(s)
        model.buildModel();
        
        /*
        /// compress target(s) using reference(s) model -- multithreaded
        uint8_t MAX_N_THREADS = (uint8_t) thread::hardware_concurrency();   /// max cores in current machine
        /// N_FREE_THREADS considered for other jobs in current system
        uint8_t n_threads_available = (uint8_t) (!MAX_N_THREADS ? DEFAULT_N_THREADS - N_FREE_THREADS
                                                                : MAX_N_THREADS - N_FREE_THREADS);
        uint8_t n_targets = (uint8_t) model.getTarFilesAddresses().size();  /// up to 2^8=256 targets

        uint8_t arrThrSize = (n_targets > n_threads_available) ? n_threads_available : n_targets;
        thread *arrThread = new thread[arrThrSize];   /// array of threads
        */
        
        /// compress target(s) using reference(s) model -- multithreaded
        uint8_t n_targets = (uint8_t) model.getTarFilesAddresses().size();  /// up to 2^8=256 targets

        uint8_t arrThrSize = (n_targets > n_threads) ? n_threads : n_targets;
        thread *arrThread = new thread[ arrThrSize ];   /// array of threads

        for (uint8_t i = 0; i < n_targets; i += arrThrSize)
        {
            for (uint8_t j = 0; j < arrThrSize && i + j < n_targets; ++j)
                arrThread[ j ] = thread(&FCM::compressTarget, &model, model.getTarFilesAddresses()[ i + j ]);

            for (uint8_t j = 0; j < arrThrSize && i + j < n_targets; ++j)
                arrThread[ j ].join();
        }

        delete[] arrThread;
    }
    
    /// Print any remaining command line arguments (not options).
    if (optind < argc)
    {
        cerr << "non-option ARGV-element(s): ";
        while (optind < argc)
            cerr << argv[ optind++ ] << " ";
        cerr << '\n';
    }
}


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
