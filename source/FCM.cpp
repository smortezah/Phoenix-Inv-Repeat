#include <iostream>
#include <fstream>
#include <algorithm>

#include "FCM.h"
#include "functions.h"


/***********************************************************
    constructor
************************************************************/
FCM::FCM () {}


/***********************************************************
    build hash table
************************************************************/
void FCM::buildHashTable ()
{
    uint8_t contextDepth    = getContextDepth();    // get context depth
    uint32_t alphaDen       = getAlphaDenom();      // get alpha denominator
    bool isInvertedRepeat   = getInvertedRepeat();  // get inverted repeat
    std::string fileName    = getFileAddress();     // get file address
    
    std::ifstream fileIn(fileName, std::ios::in);   // open file located in fileName
    
    if (Functions::isfileCorrect(fileName))         // file opened correctly
    {
        std::string context(contextDepth, 'A');     // context, that slides in the dataset
        
        hashTable_t hTable;                         // create hash table
        hTable.insert({context, {0, 0, 0, 0, 0}});  // initialize hash table with 0'z
        
        std::string datasetLine;                    // to keep each line of file
        std::getline(fileIn, datasetLine);          // read first line of file
        datasetLine = context + datasetLine;        // add "AA..." to beginning of first line of file
        
        // iterator for each line of file.
        // at first line, it starts from index "contextDepth". at other lines, it starts from index 0
        size_t lineIter = contextDepth;
        do
        {
            // fill hash table by number of occurrences of symbols A, C, G, T, N
            for (; lineIter != datasetLine.size(); ++lineIter)
            {
                // TODO
                // tu DEFINE tarif kon
                char c = datasetLine[ lineIter ];
                uint8_t i = (c == 'A') ? (uint8_t) 0 :
                            (c == 'C') ? (uint8_t) 1 :
                            (c == 'G') ? (uint8_t) 2 :
                            (c == 'T') ? (uint8_t) 3 : (uint8_t) 4;
//
                ++(hTable[ context ])[ i ];
//
//                (c == 'A') ? ++(hTable[ context ])[ 0 ] :
//                (c == 'C') ? ++(hTable[ context ])[ 1 ] :
//                (c == 'G') ? ++(hTable[ context ])[ 2 ] :
//                (c == 'T') ? ++(hTable[ context ])[ 3 ] :
//                ++(hTable[ context ])[ 4 ];

                
//                switch (datasetLine[ lineIter ])
//                {
//                    case 'A':   ++(hTable[ context ])[ 0 ]; break;
//                    case 'C':   ++(hTable[ context ])[ 1 ]; break;
//                    case 'T':   ++(hTable[ context ])[ 2 ]; break;
//                    case 'G':   ++(hTable[ context ])[ 3 ]; break;
//                    case 'N':   ++(hTable[ context ])[ 4 ]; break;
//                    default:                                break;
//                }
    
                // considering inverted repeats to update hash table
                if (isInvertedRepeat)
                {
                    std::string invRepeat = context + datasetLine[ lineIter ];
        
                    // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                    for (char &ch : invRepeat)
                        ch = (ch == 'A') ? 'T' :
                             (ch == 'C') ? 'G' :
                             (ch == 'G') ? 'C' :
                             (ch == 'T') ? 'A' :
                             'N';
        
                    // reverse the string
                    std::reverse( invRepeat.begin(), invRepeat.end() );
        
                    // inverted repeat context
                    std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
        
//                    // update hash table for inverted repeats
//                    switch (invRepeat[ invRepeat.size() - 1 ])
//                    {
//                        case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
//                        case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
//                        case 'G':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
//                        case 'T':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
//                        case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
//                        default:                                            break;
//                    }
    
                    char c2 = invRepeat[ invRepeat.size() - 1 ];
                    uint8_t r = (c2 == 'A') ? (uint8_t) 0 :
                                (c2 == 'C') ? (uint8_t) 1 :
                                (c2 == 'G') ? (uint8_t) 2 :
                                (c2 == 'T') ? (uint8_t) 3 : (uint8_t) 4;
                    
                    ++(hTable[ invRepeatContext ])[ r ];
                }
                
                // update context
                context = (contextDepth == 1)
                          ? std::string("") + datasetLine[ lineIter ]
                          : context.substr(1, contextDepth - 1) + datasetLine[ lineIter ];
            }
            
            lineIter = 0;           // iterator for non-first lines of file becomes 0
        } while ( std::getline(fileIn, datasetLine) );
        
        fileIn.close();             // close file
        
        FCM::setHashTable(hTable);  // save the built hash table
    }
}


/***********************************************************
    print hash table
************************************************************/
void FCM::printHashTable (hashTable_t hTable) const
{
    
    /***********************************************************
        test
    ************************************************************/
    std::cout << "\tA\tC\tG\tT\tN"
              //              << "\tP_A\tP_C\tP_G\tP_T\tP_N"
              << "\n"
              << "\t-----------------------------------"
              //              << "------------------------------------------"
              << "\n";
    
    int sum;
    int alpha = 1;
    
    for (hashTable_t::iterator it = hTable.begin(); it != hTable.end(); ++it)
    {
        sum = 0;
        std::cout << it->first << "\t";
        for (int i : it->second)
        {
            std::cout << i << "\t";
            sum += i;
        }

//        for (int i = 0; i < 5; ++i)
//        {
//            std::cout << std::fixed << std::setprecision(1)
//                      << (float) (it->second[ i ] + alpha) /
//                         (sum + ALPHABET_SIZE * alpha) << "\t";
//        }
        std::cout << "\n";
    }
}


/***********************************************************
    getters and setters
************************************************************/
uint8_t FCM::getContextDepth () const { return contextDepth; }
void FCM::setContextDepth (uint8_t ctxDp) { FCM::contextDepth = ctxDp; }
uint32_t FCM::getAlphaDenom () const { return alphaDenom; }
void FCM::setAlphaDenom (uint32_t alphaDen) { FCM::alphaDenom = alphaDen; }
bool FCM::getInvertedRepeat () const { return invertedRepeat; }
void FCM::setInvertedRepeat (bool invRep) { FCM::invertedRepeat = invRep; }
const hashTable_t &FCM::getHashTable () const { return hashTable; }
void FCM::setHashTable (const hashTable_t &hT) { FCM::hashTable = hT; }
const std::string &FCM::getFileAddress () const { return fileAddress; }
void FCM::setFileAddress (const std::string &fA) { FCM::fileAddress = fA; }
