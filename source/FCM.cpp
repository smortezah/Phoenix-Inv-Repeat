#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>      // setw

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
    // TODO: supprt for both target and reference file addresses
    std::string fileName    = getTarFileAddress();

    std::ifstream fileIn(fileName, std::ios::in);   // open file located in fileName

    if (Functions::isFileCorrect(fileName))         // file opened correctly
    {
        std::string initContext(contextDepth, 'A'); // initial context = "AA..."
        std::string context(contextDepth, '0');     // context, that slides in the dataset
    
        htable_t hTable;                            // create hash table
        hTable.insert({context, {0, 0, 0, 0, 0}});  // initialize hash table with 0'z

        std::string datasetLine;                    // to keep each line of file
        std::getline(fileIn, datasetLine);          // read first line of file
        datasetLine = initContext + datasetLine;    // add "AA..." at beginning of first line

        // iterator for each line of file.
        // starts from index "contextDepth" at first line, and index 0 at other lines
        size_t lineIter = contextDepth;
        
        do
        {
            // save integer version of each line in a vector
            std::vector< uint8_t > vecDatasetLineInt;
            for (char ch : datasetLine)  vecDatasetLineInt.push_back( symCharToInt(ch) );
            
            // fill hash table by number of occurrences of symbols A, C, N, G, T
            for (; lineIter != datasetLine.size(); ++lineIter)
            {
                // update hash table
                ++hTable[ context ][ vecDatasetLineInt[ lineIter ]];
    
                // considering inverted repeats to update hash table
                if (isInvertedRepeat)
                {
                    // save inverted repeat context
                    std::string invRepeatContext = "";
                    invRepeatContext += std::to_string(4 - vecDatasetLineInt[ lineIter ]);
                    // convert a number from char into integer format. '0'->0. '4'->4 by
                    // 52 - context[ i ] = 4 - (context[ i ] - 48). 48 is ASCII code of '0'
                    for (int i = contextDepth - 1; i != 0; --i)
                        invRepeatContext += std::to_string( 52 - context[ i ] );
    
                    // update hash table considering inverted repeats
                    ++hTable[ invRepeatContext ][ 52 - context[0] ];
                }
    
                // update context
                context = (contextDepth == 1)
                          ? std::to_string(vecDatasetLineInt[ lineIter ])
                          : context.substr(1, (unsigned) contextDepth - 1)
                            + std::to_string(vecDatasetLineInt[ lineIter ]);
            }
    
            lineIter = 0;           // iterator for non-first lines of file becomes 0
        } while ( std::getline(fileIn, datasetLine) );  // read file line by line

        fileIn.close();             // close file

        FCM::setHashTable(hTable);  // save the built hash table
    }   // end - file opened correctly
}


/***********************************************************
    transform char symbols into int (ACNGT -> 01234)
************************************************************/
uint8_t FCM::symCharToInt (char c) const
{
    return (c == 'A') ? (uint8_t) 0 :
           (c == 'C') ? (uint8_t) 1 :
           (c == 'G') ? (uint8_t) 3 :
           (c == 'T') ? (uint8_t) 4 : (uint8_t) 2;
}


/***********************************************************
    print hash table
************************************************************/
void FCM::printHashTable () const
{
    htable_t hTable = this->getHashTable();
    
    std::string tar_or_ref = (this->getTargetOrReference() == 't' ? "target" : "reference");
    std::string Tar_or_Ref = (this->getTargetOrReference() == 't' ? "Target" : "Reference");
    
    std::cout << " >>> Context model:\t\tBuilt from "  << tar_or_ref << "\n"
              << " >>> Context order size:\t" << (uint16_t) this->getContextDepth() << "\n"
              << " >>> Alpha denominator:\t\t" << this->getAlphaDenom() << "\n"
              << " >>> Inverted repeat:\t\t" << (this->getInvertedRepeat() ? "Considered"
                                                                           : "Not considered")
              << "\n"
              << " >>> " << Tar_or_Ref << " file address:\t"
              // TODO: this line must be changed to
              // << ( tar_or_ref == "target" ? this->getTarFileAddress() : this->getRefFileAddress() )
              << ( tar_or_ref == "target" ? this->getTarFileAddress() : this->getTarFileAddress() )
              << "\n\n";
    
    std::cout << "\tA\tC\tN\tG\tT"
              //              << "\tP_A\tP_C\tP_N\tP_G\tP_T"
              << "\n"
              << "\t-----------------------------------"
              //              << "------------------------------------------"
              << "\n";
    
//    int sum;
//    int alpha = 1;
    
    for (htable_t::iterator it = hTable.begin(); it != hTable.end(); ++it)
    {
//        sum = 0;
        std::cout << it->first << "\t";
        for (int i : it->second)
        {
            std::cout << i << "\t";
//            sum += i;
        }

//        for (int i = 0; i < 5; ++i)
//        {
//            std::cout << std::fixed << std::setprecision(1)
//                      << (float) (it->second[ i ] + alpha) /
//                         (sum + ALPHABET_SIZE * alpha) << "\t";
//        }
        std::cout << "\n";
    }
    
    std::cout << "\n";
}


/***********************************************************
    getters and setters
************************************************************/
char FCM::getTargetOrReference () const              { return targetOrReference; }
void FCM::setTargetOrReference (char tOrR)           { FCM::targetOrReference = tOrR; }
uint8_t FCM::getContextDepth () const                { return contextDepth; }
void FCM::setContextDepth (uint8_t ctxDp)            { FCM::contextDepth = ctxDp; }
uint32_t FCM::getAlphaDenom () const                 { return alphaDenom; }
void FCM::setAlphaDenom (uint32_t alphaDen)          { FCM::alphaDenom = alphaDen; }
bool FCM::getInvertedRepeat () const                 { return invertedRepeat; }
void FCM::setInvertedRepeat (bool invRep)            { FCM::invertedRepeat = invRep; }
const htable_t &FCM::getHashTable () const           { return hashTable; }
void FCM::setHashTable (const htable_t &hT)          { FCM::hashTable = hT; }
const std::string &FCM::getTarFileAddress () const   { return tarFileAddress; }
void FCM::setTarFileAddress (const std::string &tFA) { FCM::tarFileAddress = tFA; }
const std::string &FCM::getRefFileAddress () const   { return refFileAddress; }
void FCM::setRefFileAddress (const std::string &rFA) { FCM::refFileAddress = rFA; }
