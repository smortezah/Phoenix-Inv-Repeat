#include <iostream>
#include <fstream>

#include "messages.h"
#include "def.h"

using std::cout;


/***********************************************************
    constructor
************************************************************/
Messages::Messages () {}


/***********************************************************
    show usage guide
************************************************************/
void Messages::help () const
{
    cout                                                                                            << '\n'
         << "Synopsis:"                                                                             << '\n'
         << "    ./phoenix [OPTION]... -m [MODEL(S)] -r [REFERENCE(S)] -t [TARGET(S)]"              << '\n'
                                                                                                    << '\n'
         << "         EXAMPLE: ./phoenix -n 4 -m 0,11,100:1,9,10 -r HS21,HSY -t PT19,PT21,PT22,PTY" << '\n'
                                                                                                    << '\n'
         << "Mandatory arguments:"                                                                  << '\n'
         << "    -m [<i_1>,<k_1>,<a_1>:<i_2>,<k_2>,<a_2>:...],   --model [<i_1>,<k_1>,<a_1>:...]"   << '\n'
         << "         context model(s) (at least one)."                                             << '\n'
         << "         <i>:  inverted repeat (0=don't consider, 1=consider)."                        << '\n'
         << "         <k>:  context-order size,"                                                    << '\n'
         << "         <a>:  1/alpha,"                                                               << '\n'
         << "         EXAMPLE: -m 0,11,100:1,9,10"                                                  << '\n'
                                                                                                    << '\n'
         << "    -r [REFERENCE(S)],   --reference [REFERENCE(S)]"                                   << '\n'
         << "         reference file(s) address(es)"                                                << '\n'
         << "         EXAMPLE: -r ref1,ref2"                                                        << '\n'
                                                                                                    << '\n'
         << "    -t [TARGET(S)],   --target [TARGET(S)]"                                            << '\n'
         << "         target file(s) address(es)"                                                   << '\n'
         << "         EXAMPLE: -t tar1,tar2,tar3"                                                   << '\n'
                                                                                                    << '\n'
         << "Options:"                                                                              << '\n'
         << "    -h,  --help"                                                                       << '\n'
         << "         usage guide"                                                                  << '\n'
                                                                                                    << '\n'
         << "    -n [NUMBER (integer)],  --n_threads [NUMBER (integer)]"                            << '\n'
         << "         number of threads (default: 2)."                                              << '\n'
         << "         requires an integer number (n >= 1)"                                          << '\n'
                                                                                                    << '\n'
         << "    -g [NUMBER (float)],  --gamma [NUMBER (float)]"                                    << '\n'
         << "         gamma (default: 0.95)."                                                       << '\n'
         << "         requires a float number (0 <= g < 1)"                                         << '\n'
                                                                                                    << '\n'
         << "    -V,  --version"                                                                    << '\n'
         << "         version number"                                                               << '\n'
                                                                                                    << '\n'
         << "    -v,  --verbose"                                                                    << '\n'
         << "         verbose mode (more information)"                                              << '\n'
                                                                                                    << '\n';
}


/***********************************************************
    show About Phoenix
************************************************************/
void Messages::about () const
{
    cout                                                                 << '\n'
         << "PHOENIX v" << VERSION_PHOENIX << "." << RELEASE_PHOENIX     << '\n'
         << "================"                                           << '\n'
         << "A compression-based method"                                 << '\n'
                                                                         << '\n'
         << "Morteza Hosseini, Diogo Pratas, Armando J. Pinho"           << '\n'
         << "Copyright (C) 2016-2017 University of Aveiro"               << '\n'
                                                                         << '\n'
         << "This is a Free software, under GPLv3. You may redistribute" << '\n'
         << "copies of it under the terms of the GNU - General Public"   << '\n'
         << "License v3 <http://www.gnu.org/licenses/gpl.html>. There"   << '\n'
         << "is NOT ANY WARRANTY, to the extent permitted by law."       << '\n'
                                                                         << '\n';
}


/***********************************************************
    show verbose mode
************************************************************/
void Messages::verbose () const
{
    cout << "verbose mode.\n"; /// TODO for test
}
