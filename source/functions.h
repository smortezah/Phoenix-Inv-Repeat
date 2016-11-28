#ifndef PHOENIX_FUNCTIONS_H
#define PHOENIX_FUNCTIONS_H


#include <iostream>


class Functions
{
public:
    Functions ();                                   // constructor
    int8_t commandLineParser (int, char**);        // parse command line
    bool fileOpenErrorHandle(const std::string&);   // handle errors while opening file
};


#endif //PHOENIX_FUNCTIONS_H