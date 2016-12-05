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
//        std::string context(contextDepth, 'A');     // context, that slides in the dataset
        std::string context(contextDepth, '0');     // context, that slides in the dataset
    
        htable_t hTable;                            // create hash table
        hTable.insert({context, {0, 0, 0, 0, 0}});  // initialize hash table with 0'z

        std::string datasetLine;                    // to keep each line of file
        std::getline(fileIn, datasetLine);          // read first line of file
//        datasetLine = context + datasetLine;        // add "AA..." to beginning of first line of file

        // iterator for each line of file.
        // at first line, it starts from index "contextDepth". at other lines, it starts from index 0
        size_t lineIter = contextDepth;

        do
        {
            // TODO
            // char haye voroodi o int kon
            std::vector< uint8_t > vecDatasetLineInt;
////            vecDatasetLineInt.push_back(context);
//            for (char ch : datasetLine)  vecDatasetLineInt.push_back(symCharToInt(ch));
////            for (size_t i = contextDepth; i != datasetLine.size(); ++i)
////                vecDatasetLineInt.push_back(symCharToInt(datasetLine[ i ]));
    
            for (size_t i = 0; i != contextDepth ; ++i)
            {
                vecDatasetLineInt.push_back((uint8_t) context[i]);
            }
//            for (char c : context)  vecDatasetLineInt.push_back((uint8_t) c);
//            std::cout << std::stoi(context);
            for(uint8_t u:vecDatasetLineInt)    std::cout<<(int)u <<' ';
////            std::cout << datasetLine <<' ';
            
            
            // fill hash table by number of occurrences of symbols A, C, N, G, T
            for (; lineIter != datasetLine.size(); ++lineIter)
            {
//                ++hTable[ context ][ vecDatasetLineInt[ lineIter ]];
    
                // considering inverted repeats to update hash table
                if (isInvertedRepeat)
                {
                    std::string invRepeatContext = "";
                    invRepeatContext += std::to_string(4 - vecDatasetLineInt[ lineIter ]);
                    
                    
                    
//                    std::string s = context + datasetLine[ lineIter ];
//                    for (size_t invIt = s.size()-1; invIt != -1; --invIt)
//                    {
//                        invRepeatContext += symComplementChar(s[ invIt ]);
////                        std::cout
////                                << "\n" << symComplementChar(s[ invIt ]);
//                    }
//
////                    ++hTable[ invRepeatContext ][ 4 - symCharToInt(symComplementChar(s[ 0 ])) ];
    
    
//                    std::cout
////                            << "\n" << (int)symCharToInt(symComplementChar(s[ 0 ]))
////                            << "\n" <<  symIntToChar((uint8_t) 4 - vecDatasetLineInt[ lineIter - contextDepth ])
////                            << "\n" << symIntToChar((uint8_t) 4 - vecDatasetLineInt[ lineIter ])
////                            << "\n"
//                            << invRepeatContext << ' '
////                            << "\n" << (int)4 -vecDatasetLineInt[ lineIter ] << ' '
//                            ;
                }
                
                // update context
//                context = (contextDepth == 1)
//                          ? std::string("") + datasetLine[ lineIter ]
//                          : context.substr(1, (unsigned) contextDepth - 1) + datasetLine[ lineIter ];
    
                context = (contextDepth == 1)
                          ? std::string("") + datasetLine[ lineIter ]
                          : context.substr(1, (unsigned) contextDepth - 1) + std::to_string(vecDatasetLineInt[ lineIter ]);
//                std::cout<< context;
            }
    
            lineIter = 0;           // iterator for non-first lines of file becomes 0
        } while (std::getline(fileIn, datasetLine));

        fileIn.close();             // close file

        FCM::setHashTable(hTable);  // save the built hash table
    }   // end - file opened correctly
}


/***********************************************************
    print hash table
************************************************************/
void FCM::printHashTable (htable_t hTable) const
{

    /***********************************************************
        test
    ************************************************************/
    std::cout << "\tA\tC\tN\tG\tT"
              //              << "\tP_A\tP_C\tP_N\tP_G\tP_T"
              << "\n"
              << "\t-----------------------------------"
              //              << "------------------------------------------"
              << "\n";

    int sum;
    int alpha = 1;

    for (htable_t::iterator it = hTable.begin(); it != hTable.end(); ++it)
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
    transform char symbols into int (ACNGT->01234)
************************************************************/
uint8_t FCM::symCharToInt (char c)
{
    return (c == 'A') ? (uint8_t) 0 :
           (c == 'C') ? (uint8_t) 1 :
           (c == 'G') ? (uint8_t) 3 :
           (c == 'T') ? (uint8_t) 4 : (uint8_t) 2;
}


/***********************************************************
    transform int symbols into char (01234->ACNGT)
************************************************************/
char FCM::symIntToChar (uint8_t i)
{
    return (i == 0) ? 'A' :
           (i == 1) ? 'C' :
           (i == 3) ? 'G' :
           (i == 4) ? 'T' : 'N';
}


/***********************************************************
    transform symbols into their complements (A<>T, N<>N, C<>G)
************************************************************/
char FCM::symComplementChar (char c)
{
    return (c == 'A') ? 'T' :
           (c == 'C') ? 'G' :
           (c == 'G') ? 'C' :
           (c == 'T') ? 'A' : 'N';
}


/***********************************************************
    getters and setters
************************************************************/
uint8_t FCM::getContextDepth () const           { return contextDepth; }
void FCM::setContextDepth (uint8_t ctxDp)       { FCM::contextDepth = ctxDp; }
uint32_t FCM::getAlphaDenom () const            { return alphaDenom; }
void FCM::setAlphaDenom (uint32_t alphaDen)     { FCM::alphaDenom = alphaDen; }
bool FCM::getInvertedRepeat () const            { return invertedRepeat; }
void FCM::setInvertedRepeat (bool invRep)       { FCM::invertedRepeat = invRep; }
const htable_t &FCM::getHashTable () const      { return hashTable; }
void FCM::setHashTable (const htable_t &hT)     { FCM::hashTable = hT; }
const std::string &FCM::getFileAddress () const { return fileAddress; }
void FCM::setFileAddress (const std::string &fA){ FCM::fileAddress = fA; }
