#include "Functions.h"
#include "messages.h"

#include <iostream>
#include <getopt.h>
#include <string>


/***********************************************************
    constructor
************************************************************/
Functions::Functions () {}


/***********************************************************
    command line parser
************************************************************/
int32_t Functions::commandLineParser (int argc, char **argv)
{
    // flags for both short and long arguments
    static int version_flag;    // argument 'version'
    static int V_flag;          // argument 'V'
    static int help_flag;       // argument 'help'
    static int h_flag;          // argument 'h'
    static int verbose_flag;    // argument 'verbose'
    static int v_flag;          // argument 'v'

    int c;              // deal with getopt_long()
    int option_index;

    opterr = 0;         // force getopt_long() to remain silent when it finds a problem

    static struct option long_options[] =
            {
                    /* These options set a flag. */
                    {"help",    no_argument, &help_flag,    1},
                    {"version", no_argument, &version_flag, 1},
                    {"verbose", no_argument, &verbose_flag, 1},
                    /* These options don’t set a flag.
                       We distinguish them by their indices. */
                    {"",        no_argument, &h_flag, (int) 'h'}, // help
                    {"",        no_argument, &V_flag, (int) 'V'}, // version
                    {"",        no_argument, &v_flag, (int) 'v'}, // verbose
                    {"number",  required_argument, 0,       'n'}, // number (integer)
                    {"fnumber", required_argument, 0,       'd'}, // number (float)
                    {0, 0, 0, 0}
            };

    while (1)
    {
        /* getopt_long() stores the option index here. */
        option_index = 0;

        c = getopt_long(argc, argv, ":hVvn:d:", long_options, &option_index);

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

            case 'v':
                v_flag = 1;
                break;

            case 'n':   // needs an integer argument
                try
                {
                    std::cout << std::stoi((std::string) optarg) << "\n";//for test
                }
                catch (const std::invalid_argument& ia)
                {
                    std::cerr << "Option 'n' has an invalid argument.\n";
                }
                break;
    
            case 'd':   // needs a float argument
                try
                {
                    std::cout << std::stof((std::string) optarg) << "\n";//for test
                }
                catch (const std::invalid_argument& ia)
                {
                    std::cerr << "Option 'd' has an invalid argument.\n";
                }
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

    if (help_flag || h_flag)
        Messages::help();

    if (version_flag || V_flag)
        Messages::version();

    if (verbose_flag || v_flag)
        Messages::verbose();

    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        std::cerr << "non-option ARGV-element(s): ";
        while (optind < argc)
            std::cerr << argv[ optind++ ] << " ";
        std::cerr << std::endl;
    }
}
