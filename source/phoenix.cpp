#include <iostream>
#include <getopt.h>
#include <cstring>
#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#if defined(_MSC_VER)
#include <io.h>
#else
#include <unistd.h>
#endif
#include "messages.h"


static int version_flag;    // for argument 'version'
static int V_flag;          // for argument 'V'
static int help_flag;    // for argument 'help'
static int h_flag;          // for argument 'h'
static int verbose_flag;    // for argument 'verbose'
static int v_flag;          // for argument 'v'


////  command line parser ////////////////////////////////////////////
static int32_t CommandLineParser (int argc, char **argv)
{
    int c;
    int option_index;
    
    opterr = 0; // force getopt_long() to remain silent when it finds a problem
    
    static struct option long_options[] =
            {
                    /* These options set a flag. */
                    {"help",    no_argument, &help_flag,    1},
                    {"version", no_argument, &version_flag, 1},
                    {"verbose", no_argument, &verbose_flag, 1},
                    /* These options don’t set a flag.
                       We distinguish them by their indices. */
                    {"",        no_argument, &h_flag,   (int) 'h'}, // help
                    {"",        no_argument, &V_flag,   (int) 'V'}, // version
                    {"",        no_argument, &v_flag,   (int) 'v'}, // verbose
                    {"number",  no_argument, 0,         'n'},
                    {0, 0, 0, 0}
            };
    
    while (1)
    {
        /* getopt_long stores the option index here. */
        option_index = 0;
        
        c = getopt_long(argc, argv, ":hVvn:", long_options, &option_index);
        
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
    
            case 'n':   // needs an argument
                int numberOfDigits; // number of digits of the argument
                int optargSize;     // size of the argument

                numberOfDigits = 0;
                optargSize = (int) strlen(optarg);

                if (optarg[ 0 ] == '-')
                {
                    for (int i = 1; i < optargSize; ++i)
                        if (isdigit((int) optarg[ i ]))
                            ++numberOfDigits;

                    // argument is a negative number
                    if ( (optargSize > 1) && (numberOfDigits == (optargSize-1)) )
                        std::cout << "Argument of 'n' is " << optarg << ".\n";  // for test
                    else    // argument is not a number
                        std::cout << "Option 'n' has an invalid argument.\n";
                }
                else
                {
                    for (int i = 0; i < optargSize; ++i)
                        if ( isdigit((int) optarg[ i ]) )
                            ++numberOfDigits;

                    // argument is a positive number
                    if (numberOfDigits == optargSize)
                        std::cout << "Argument of 'n' is " << optarg << ".\n";  // for test
                    else    // argument is not a number
                        std::cout << "Option 'n' has an invalid argument.\n";
                }
                break;
    
            case ':':   /* missing option argument */
                std::cout << "Option '" << (char) optopt << "' requires an argument.\n";
                break;
    
            case '?':   /* invalid option */
            default:
                std::cout << "Option '" << (char) optopt << "' is invalid.\n";
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
        std::cout << "non-option ARGV-element(s): ";
        while (optind < argc)
            std::cout << argv[ optind++ ] << " ";
        std::cout << std::endl;
    }
}


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////

int32_t main (int argc, char *argv[])
{
    CommandLineParser(argc, argv);
    
    
    return 0;
}