#include <iostream>
#include <fstream>
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
#include "functions.h"

using std::cout;
using std::chrono::high_resolution_clock;


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////
#include <iomanip>      // setw, setprecision
#include <fstream>

int32_t main (int argc, char *argv[])
{
    // Record start time
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();

    // for access to Functions (object 'function' on memory stack)
    Functions function;
    function.commandLineParser(argc, argv); // parse the command line

//    string s1="ac";
//    string s2="ntac";
//    string::iterator i1=s1.begin();
//    string::iterator i2=s2.begin();
////    cout<<*i2;
//    s2="qwe";
//    cout<<*i2;
    
//    int i=0;
//    char c;
//    std::cin>>c;
//    switch (c)
//    {
//        case 'c':
//            i+=5*5*5;
//            break;
//        case 'n':
//            i+=2*5*5*5;
//            break;
//    }
//    cout<<i;
    
//    array<int,2> a={4,6};
//    for (array<int,2>::iterator i = a.begin(); i !=a.end(); ++i)
//    {
//        cout<<*(i);
//    }
    
    
//    uint16_t alphaDen=1;
//    uint64_t nSym=4;
//    uint64_t sumNSyms=13;
//    double probability = (double) ((uint64_t) alphaDen*nSym + (uint64_t)alphaDen) / (double) ((uint64_t) alphaDen*sumNSyms + (uint64_t) ALPHABET_SIZE);
//    std::cout << std::fixed;
//    std::cout << std::setprecision(15);
//    cout<<probability;
    
    
    
    // Record end time
    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now();

    // calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    cout
//            << '\n'
            << '\t'
//            << "Elapsed time: "
            << elapsed.count()
//            << " s"
            << '\n'
            ;
    
    
    return 0;
}
