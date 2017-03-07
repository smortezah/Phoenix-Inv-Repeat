#ifndef PHOENIX_FUNCTIONS_H
#define PHOENIX_FUNCTIONS_H


#include <iostream>

#include "def.h"
#include "FCM.h"

class Functions
{
public:
    Functions ();                           /// constructor
    
    void commandLineParser (int, char**, FCM&);   /// parse command line
    
    //todo checkEqualFiles(f1, f2);
    
//    static bool isFileCorrect (std::ifstream&); /// check if file opened correctly
    
//    bool getDecompressFlag () const;        /// getter of decompress flag
//    void setDecompressFlag (bool);          /// getter of decompress flag
    
private:
//    bool decompressFlag;                    /// decompress flag
};


#endif //PHOENIX_FUNCTIONS_H
