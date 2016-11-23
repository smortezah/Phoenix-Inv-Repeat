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
    Functions function; // for access to Functions (object 'function' on memory stack)
    function.commandLineParser(argc, argv);
    
    
    return 0;
}