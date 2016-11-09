#include "messages.h"
#include "def.h"

#include <iostream>


////  shows usage guide ////////////////////////////////////////////
void PrintUsageGuide (void)
{
    std::cout
            << "Synopsis:"                                  << "\n"
            << "    phoenix [OPTION]... [FILE1] [FILE2]"    << "\n"
                                                            << "\n"
            << "Options:"                                   << "\n"
            << "    -h,  --help"                            << "\n"
            << "         usage guide"                       << "\n"
                                                            << "\n"
            << "    -V,  --version"                         << "\n"
            << "         version number"                    << "\n"
                                                            << "\n"
            << "    -v,  --verbose"                         << "\n"
            << "         verbose mode (more information)"   << "\n"
                                                            << "\n"
            << "    -n [NUMBER],  --number [NUMBER]"        << "\n"
            << "         number of something."              << "\n"
            << "         requires a number (NUMBER) between 1 and 100" << "\n"
                                                            << "\n"
            << "    [FILE1]"                                << "\n"
            << "         reference file"                    << "\n"
                                                            << "\n"
            << "    [FILE2]"                                << "\n"
            << "         target file"                       << "\n";
}

////  shows version number ////////////////////////////////////////////
void PrintVersion(void)
{
    std::cout
                                                                                      <<"\n"
            <<"              ===================="                                    <<"\n"
            <<"              ==  PHOENIX v" << VERSION << "." << RELEASE << "  =="    <<"\n"
            <<"              ===================="                                    <<"\n";
}
