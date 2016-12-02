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
#include <unordered_set>

#endif

#include "def.h"
#include "functions.h"
#include "FCM.h"
#include "messages.h"


///////////////////////////////////////////////////////////
/////////                 M A I N                 /////////
///////////////////////////////////////////////////////////
namespace std
{
    template<typename T, size_t N>
    struct hash<array<T, N> >
    {
        typedef array<T, N> argument_type;
        typedef size_t result_type;
        
        result_type operator()(const argument_type& a) const
        {
            hash<T> hasher;
            result_type h = 0;
            for (result_type i = 0; i < N; ++i)
            {
                h = h * 31 + hasher(a[i]);
            }
            return h;
        }
    };
}

std::unordered_set< std::array< int, 16 > > closelist;

typedef std::unordered_map< std::array< int, 16 >, std::array<uint16_t, ALPHABET_SIZE> > htable_ta;

int32_t main (int argc, char *argv[])
{
    // Record start time
    highResClock::time_point exeStartTime = highResClock::now();
    

    
    // for access to Functions (object 'function' on memory stack)
    Functions function;
    function.commandLineParser(argc, argv); // parse the command line
    
    
//    std::string s="012";
//    int i=std::stoi(s);
//    std::cout<<i;
    std::array<int, 16> sn = {1,2,3,4,5,6,0,8,9,10,11,12,13,14,7,15};
    closelist.insert(sn);
    
    
    
//    std::vector< int > v(4, 0);
////    std::vector< int > v2;
////    v2.reserve(1);
////    v2.push_back(1);
////    v.insert(v.end(), v2.begin(), v2.end());
////    std::array<int,1> a;
////
////    std::string s="ACT";
////    std::cout<<"\n"<<s.size();
////
//    std::array<int, 1> a2 ={1};
////    std::array<int, 1> a3={2};
//
//    v.insert(v.end(), a2.begin(), a2.end());
    
    
    
    
    // Record end time
    highResClock::time_point exeFinishTime = highResClock::now();

    // calculate and show duration in seconds
    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
    std::cout << "\nElapsed time: " << elapsed.count() << " s\n";

    
    return 0;
}
