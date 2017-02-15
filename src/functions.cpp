#include <iostream>
#include <getopt.h>     /// parsing command line
#include <fstream>
#include <vector>

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


/***********************************************************
    constructor
************************************************************/
Functions::Functions () {}


/***********************************************************
    parse command line
************************************************************/
void Functions::commandLineParser (int argc, char **argv)
{
    Messages messageObj;    /// object for showing messages
    
    /// using these flags, if both short and long arguments
    /// are entered, just one of them is considered
    static int h_flag;  /// option 'h' (help)
    static int A_flag;  /// option 'A' (about)
    static int v_flag;  /// option 'v' (verbose)
    
    bool m_flag = false;            /// model(s) parameters entered
    string modelsParameters = "";   /// argument of option 'm'
    
    bool t_flag = false;            /// target(s) file name entered
    bool r_flag = false;            /// reference(s) file name entered
    string targetFileName = "";     /// argument of option 't'
    string referenceFileName = "";  /// argument of option 'r'
    
    int c;              /// deal with getopt_long()
    int option_index;   /// option index stored by getopt_long()
    
    opterr = 0;         /// force getopt_long() to remain silent when it finds a problem
    
    static struct option long_options[] =
            {
                    {"help",      no_argument,       &h_flag, (int) 'h'},   /// help
                    {"about",     no_argument,       &A_flag, (int) 'A'},   /// About
                    {"verbose",   no_argument,       &v_flag, (int) 'v'},   /// verbose
                    {"model",     required_argument, 0,       'm'},         /// model
                    {"number",    required_argument, 0,       'n'},         /// number (integer)
                    {"fnumber",   required_argument, 0,       'd'},         /// number (float)
                    {"target",    required_argument, 0,       't'},         /// target file
                    {"reference", required_argument, 0,       'r'},         /// reference file
                    {0, 0,                           0,       0}
            };
    
    while (1)
    {
        /// getopt_long() stores the option index here.
        option_index = 0;
        
        c = getopt_long(argc, argv, ":hAvm:n:d:t:r:", long_options, &option_index);
        
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
            
            case 'm':   /// needs model(s) parameters
                try
                {
                    m_flag = true;
                    modelsParameters = (string) optarg; /// keep argument = model(s) parameters
                }
                catch (const invalid_argument &ia)
                {
                    cerr << "Option 'm' ('model') has an invalid argument.\n";
                }
                break;
            
            case 'n':   /// needs an integer argument
                try
                {
                    messageObj.number(stoi((string) optarg));    /// TODO for test
                }
                catch (const invalid_argument &ia)
                {
                    cerr << "Option 'n' ('number') has an invalid argument.\n";
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
                }
                break;
            
            case 't':   /// needs target file name
                t_flag = true;
                targetFileName = (string) optarg;      /// keep argument = target file name
                break;
            
            case 'r':   /// needs reference file name
                r_flag = true;
                referenceFileName = (string) optarg;   /// keep argument = reference file name
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
    
    
    if (m_flag)
    {
        /// check if target or reference file addresses are entered
        if (!t_flag || !r_flag)
            cerr << "Input file address is needed.";
        else
        {
            /// seperate and save the models in a vector of strings. each model in one string
            vector< string > strModels;
            uint8_t mIndex = 0;  /// index for the first character of models string
            /// save all models except the last model
            for (uint8_t i = 0; i != modelsParameters.size(); ++i)
                if (modelsParameters[ i ] == ':')
                {
                    strModels.push_back(modelsParameters.substr(mIndex, i - mIndex));
                    mIndex = i + 1;
                }
            /// save last model in multi-model input, and the only model in single-model input
            strModels.push_back(modelsParameters.substr(mIndex, modelsParameters.size() - mIndex));
//            uint8_t mIndex = modelsParameters.size();   /// index for the first character of models string
//            /// save all models except the last model
//            for (uint8_t i = mIndex; i--;)              /// = for (uint8_t i = modelsParameters.size(); i--;)
//                if (modelsParameters[ i ] == ':')
//                {
//                    strModels.push_back(modelsParameters.substr(i+1, mIndex));
//                    mIndex = i - 1;
//                }
//            /// save last model in multi-model input, and the only model in single-model input
//            strModels.push_back(modelsParameters.substr(0, mIndex));

            /// create an array of models and set their parameters
            uint8_t n_models = (uint8_t) strModels.size();  /// number of models
            FCM *models = new FCM[ n_models ];              /// array of models
            vector< string > vecParameters;                 /// to save models parameters
            uint8_t vecParamIndex = 0;
            
            /// save models parameters and process the models
//            for (uint8_t n = n_models; n--;)
            for (uint8_t n = 0; n != n_models; ++n)
            {
                uint8_t index = 0;
                /// save all models parameters except the last model
                for (uint8_t i = 0; i != strModels[ n ].size(); ++i)
                    if (strModels[ n ][ i ] == ',')
                    {
                        vecParameters.push_back(strModels[ n ].substr(index, i - index));
                        index = i + 1;
                    }
                /// save last model parameters in multi-model input, and the only model in single model input
                vecParameters.push_back(strModels[ n ].substr(index, strModels[ n ].size() - index));
                
                /// chack if the model is built from target or reference
                char tarOrRefChar = vecParameters[ vecParamIndex++ ][ 0 ];
                models[ n ].setTargetOrReference(tarOrRefChar);
                /// set target and/or reference file address
//                (tarOrRefChar == 't') ? models[ n ].setTarFileAddress(targetFileName)
//                                      : models[ n ].setRefFileAddress(referenceFileName);
                models[ n ].setTarFileAddress(targetFileName);
                models[ n ].setRefFileAddress(referenceFileName);
                /// set the context depth of the model
//                uint8_t ctxDepth = (uint8_t) stoi(vecParameters[ vecParamIndex++ ]);
//                models[ n ].setContextDepth(ctxDepth);
                models[ n ].setContextDepth((uint8_t) stoi(vecParameters[ vecParamIndex++ ]));
                /// set the alpha denominator of the model
                models[ n ].setAlphaDenom((uint16_t) stoi(vecParameters[ vecParamIndex++ ]));
//                models[ n ].setAlphaDenom(stod(vecParameters[ vecParamIndex++ ]));
                /// set the inverted repeat condition of the model
                !stoi(vecParameters[ vecParamIndex++ ]) ? models[ n ].setInvertedRepeat(false)
                                                        : models[ n ].setInvertedRepeat(true);
                
                /// print reference and target file names in the output
                size_t lastSlash_Ref = models[ n ].getRefFileAddress().find_last_of("/");
                size_t lastSlash_Tar = models[ n ].getTarFileAddress().find_last_of("/");
                
                cout << models[ n ].getRefFileAddress().substr(lastSlash_Ref + 1) << '\t'
                     << models[ n ].getTarFileAddress().substr(lastSlash_Tar + 1) << '\t';
                
                models[ n ].buildRefModel();    /// build a model for reference
                models[ n ].compressTarget();   /// compress target using model built based on reference
                
                /// print the built hash table
//                cout << "Model " << n + 1 << " parameters:\n";
//                models[ n ].printHashTable();
            }
            
            delete[] models;   /// delete all models created
        }   /// end - else: if target or reference file addresses are entered
    }   ///  end - if '-m' (model) is entered
    else    /// if '-m' (model) is entered but '-t' or '-r' (file addresses) are not entered
    {
        if (t_flag)
            cerr << "Model(s) parameters are missing.";
        else if (r_flag)
            cerr << "Model(s) parameters are missing.";
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
