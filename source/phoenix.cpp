#include <iostream>
#include <getopt.h>

#include "messages.h"


/* Flag set by ‘--verbose’. */
static int something_flag;

////  command line parser ////////////////////////////////////////////
static int32_t CommandLineParser (int argc, char **argv)
{
    int c;
    int option_index;
    
    opterr = 0; // force getopt_long() to remain silent when it finds a problem
    
    static struct option long_options[] =
            {
                    /* These options set a flag. */
                    {"something", no_argument,       &something_flag, 1}, // for long-only options
                    /* These options don’t set a flag.
                       We distinguish them by their indices. */
                    {"help",      no_argument,       0,               'h'},
                    {"version",   no_argument,       0,               'V'},
                    {"verbose",   no_argument,       0,               'v'},
                    {"number",    required_argument, 0,               'n'},
                    {0, 0,                           0,               0}
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
            
            case 'h':
                PrintUsageGuide();
                break;
            
            case 'V':
                PrintVersion();
                break;
            
            case 'v':
                break;
            
            case 'n':
                if (optarg[ 0 ] == '-')
                    std::cout << "Option 'n' requires an argument.\n";
                else
                    std::cout << "Argument of 'n' is " << optarg << std::endl; // for test
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
    
    /* Instead of reporting ‘--verbose' as they are encountered,
       we report the final status resulting from them. */
    if (something_flag)
        std::cout << "something flag is set\n";
    
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