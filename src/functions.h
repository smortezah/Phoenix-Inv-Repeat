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
    
    U8   getN_threads () const;             /// getter of number of threads
    bool getDecompressFlag () const;        /// getter of decompress flag
    void setDecompressFlag (bool);          /// getter of decompress flag
    
private:
    U8   n_threads;                         /// number of threads
    bool decompressFlag;                    /// decompress flag
};


#endif //PHOENIX_FUNCTIONS_H
