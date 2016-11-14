#ifndef PHOENIX_FUNCTIONS_H
#define PHOENIX_FUNCTIONS_H

#include <iostream>


class functions
{
public:
    functions ();                                   // constructor
    static int32_t commandLineParser (int, char**); // command line parser

private:
    static int argumentNumberDetector (const char &, const std::string);
};


#endif //PHOENIX_FUNCTIONS_H
