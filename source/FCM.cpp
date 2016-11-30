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
    std::string fileName = getFileAddress();        // get file address
    bool isInvertedRepeat = getInvertedRepeat();    // get inverted repeat
    
    Functions funcObj;  // object of class Functions
    
    std::ifstream fileIn(fileName, std::ios::in);           // open file located in fileName
    bool isFileOk = funcObj.fileOpenErrorHandle(fileName);  // check if file is opened correctly
    
    if (isFileOk)   // file opened correctly
    {
        std::string context(CONTEXT_DEPTH, 'A');    // context, that slides in the dataset
        
        hashTable_t hTable;                         // create hash table
        hTable.insert({context, {0, 0, 0, 0, 0}});  // initialize hash table with 0'z
        
        std::string datasetLine;                    // to keep each line of file
        std::getline(fileIn, datasetLine);          // read first line of file
        datasetLine = context + datasetLine;        // add "AA..." to beginning of first line of file
        
        // iterator for each line of file.
        // at first line, it starts from index CONTEXT_DEPTH. at other lines, it starts from index 0
        size_t lineIter = CONTEXT_DEPTH;
        do
        {
            // fill hash table by number of occurrences of symbols A, C, T, G, N
            for (size_t i = lineIter; i != datasetLine.size(); ++i)
            {
                switch (datasetLine[ i ])
                {
                    case 'A':
                        // increment number of 'A's. order: {A, C, T, G, N}
                        ++(hTable[ context ])[ 0 ];
                        
                        // considering inverted repeats to update hash table
                        if (isInvertedRepeat)
                        {
                            std::string invRepeat = context + "A";
                            
                            // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                            for (char &ch : invRepeat)
                                ch = (ch == 'A') ? 'T' :
                                     (ch == 'C') ? 'G' :
                                     (ch == 'T') ? 'A' :
                                     (ch == 'G') ? 'C' :
                                     'N';
                            
                            // reverse the string
                            std::reverse( invRepeat.begin(), invRepeat.end() );
                            
                            // inverted repeat context
                            std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
                            
                            // update hash table for inverted repeats
                            switch (invRepeat[ invRepeat.size() - 1 ])
                            {
                                case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                                case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                                case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                                case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                                case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                                default:                                            break;
                            }
                        }

//                        counters[ 0 ] += ALPHA_DENUMERATOR * table[ index ][ 0 ] + ALPHA_NUMERATOR;
//                        totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                           table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                                     (4 * ALPHA_NUMERATOR);
////                        AESym(3, counters, totalCount, writer);
                        break;
                    
                    case 'C':
                        // increment number of 'C's. order: {A, C, T, G, N}
                        ++(hTable[ context ])[ 1 ];
                        
                        // considering inverted repeats to update hash table
                        if (isInvertedRepeat)
                        {
                            std::string invRepeat = context + "C";
                            
                            // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                            for (char &ch : invRepeat)
                                ch = (ch == 'A') ? 'T' :
                                     (ch == 'C') ? 'G' :
                                     (ch == 'T') ? 'A' :
                                     (ch == 'G') ? 'C' :
                                     'N';
                            
                            // reverse the string
                            std::reverse( invRepeat.begin(), invRepeat.end() );
                            
                            // inverted repeat context
                            std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
                            
                            // update hash table for inverted repeats
                            switch (invRepeat[ invRepeat.size() - 1 ])
                            {
                                case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                                case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                                case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                                case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                                case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                                default:                                            break;
                            }
                        }

//                        counters[ 1 ] += ALPHA_DENUMERATOR * table[ index ][ 1 ] + ALPHA_NUMERATOR;
//                        totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                           table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                                     (4 * ALPHA_NUMERATOR);
////                        AESym(3, counters, totalCount, writer);
                        break;
                    
                    case 'T':
                        // increment number of 'T's. order: {A, C, T, G, N}
                        ++(hTable[ context ])[ 2 ];
                        
                        // considering inverted repeats to update hash table
                        if (isInvertedRepeat)
                        {
                            std::string invRepeat = context + "T";
                            
                            // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                            for (char &ch : invRepeat)
                                ch = (ch == 'A') ? 'T' :
                                     (ch == 'C') ? 'G' :
                                     (ch == 'T') ? 'A' :
                                     (ch == 'G') ? 'C' :
                                     'N';
                            
                            // reverse the string
                            std::reverse( invRepeat.begin(), invRepeat.end() );
                            
                            // inverted repeat context
                            std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
                            
                            // update hash table for inverted repeats
                            switch (invRepeat[ invRepeat.size() - 1 ])
                            {
                                case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                                case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                                case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                                case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                                case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                                default:                                            break;
                            }
                        }

//                        counters[ 2 ] += ALPHA_DENUMERATOR * table[ index ][ 2 ] + ALPHA_NUMERATOR;
//                        totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                           table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                                     (4 * ALPHA_NUMERATOR);
////                        AESym(3, counters, totalCount, writer);
                        break;
                    
                    case 'G':
                        // increment number of 'G's. order: {A, C, T, G, N}
                        ++(hTable[ context ])[ 3 ];
                        
                        // considering inverted repeats to update hash table
                        if (isInvertedRepeat)
                        {
                            std::string invRepeat = context + "G";
                            
                            // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                            for (char &ch : invRepeat)
                                ch = (ch == 'A') ? 'T' :
                                     (ch == 'C') ? 'G' :
                                     (ch == 'T') ? 'A' :
                                     (ch == 'G') ? 'C' :
                                     'N';
                            
                            // reverse the string
                            std::reverse( invRepeat.begin(), invRepeat.end() );
                            
                            // inverted repeat context
                            std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
                            
                            // update hash table for inverted repeats
                            switch (invRepeat[ invRepeat.size() - 1 ])
                            {
                                case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                                case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                                case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                                case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                                case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                                default:                                            break;
                            }
                        }
//
//                        counters[ 3 ] += ALPHA_DENUMERATOR * table[ index ][ 3 ] + ALPHA_NUMERATOR;
//                        totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                           table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                                     (4 * ALPHA_NUMERATOR);
////                        AESym(3, counters, totalCount, writer);
                        break;
                    
                    case 'N':
                        // increment number of 'N's. order: {A, C, T, G, N}
                        ++(hTable[ context ])[ 4 ];
                        
                        // considering inverted repeats to update hash table
                        if (isInvertedRepeat)
                        {
                            std::string invRepeat = context + "N";
                            
                            // A <-> T  ,  C <-> G  ,  N <-> N (N unchanged)
                            for (char &ch : invRepeat)
                                ch = (ch == 'A') ? 'T' :
                                     (ch == 'C') ? 'G' :
                                     (ch == 'T') ? 'A' :
                                     (ch == 'G') ? 'C' :
                                     'N';
                            
                            // reverse the string
                            std::reverse( invRepeat.begin(), invRepeat.end() );
                            
                            // inverted repeat context
                            std::string invRepeatContext = invRepeat.substr(0, invRepeat.size() - 1);
                            
                            // update hash table for inverted repeats
                            switch (invRepeat[ invRepeat.size() - 1 ])
                            {
                                case 'A':   ++(hTable[ invRepeatContext ])[ 0 ];    break;
                                case 'C':   ++(hTable[ invRepeatContext ])[ 1 ];    break;
                                case 'T':   ++(hTable[ invRepeatContext ])[ 2 ];    break;
                                case 'G':   ++(hTable[ invRepeatContext ])[ 3 ];    break;
                                case 'N':   ++(hTable[ invRepeatContext ])[ 4 ];    break;
                                default:                                            break;
                            }
                        }

//                        counters[ 3 ] += ALPHA_DENUMERATOR * table[ index ][ 3 ] + ALPHA_NUMERATOR;
//                        totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                           table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                                     (4 * ALPHA_NUMERATOR);
////                        AESym(3, counters, totalCount, writer);
                        break;
                    
                    default: break;
                }

//                totalCount = (ALPHA_DENUMERATOR * (table[ index ][ 0 ] + table[ index ][ 1 ] +
//                                                   table[ index ][ 2 ] + table[ index ][ 3 ])) +
//                             (4 * ALPHA_NUMERATOR);
//
//                memset(counters, 0, sizeof(counters[ 0 ]) * 4);
                
                // update context
                context = (CONTEXT_DEPTH == 1)
                          ? std::string("") + datasetLine[ i ]
                          : context.substr(1, CONTEXT_DEPTH - 1) + datasetLine[ i ];
            }
            
            lineIter = 0;   // iterator for non-first lines of file becomes 0
        } while ( std::getline(fileIn, datasetLine) );
        
        fileIn.close();     // close file
        
//        return hTable;      // return the made hash table
        FCM::hashTable = hTable;
    }
}


/***********************************************************
    print hash table
************************************************************/
void FCM::printHashTable (hashTable_t hTable)
{
    
    /***********************************************************
        test
    ************************************************************/
    std::cout << "\tA\tC\tT\tG\tN"
              //              << "\tP_A\tP_C\tP_T\tP_G\tP_N"
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
