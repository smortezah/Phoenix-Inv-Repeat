#include "messages.h"
#include "def.h"

#include <iostream>


/***********************************************************
    constructor
************************************************************/
Messages::Messages () {}


/***********************************************************
    shows usage guide
************************************************************/
void Messages::help (void)
{
    std::cout
            << "Synopsis:\n"
            << "    phoenix [OPTION]... [FILE1] [FILE2]\n"
            << "\n"
            << "Options:\n"
            << "    -h,  --help\n"
            << "         usage guide\n"
            << "\n"
            << "    -V,  --version\n"
            << "         version number\n"
            << "\n"
            << "    -v,  --verbose\n"
            << "         verbose mode (more information)\n"
            << "\n"
            << "    -n [NUMBER],  --number [NUMBER]\n"
            << "         number of something.\n"
            << "         requires an integer number (NUMBER)\n"
            << "\n"
            << "    -d [NUMBER],  --fnumber [NUMBER]\n"
            << "         number of something.\n"
            << "         requires a float number (NUMBER)\n"
            << "\n"
            << "    [FILE1]\n"
            << "         reference file\n"
            << "\n"
            << "    [FILE2]\n"
            << "         target file\n";
}


/***********************************************************
    shows version number
************************************************************/
void Messages::version (void)
{
    std::cout
            <<"\n"
            <<"              ====================\n"
            <<"              ==  PHOENIX v" << VERSION << "." << RELEASE << "  ==\n"
            <<"              ====================\n";
}


/***********************************************************
    shows verbose mode
************************************************************/
void Messages::verbose (void)
{
    std::cout << "verbose mode.\n"; // for test
}
