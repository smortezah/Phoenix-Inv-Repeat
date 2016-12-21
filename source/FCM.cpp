#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>      // setw, setprecision
#include <cstring>      // memset, memcpy

#include "FCM.h"
#include "functions.h"

using std::cout;
using std::string;
using std::ifstream;
using std::getline;
using std::to_string;
using std::ios;
using std::memset;
using std::memmove;
using std::fixed;
using std::setprecision;

////TODO TEST
//using std::chrono::high_resolution_clock;

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
    uint16_t alphaDen       = getAlphaDenom();      // get alpha denominator
    bool isInvertedRepeat   = getInvertedRepeat();  // get inverted repeat
    // TODO: supprt for both target and reference file addresses
    string fileName         = getTarFileAddress();

    ifstream fileIn(fileName, ios::in);             // open file located in fileName
    
    if (Functions::isFileCorrect(fileName))         // file opened correctly
    {
        string context(contextDepth, '0');          // context, that slides in the dataset
//        // context, that slides in the dataset
//        uint8_t context[contextDepth];
//        memset(context, 0, contextDepth);
        
        htable_t hTable;                            // create hash table
        hTable.insert({context, {0, 0, 0, 0, 0}});  // initialize hash table with 0'z
        
//        //TODO
//        hTable.at(context)={1,2,3,4,5};
        
//        hTable.insert({context, {0, 0, 0, 0, 0, 0}});// initialize hash table with 0'z

        string datasetFirstLine;                    // keep first line of file
        getline(fileIn, datasetFirstLine);          // read first line of file
        string datasetLine(contextDepth, '0');      // to keep each line of file
//        for (size_t i = 0; i != contextDepth ; ++i)    datasetLine += '0';
        datasetLine += datasetFirstLine;

        // iterator for each line of file.
        // starts from index "contextDepth" at first line, and index 0 at other lines
//        size_t lineIter = contextDepth;
        string::iterator lineIter = datasetFirstLine.begin();
        
        //////////////////////////////////
        uint64_t nSym;                      // number of symbols (n_s). To calculate probability
        uint64_t sumNSyms;                  // sum of number of symbols (sum n_a). To calculate probability
        double   probability = 0;           // probability of a symbol, based on an identified context
        double   sumOfEntropies = 0;        // sum of entropies for different symbols
        uint8_t  dataSetLineSize;           // size of each line of dataset
        uint64_t totalNumberOfSymbols = 0;  // number of all symbols in the sequence
        double   averageEntropy = 0;        // average entropy (H)
        //////////////////////////////////

//        //TODO TEST
//        high_resolution_clock::time_point exeStartTime = high_resolution_clock::now();
//        sumNSyms = 0;
        do
        {
            
            //////////////////////////////////
            dataSetLineSize = (uint8_t) datasetLine.size();

            totalNumberOfSymbols += dataSetLineSize;    // number of symbols in each line of dataset
            //////////////////////////////////

            // fill hash table by number of occurrences of symbols A, C, N, G, T
//            for (; lineIter != dataSetLineSize; ++lineIter)
            for (; lineIter != datasetLine.end(); ++lineIter)
            {
                // htable includes an array of uint16_t numbers
//                uint8_t currSymInt = symCharToInt(datasetLine[ lineIter ]);
//                char c = datasetLine[ lineIter ];
                char c = *lineIter;
                uint8_t currSymInt = (c == 'A') ? (uint8_t) 0 :
                                     (c == 'C') ? (uint8_t) 1 :
                                     (c == 'G') ? (uint8_t) 3 :
                                     (c == 'T') ? (uint8_t) 4 : (uint8_t) 2;

                //////////////////////////////////
                // update hash table
                nSym = hTable[ context ][ currSymInt ]++;

////                // sum(n_a)
                sumNSyms = 0;
                for (uint64_t u : hTable[ context ])      sumNSyms += u;
//                sumNSyms += hTable[ context ][0]+hTable[ context ][1]+hTable[ context ][2]+
//                        hTable[ context ][3]+hTable[ context ][4];
//                cout<<"\n"<<context<<'\t'<<sumNSyms<<"\n";
//                hTable[ context ][ 5 ] = sumNSyms;
                if(sumNSyms<0)cout<<sumNSyms;

                // P(s|c^t)
//                probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPHABET_SIZE/alphaDen);
                probability = (double) (alphaDen*nSym + 1) / (alphaDen*sumNSyms + ALPHABET_SIZE);

                // sum( log_2 P(s|c^t) )
                sumOfEntropies += log2(probability);
                /////////////////////////////////


//                cout << "ctx_bef ";for (uint8_t u:context) cout << (int) u;
//                cout << '\n' << "curr_Sym " << (int) currSymInt << '\n';

                // update hash table
//                ++hTable[ context ][ currSymInt ];
//                ++hTable[ context ][ 5 ];

                // considering inverted repeats to update hash table
                if (isInvertedRepeat)
                {
                    // save inverted repeat context
                    string invRepeatContext = "";
                    invRepeatContext += to_string(4 - currSymInt);
                    // convert a number from char into integer format. '0'->0. '4'->4 by
                    // 52 - context[ i ] = 4 - (context[ i ] - 48). 48 is ASCII code of '0'
                    for (int i = contextDepth - 1; i != 0; --i)
                        invRepeatContext += to_string( 52 - context[ i ] );

                    // update hash table considering inverted repeats
                    ++hTable[ invRepeatContext ][ 52 - context[ 0 ]];
//                    ++hTable[ invRepeatContext ][ 5 ];
                }

                // update context
                context = (contextDepth == 1)
                          ? to_string(currSymInt)
                          : context.substr(1, (unsigned) contextDepth - 1)
                            + to_string(currSymInt);
////                cout << "ctx_bef ";for (uint8_t u:context) cout << (int) u;
////                cout << '\n' << "curr_Sym " << (int) currSymInt << '\n';
//                for (int i = 0; i < 5; ++i) cout<<(hTable[ context ])[ i ];
//                cout<<"\n\n";
//
//                memcpy(context, context + 1, contextDepth - 1);
//                context[ contextDepth-1 ] = currSymInt;
            }

//            lineIter = 0;           // iterator for non-first lines of file becomes 0
            lineIter = datasetLine.begin();         // iterator for non-first lines of file becomes 0
    
        } while ( getline(fileIn, datasetLine) );   // read file line by line


//        //TODO TEST
//        high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now();
//        std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
//        cout << '\t' << "build_hash="<<elapsed.count();


        ////////////////////////////////
        totalNumberOfSymbols -= contextDepth;   // first line includes contextDepth of "AA..."
        
        // H_N = -1/N sum( log_2 P(s|c^t) )
        averageEntropy = (-1) * sumOfEntropies / totalNumberOfSymbols;
        
        cout
//                << nSym << '\n'
//                << sumNSyms << '\n'
//                << probability << '\n'
//                << sumOfEntropies << '\n'
//                << totalNumberOfSymbols << '\n'
                << "  "
                << getInvertedRepeat() << '\t'
                << (float) 1/alphaDen << '\t'
                << (int) contextDepth << '\t'
                << averageEntropy
//                << '\t'
//                << hTable.size();
//                << '\n'
;

//        for (htable_t::iterator it = hTable.begin(); it != hTable.end(); ++it)
//        {
//            cout << it->first << "\t";
//            for (int i : it->second)    cout << i << "\t";
//            cout << '\n';
//        }
//        for(uint16_t u:hTable[context]) cout<<u;
        ////////////////////////////////

        fileIn.close();             // close file

        FCM::setHashTable(hTable);  // save the built hash table
    }   // end - file opened correctly
    
    
