#ifndef PHOENIX_FUNCTIONS_H
#define PHOENIX_FUNCTIONS_H


#include <iostream>


class Functions
{
public:
    Functions ();                               // constructor
    
    int8_t commandLineParser (int, char**);     // parse command line
    bool isfileCorrect (const std::string &);   // check if file opened correctly
};


#endif //PHOENIX_FUNCTIONS_H