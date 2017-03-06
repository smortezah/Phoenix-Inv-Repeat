#ifndef PHOENIX_FUNCTIONS_H
#define PHOENIX_FUNCTIONS_H


#include <iostream>


class Functions
{
public:
    Functions ();                               /// constructor
    
    void commandLineParser (int, char**);       /// parse command line
    
    //todo checkEqualFiles(f1, f2);
    
//    static bool isFileCorrect (std::ifstream&); /// check if file opened correctly
};


#endif //PHOENIX_FUNCTIONS_H