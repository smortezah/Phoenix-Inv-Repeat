#include <iostream>
#include <fstream>
#include <cstring>
//#include <getopt.h>     // for command line parsing
#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#if defined(_MSC_VER)
#include <io.h>
#else
#include <unistd.h>
#endif

#include "def.h"
#include "messages.h"
#include "functions.h"
#include "hash.h"



///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////

int32_t main (int argc, char *argv[])
{
//    Functions function; // for access to Functions (object 'function' on memory stack)
//    function.commandLineParser(argc, argv);
    std::string s="atntcaggn";
    std::cout<<s<<"\n";
    
//    for (int i = 0; i < s.size(); ++i)
//    {
//        (s[ i ] == 'a') ? (s[ i ] = 't') :
//        (
//                (s[ i ] == 'c') ? (s[ i ] = 'g') :
//                (
//                        (s[ i ] == 't') ? (s[ i ] = 'a') :
//                        (
//                                (s[ i ] == 'g') ? (s[ i ] = 'c') :
//                                true
//                        )
//                )
//        );
//    }
    
//    for (int i = 0; i < s.size(); ++i)
//    {
//        if (s[ i ] == 'a')  s[ i ] = 't';
//    }
    
    for (int i = 0; i < s.size(); ++i)
//    {
        (s[ i ] == 'a') ? (s[ i ] = 't')
                        : ((s[ i ] == 'c') ? (s[ i ] = 'g')
                                           : ((s[ i ] == 't') ? (s[ i ] = 'a')
                                                              : ((s[ i ] == 'g') ? (s[ i ] = 'c')
                                                                                 : true)));
//    }

    std::cout<<s<<"\n";
    
    return 0;
}