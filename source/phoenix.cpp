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
#define FIXEDSIZE 3
template<typename T, std::size_t N>
class arrayHash {
public:
    std::size_t operator()(std::array<T, N> const &arr) const {
        std::size_t sum(0);
        for(auto &&i : arr) sum += std::hash<T>()(i);
        return sum;
    }
};





int32_t main (int argc, char *argv[])
{
    // Record start time
    high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();




    // for access to Functions (object 'function' on memory stack)
    Functions function;
//    function.commandLineParser(argc, argv); // parse the command line
    
    
    
    std::unordered_map<std::array<int, FIXEDSIZE>, int, arrayHash<int, FIXEDSIZE>> umap;
    std::array<int, FIXEDSIZE> arr {{1, 2, 3}};
    umap[arr] = 1;
    std::cout << umap[arr] << std::endl;
    
//    typedef unordered_map< array<uint16_t, 1>, int > htable_t;
//    htable_t ht;
//
////    ht.insert(1, 5);
//
//    for (htable_t::iterator it = ht.begin(); it != ht.end(); ++it)
//    {
//        cout << it->first << "\t";
//        for (int i : it->second)
//            cout << i << "\t";
//    }

    
    
    
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
