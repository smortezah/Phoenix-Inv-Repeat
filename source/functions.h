#ifndef PHOENIX_FUNCTIONS_H
#define PHOENIX_FUNCTIONS_H


#include <iostream>


class Functions
{
public:
    Functions ();                               // constructor
    int32_t commandLineParser (int, char**);    // parse command line

private:
    std::string fileRead (std::string);         // read a file
};


#endif //PHOENIX_FUNCTIONS_H
