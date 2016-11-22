#ifndef PHOENIX_FUNCTIONS_H
#define PHOENIX_FUNCTIONS_H

#include <iostream>


class Functions
{
public:
    Functions ();                                   // constructor
    static int32_t commandLineParser (int, char**); // command line parser

private:
    static std::string fileRead (std::string); // reads a file
    int  a;
};


#endif //PHOENIX_FUNCTIONS_H