//
//    //TODO TEST
//    high_resolution_clock::time_point exeFinishTime = high_resolution_clock::now();
//    std::chrono::duration< double > elapsed = exeFinishTime - exeStartTime;
//    cout << '\t' << "build_hash="<<elapsed.count();
    
}


///***********************************************************
//    transform char symbols into int (ACNGT -> 01234)
//************************************************************/
//uint8_t FCM::symCharToInt (char c) const
//{
//    return (c == 'A') ? (uint8_t) 0 :
//           (c == 'C') ? (uint8_t) 1 :
//           (c == 'G') ? (uint8_t) 3 :
//           (c == 'T') ? (uint8_t) 4 : (uint8_t) 2;
//}


/***********************************************************
    print hash table
************************************************************/
void FCM::printHashTable () const
{
    htable_t hTable = this->getHashTable();
    
    string tar_or_ref = (this->getTargetOrReference() == 't' ? "target" : "reference");
    string Tar_or_Ref = (this->getTargetOrReference() == 't' ? "Target" : "Reference");
    
    cout
         << " >>> Context model:\t\tBuilt from "  << tar_or_ref << '\n'
         << " >>> Context order size:\t" << (uint16_t) this->getContextDepth() << '\n'
         << " >>> Alpha denominator:\t\t" << this->getAlphaDenom() << '\n'
         << " >>> Inverted repeat:\t\t" << (this->getInvertedRepeat() ? "Considered"
                                                                      : "Not considered")
         << '\n'
         << " >>> " << Tar_or_Ref << " file address:\t"
         // TODO: this line must be changed to
         // << ( tar_or_ref == "target" ? this->getTarFileAddress() : this->getRefFileAddress() )
         << ( tar_or_ref == "target" ? this->getTarFileAddress() : this->getTarFileAddress() )
         << "\n\n";
    
    cout << "\tA\tC\tN\tG\tT"
         //              << "\tP_A\tP_C\tP_N\tP_G\tP_T"
         << '\n'
         << "\t-----------------------------------"
         //              << "------------------------------------------"
         << '\n';

    for (htable_t::iterator it = hTable.begin(); it != hTable.end(); ++it)
    {
        cout << it->first;
        cout << "\t";
        for (uint64_t i : it->second)    cout << i << "\t";
        cout << '\n';
    }
    
    cout << '\n';
}


/***********************************************************
    getters and setters
************************************************************/
char FCM::getTargetOrReference () const         { return targetOrReference; }
void FCM::setTargetOrReference (char tOrR)      { FCM::targetOrReference = tOrR; }
uint8_t FCM::getContextDepth () const           { return contextDepth; }
void FCM::setContextDepth (uint8_t ctxDp)       { FCM::contextDepth = ctxDp; }
uint16_t FCM::getAlphaDenom () const            { return alphaDenom; }
void FCM::setAlphaDenom (uint16_t alphaDen)     { FCM::alphaDenom = alphaDen; }
bool FCM::getInvertedRepeat () const            { return invertedRepeat; }
void FCM::setInvertedRepeat (bool invRep)       { FCM::invertedRepeat = invRep; }
const htable_t &FCM::getHashTable () const      { return hashTable; }
void FCM::setHashTable (const htable_t &hT)     { FCM::hashTable = hT; }
const string &FCM::getTarFileAddress () const   { return tarFileAddress; }
void FCM::setTarFileAddress (const string &tFA) { FCM::tarFileAddress = tFA; }
const string &FCM::getRefFileAddress () const   { return refFileAddress; }
void FCM::setRefFileAddress (const string &rFA) { FCM::refFileAddress = rFA; }
