#include "functions.h"
#include "messages.h"

#include <iostream>
#include <getopt.h>     // for command line parsing
#include <string>
#include <fstream>


/***********************************************************
    constructor
************************************************************/
Functions::Functions () {}


/***********************************************************
    command line parser
************************************************************/
int32_t Functions::commandLineParser (int argc, char **argv)
{
    // using these flags, if both short and long arguments
    // are entered, just one of them is considered
    static int h_flag;  // option 'h' (help)
    static int V_flag;  // option 'V' (version)
    static int v_flag;  // option 'v' (verbose)
    static int i_flag;  // option 'i' (inverted_repeat)
    
    int c;              // deal with getopt_long()
    int option_index;

    opterr = 0;         // force getopt_long() to remain silent when it finds a problem

    static struct option long_options[] =
            {
                    {"help",            no_argument, &h_flag, (int) 'h'},   // help
                    {"version",         no_argument, &V_flag, (int) 'V'},   // version
                    {"verbose",         no_argument, &v_flag, (int) 'v'},   // verbose
                    {"inverted_repeat", no_argument, &i_flag, (int) 'i'},   // inverted_repeat
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

        c = getopt_long(argc, argv, ":hVvin:d:t:r:", long_options, &option_index);

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

            case 'h':   // shows usage guide
                h_flag = 1;
                break;

            case 'V':   // shows version number
                V_flag = 1;
                break;

            case 'v':   // verbose mode
                v_flag = 1;
                break;
    
            case 'i':   // inverted_repeat mode
                i_flag = 1;
                break;

            case 'n':   // needs an integer argument
                try
                {
                    Messages::number( std::stoi((std::string) optarg) );    //for test
                }
                catch (const std::invalid_argument& ia)
                {
                    std::cerr << "Option 'n' ('number') has an invalid argument.\n";
                }
                break;
    
            case 'd':   // needs a float argument
                try
                {
                    Messages::fnumber( std::stof((std::string) optarg) );   //for test
                }
                catch (const std::invalid_argument& ia)
                {
                    std::cerr << "Option 'd' ('fnumber') has an invalid argument.\n";
                }
                break;
    
            case 't':   // needs target file name
                if (Functions::fileRead( (std::string) optarg) != "")
                    std::cout << "Dataset: " << Functions::fileRead((std::string) optarg) << "\n";
                break;
    
            case 'r':   // needs reference file name
                if (Functions::fileRead( (std::string) optarg) != "")
                    std::cout << "Dataset: " << Functions::fileRead((std::string) optarg) << "\n";
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
    
    if (h_flag) Messages::help();
    if (V_flag) Messages::version();
    if (v_flag) Messages::verbose();
    if (i_flag) Messages::inverted_repeat();

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
    reads a file
************************************************************/
std::string Functions::fileRead (std::string fileName)
{
    // open file
    std::ifstream myFile(fileName, std::ios::in);
    
    if (!myFile)
    {
        std::cerr << "File \"" << fileName << "\" could not be opened.\n";
        return "";
    }
    
    std::string strEachLine;
    std::string strDataset;
    
    while (std::getline(myFile, strEachLine))
        strDataset += strEachLine;
    
    myFile.close();     // close file
    
//    // show the dataset
//    std::cout << "Dataset: " << strDataset << std::endl;
    
    return strDataset;
}
