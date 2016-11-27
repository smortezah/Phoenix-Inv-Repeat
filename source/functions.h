#ifndef PHOENIX_FUNCTIONS_H
#define PHOENIX_FUNCTIONS_H


#include <iostream>


class Functions
{
public:
    Functions ();                                   // constructor
    int32_t commandLineParser (int, char**);        // parse command line

private:
    std::ifstream fileOpen(std::string);    // open file
};


#endif //PHOENIX_FUNCTIONS_H
