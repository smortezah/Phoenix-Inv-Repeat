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
    Functions function; // for having access to functions (object 'function' on memory stack)
    function.commandLineParser(argc, argv);
    
//    Hash h1;
//
//    // open file
//    std::ifstream myFile("d.fa", std::ios::in);
//
//    if (!myFile)
//    {
//        std::cerr << "File \"" << "d.fa" << "\" could not be opened.\n";
////        return "";
//    }
//
//    std::string strEachLine;
//    std::string strDataset;
//
//    while (std::getline(myFile, strEachLine))
//        strDataset += strEachLine;
//
//    myFile.close();     // close file
//
//
//    std::unordered_map< std::string, std::array< int, 4> > hashTable = h1.hashTableBuild(strDataset);
//
//
//    /* show hash table */
//    // type definition: unordered_map iterator
//    typedef std::unordered_map< std::string, std::array< int, 4 > >::iterator umit;
//
//    int ind = 1;  // for test
//    std::cout << "\t\tA\tC\tT\tG\n"
//              << "        --------------------------------------\n";
//    for (umit it = hashTable.begin(); it != hashTable.end(); ++it)
//    {
//        std::cout << ind << ":\t" << it->first << "\t";
//        for (int i : it->second)    std::cout << i << "\t";
//        std::cout << "\n";
//        ++ind;
//    }
//    std::cout << "\nbucket size = " << hashTable.bucket_count() << "\n\n";  // for test
    
    
    return 0;
}