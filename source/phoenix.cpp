#include <iostream>
#include <getopt.h>
#include <cstring>
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


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////

int32_t main (int argc, char *argv[])
{
    Functions::commandLineParser(argc, argv);
    
    
    return 0;
}