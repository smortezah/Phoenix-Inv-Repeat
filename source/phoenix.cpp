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
//#define FIXEDSIZE 20
//template<typename T, std::size_t N>
//class arrayHash {
//public:
//    std::size_t operator()(std::array<T, N> const &arr) const {
//        std::size_t sum(0);
//        for(auto &&i : arr) sum += std::hash<T>()(i);
//        return sum;
//    }
//};





int32_t main (int argc, char *argv[])
{
    // Record start time
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();

    // for access to Functions (object 'function' on memory stack)
    Functions function;
    function.commandLineParser(argc, argv); // parse the command line
    
//    uint8_t contextDepth=5;
//    uint8_t initContext[contextDepth];          // initial context = "00..."
//    std::memset(initContext,0,contextDepth);
//    for (uint8_t i:initContext) cout << (int)i;
    
//    int q;std::cin>>q;
//    int a[3] = {4, 5, 6};
////    for (int &i:a) i = (++i);
//    std::memmove(a,a+1,q);
//    for (int i:a) cout << i;
    
    
    
//    int a;std::cin>>a;
//    std::unordered_map< int*, std::array< int ,2>> umap;
////    std::array< int, FIXEDSIZE > arr{{1, 2, 3}};
//
//    int context[a];for(int &i:context)i=0;
//    for(int i:context)cout<<i;
////    std::array arr{{10,11}};
////    umap[context]=arr[0];
////    cout<<umap[context][1];
//
////    for(auto i:umap)
////    std::cout << umap[ arr ] << std::endl;
////
////
////
////    typedef unordered_map< uint8_t*, std::array<uint8_t ,5> > htable_t;
////    htable_t ht;
////
//////    ht.insert(1, 5);
////
////    for (htable_t::iterator it = ht.begin(); it != ht.end(); ++it)
////    {
////        cout << it->first << "\t";
////        for (int i : it->second)
////            cout << i << "\t";
////    }

    
    
    
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
