#include "messages.h"

#include <iostream>
#include <fstream>

#include "def.h"


/***********************************************************
    constructor
************************************************************/
Messages::Messages () {}


/***********************************************************
    show usage guide
************************************************************/
void Messages::help (void)
{
    std::cout                                                       << "\n"
            << "Synopsis:"                                          << "\n"
            << "    phoenix [OPTION]... -t [TARGET] -r [REFERENCE]" << "\n"
                                                                    << "\n"
            << "Options:"                                           << "\n"
            << "    -h,  --help"                                    << "\n"
            << "         usage guide"                               << "\n"
                                                                    << "\n"
            << "    -V,  --version"                                 << "\n"
            << "         version number"                            << "\n"
                                                                    << "\n"
            << "    -v,  --verbose"                                 << "\n"
            << "         verbose mode (more information)"           << "\n"
                                                                    << "\n"
            << "    -i,  --inverted_repeat"                         << "\n"
            << "         considers inverted repeats"                << "\n"
                                                                    << "\n"
            << "    -n [NUMBER],  --number [NUMBER]"                << "\n"
            << "         number of something."                      << "\n"
            << "         requires an integer number (NUMBER)"       << "\n"
                                                                    << "\n"
            << "    -d [NUMBER],  --fnumber [NUMBER]"               << "\n"
            << "         number of something."                      << "\n"
            << "         requires a float number (NUMBER)"          << "\n"
                                                                    << "\n"
            << "    -t [TARGET],  --target [TARGET]"                << "\n"
            << "         target file"                               << "\n"
                                                                    << "\n"
            << "    -r [REFERENCE],  --reference [REFERENCE]"       << "\n"
            << "         reference file"                            << "\n"
                                                                    << "\n";
}


/***********************************************************
    show About Phoenix
************************************************************/
void Messages::about (void)
{
    std::cout                                                               << "\n"
            << "PHOENIX v" << VERSION_PHOENIX << "." << RELEASE_PHOENIX     << "\n"
            << "================"                                           << "\n"
            << "A compression-based method"                                 << "\n"
                                                                            << "\n"
            << "Morteza Hosseini, Diogo Pratas"                             << "\n"
            << "Copyright (C) 2016 University of Aveiro"                    << "\n"
                                                                            << "\n"
            << "This is a Free software, under GPLv3. You may redistribute" << "\n"
            << "copies of it under the terms of the GNU - General Public"   << "\n"
            << "License v3 <http://www.gnu.org/licenses/gpl.html>. There"   << "\n"
            << "is NOT ANY WARRANTY, to the extent permitted by law."       << "\n"
                                                                            << "\n";
}


/***********************************************************
    show verbose mode
************************************************************/
void Messages::verbose (void)
{
    std::cout << "verbose mode.\n"; // for test
}


/***********************************************************
    show inverted_repeat mode
************************************************************/
void Messages::inverted_repeat (void)
{
    std::cout << "inverted repeat mode.\n"; // for test
}


/***********************************************************
    show integer number mode
************************************************************/
void Messages::number (int num)
{
    std::cout << "integer number mode (n=" << num << ")\n"; // for test
}


/***********************************************************
    show float number mode
************************************************************/
void Messages::fnumber (float num)
{
    std::cout << "float number mode (d=" << num << ")\n"; // for test
}
