#include <iostream>
#include <getopt.h>     // for command line parsing
#include <fstream>
#include <vector>

#include "functions.h"
#include "messages.h"
#include "FCM.h"


/***********************************************************
    constructor
************************************************************/
Functions::Functions () {}


/***********************************************************
    parse command line
************************************************************/
int8_t Functions::commandLineParser (int argc, char **argv)
{
    Messages messageObj;    // object for showing messages
    
    // using these flags, if both short and long arguments
    // are entered, just one of them is considered
    static int h_flag;  // option 'h' (help)
    static int A_flag;  // option 'A' (about)
    static int v_flag;  // option 'v' (verbose)
    
    bool m_flag = false;                // model parameters entered
    std::string modelsParameters = "";  // argument of option 'm'
    
    bool t_flag = false;                // target file name entered
    bool r_flag = false;                // reference file name entered
    std::string targetFileName = "";    // argument of option 't'
    std::string referenceFileName = ""; // argument of option 'r'
    
    int c;              // deal with getopt_long()
    int option_index;   // option index stored by getopt_long()

    opterr = 0;         // force getopt_long() to remain silent when it finds a problem

    static struct option long_options[] =
            {
                    {"help",            no_argument, &h_flag, (int) 'h'},   // help
                    {"about",           no_argument, &A_flag, (int) 'A'},   // About
                    {"verbose",         no_argument, &v_flag, (int) 'v'},   // verbose
                    {"model",     required_argument,       0,       'm'},   // model
                    {"number",    required_argument,       0,       'n'},   // number (integer)
                    {"fnumber",   required_argument,       0,       'd'},   // number (float)
                    {"target",    required_argument,       0,       't'},   // target file
                    {"reference", required_argument,       0,       'r'},   // reference file
                    {0, 0, 0, 0}
            };
    
    while (1)
    {
        /* getopt_long() stores the option index here. */
        option_index = 0;

        c = getopt_long(argc, argv, ":hAvm:n:d:t:r:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[ option_index ].flag != 0)
                    break;

                std::cout << "option '" << long_options[ option_index ].name << "'\n";
                if (optarg)
                    std::cout << " with arg " << optarg << "\n";
                break;

            case 'h':   // show usage guide
                h_flag = 1;
                messageObj.help();
                break;
    
            case 'A':   // show About Phoenix
                A_flag = 1;
                messageObj.about();
                break;
                
            case 'v':   // verbose mode
                v_flag = 1;
                messageObj.verbose();
                break;
    
            case 'm':   // needs model parameters
                try
                {
                    m_flag = true;
                    modelsParameters = (std::string) optarg; // keep argument = model parameters
                }
                catch (const std::invalid_argument &ia)
                {
                    std::cerr << "Option 'm' ('model') has an invalid argument.\n";
                }
                break;

            case 'n':   // needs an integer argument
                try
                {
                    messageObj.number(std::stoi((std::string) optarg));    // TODO for test
                }
                catch (const std::invalid_argument &ia)
                {
                    std::cerr << "Option 'n' ('number') has an invalid argument.\n";
                }
                break;
    
            case 'd':   // needs a float argument
                try
                {
                    messageObj.fnumber( std::stof((std::string) optarg) );   // TODO for test
                }
                catch (const std::invalid_argument &ia)
                {
                    std::cerr << "Option 'd' ('fnumber') has an invalid argument.\n";
                }
                break;
    
            case 't':   // needs target file name
                t_flag = true;
                targetFileName = (std::string) optarg;      // keep argument = target file name
                break;
    
            case 'r':   // needs reference file name
                r_flag = true;
                referenceFileName = (std::string) optarg;   // keep argument = reference file name
                break;
                
            case ':':   /* missing option argument */
                std::cerr << "Option '" << (char) optopt << "' requires an argument.\n";
                break;

            case '?':   /* invalid option */
            default:
                std::cerr << "Option '" << (char) optopt << "' is invalid.\n";
                break;
        }
    }
    
    
    if (m_flag)
    {
        if (!t_flag && !r_flag)    std::cerr << "Input file address is needed.";
        else
        {
            std::vector< std::string > strModels;
    
            std::size_t mIndex = 0;
            for (size_t i = 0; i != modelsParameters.size(); ++i)
                if (modelsParameters[ i ] == ':')
                {
                    strModels.push_back(modelsParameters.substr(mIndex, i - mIndex));
                    mIndex = i + 1;
                }
            strModels.push_back(modelsParameters.substr(mIndex, modelsParameters.size() - mIndex));
    
    
            size_t n_models = strModels.size();
            FCM *models = new FCM[n_models];
            std::vector< std::string > vecParameters;
            size_t vecParamIndex = 0;
    
            for (size_t n = 0; n != n_models; ++n)
            {
                std::size_t index = 0;
                for (size_t i = 0; i != strModels[ n ].size(); ++i)
                    if (strModels[ n ][ i ] == ',')
                    {
                        vecParameters.push_back(strModels[ n ].substr(index, i - index));
                        index = i + 1;
                    }
                vecParameters.push_back(strModels[ n ].substr(index, strModels[ n ].size() - index));
        
                //
                (vecParameters[ vecParamIndex++ ][ 0 ] == 't') ? models[ n ].setTargetOrReference('t')
                                                               : models[ n ].setTargetOrReference('r');
                if(vecParameters[ vecParamIndex++ ][ 0 ] == 't')
                {
                    models[ n ].setTargetOrReference('t');
                    std::cout<<"tar";
                }
                else
                {
                    models[ n ].setTargetOrReference('r');
                }
                models[ n ].setContextDepth((uint8_t) std::stoi(vecParameters[ vecParamIndex++ ]));
                models[ n ].setAlphaDenom((uint8_t) std::stoi(vecParameters[ vecParamIndex++ ]));
                !std::stoi(vecParameters[ vecParamIndex++ ]) ? models[ n ].setInvertedRepeat(false)
                                                             : models[ n ].setInvertedRepeat(true);


////            models[ n ].setFileAddress(targetFileName);
                models[ n ].buildHashTable();
                models[ n ].printHashTable(models[ n ].getHashTable());
            }
    
            std::cout
                    << models[ 0 ].getTargetOrReference()
                    << "\n"
                    << (int) models[ 0 ].getContextDepth()
                    << "\n"
                    << (int) models[ 0 ].getAlphaDenom()
                    << "\n"
                    << (int) models[ 0 ].getInvertedRepeat()
                    << "\n"
                    << models[ 1 ].getTargetOrReference()
                    << "\n"
                    << (int) models[ 1 ].getContextDepth()
                    << "\n"
                    << (int) models[ 1 ].getAlphaDenom()
                    << "\n"
                    << (int) models[ 1 ].getInvertedRepeat(); // TODO for test
        }
    }
    else
    {
        if(t_flag)      std::cerr << "Model(s) parameters are missing.";
        else if(r_flag) std::cerr << "Model(s) parameters are missing.";
    }
    
    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        std::cerr << "non-option ARGV-element(s): ";
        while (optind < argc)
            std::cerr << argv[ optind++ ] << " ";
        std::cerr << std::endl;
    }
}


/***********************************************************
    check if file opened correctly
************************************************************/
bool Functions::isFileCorrect (const std::string &fileName)
{
    std::ifstream fileIn(fileName, std::ios::in);   // open file
    
    // check if file doesn't exist
    if (!fileIn)
    {
        std::cerr << "File '" << fileName << "' could not be opened.\n";
        fileIn.close(); // close file
        return false;   // error occurred while opening file
    }
    // check if file is empty
    else if (fileIn.peek() == std::ifstream::traits_type::eof())
    {
        std::cerr << "File '" << fileName << "' is empty.\n";
        fileIn.close(); // close file
        return false;   // error occurred while opening file
    }
    
    fileIn.close(); // close file
    
    return true;    // file opened correctly
}