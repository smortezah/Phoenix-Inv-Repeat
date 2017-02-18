#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>      /// setw, setprecision
#include <cstring>      /// memset, memcpy
#include <stdlib.h>
#include <vector>

#include "FCM.h"
#include "functions.h"

using std::cout;
using std::cerr;
using std::string;
using std::vector;
using std::ifstream;
using std::getline;
using std::to_string;
using std::ios;
using std::memset;
using std::memmove;
using std::fixed;
using std::setprecision;

/// TODO TEST
//using std::chrono::high_resolution_clock;

/***********************************************************
    constructor
************************************************************/
FCM::FCM () {}


/***********************************************************
    build reference(s) model
************************************************************/
void FCM::buildModel ()
{
    const uint8_t contextDepth     = getContextDepth();         /// context depth
    const bool isInvertedRepeat    = getInvertedRepeat();       /// inverted repeat
    vector< string > refFilesNames = getRefFilesAddresses();    /// reference file(s) address(es)
    
    uint8_t refsNumber = (uint8_t) refFilesNames.size();        /// number of references
    
    /// set compression mode: 't'=table, 'h'=hash table
    /*
    const char mode = (contextDepth > TABLE_MAX_CONTEXT) ? 'h' : 't';
     */
    /// supports multi-references case
    if ( (uint64_t) refsNumber > (uint64_t) pow(ALPHABET_SIZE, TABLE_MAX_CONTEXT-contextDepth) )
        setCompressionMode('h');
    else
        setCompressionMode('t');
    
    /// check if reference(s) file(s) cannot be opened, or are empty
    ifstream refFilesIn[ refsNumber ];
    
    for (uint8_t i = refsNumber; i--;)
    {
        refFilesIn[ i ].open( refFilesNames[ i ], ios::in );
        if (!refFilesIn[ i ])                   /// error occurred while opening file(s)
        {
            cerr << "The file '" << refFilesNames[ i ] << "' cannot be opened, or it is empty.\n";
            refFilesIn[ i ].close();            /// close file(s)
            return;                             /// exit this function
        }
    }
    
    uint64_t context = 0;                       /// context (integer), that slides in the dataset
    uint64_t maxPlaceValue = (uint64_t) pow(ALPHABET_SIZE, contextDepth);
    uint64_t invRepContext = maxPlaceValue - 1; /// inverted repeat context (integer)
    
    string refLine;                             /// keep each line of a file
    
    /// build model based on 't'=table, or 'h'=hash table
    switch ( getCompressionMode() )
    {
        case 't':
        {
            uint64_t tableSize = refsNumber * maxPlaceValue * ALPH_SUM_SIZE;    /// create table
            uint64_t *table = new uint64_t[ tableSize ];                        /// already initialized with 0's
            /*
            /// initialize table with 0's
            memset(table, 0, sizeof(table[ 0 ]) * tableSize);
            */
            
            for (uint8_t i = refsNumber; i--;)
            {
                context = 0;    /// reset in the beginning of each reference file
                
                while (getline(refFilesIn[ i ], refLine))
                {
                    /// fill table by number of occurrences of symbols A, C, N, G, T
                    for (string::iterator lineIter = refLine.begin(); lineIter != refLine.end(); ++lineIter)
                    {
                        uint8_t currSymInt = symCharToInt(*lineIter);
                        
                        /// update table
                        ++table[ context * ALPH_SUM_SIZE + currSymInt ];
                        
                        /// considering inverted repeats to update hash table
                        if (isInvertedRepeat)
                        {
                            /// concatenation of inverted repeat context and current symbol
                            uint64_t iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
                            
                            /// update inverted repeat context (integer)
                            invRepContext = (uint64_t) iRCtxCurrSym / ALPHABET_SIZE;
                            
                            /// update table considering inverted repeats
                            ++table[ invRepContext * ALPH_SUM_SIZE + iRCtxCurrSym % ALPHABET_SIZE ];
                            /// update column 'sum' of the table
                            ++table[ invRepContext * ALPH_SUM_SIZE + ALPHABET_SIZE ];
                        }
                        
                        /// update column 'sum' of the table
                        ++table[ context * ALPH_SUM_SIZE + ALPHABET_SIZE ];
                        
                        /// update context
                        context = (uint64_t) (context * ALPHABET_SIZE + currSymInt) % maxPlaceValue;
                    }   /// end for
                }   /// end while
            }   /// end for
    
            FCM::setTable(table);   /// save the built table
        }   /// end case
            break;
        
        case 'h':
        {
            htable_t hTable;    /// create hash table
            
            for (int i = refsNumber; i--;)
            {
                context = 0;    /// reset in the beginning of each reference file
        
                while (getline(refFilesIn[i], refLine))
                {
                    /// fill hash table by number of occurrences of symbols A, C, N, G, T
                    for (string::iterator lineIter = refLine.begin(); lineIter != refLine.end(); ++lineIter)
                    {
                        uint8_t currSymInt = symCharToInt(*lineIter);
                        
                        /// update hash table
                        ++hTable[ context ][ currSymInt ];
                        
                        /// considering inverted repeats to update hash table
                        if (isInvertedRepeat)
                        {
                            /// concatenation of inverted repeat context and current symbol
                            uint64_t iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
                            
                            /// update inverted repeat context (integer)
                            invRepContext = (uint64_t) iRCtxCurrSym / ALPHABET_SIZE;
                            
                            /// update hash table considering inverted repeats
                            ++hTable[ invRepContext ][ iRCtxCurrSym % ALPHABET_SIZE ];
                        }
                        
                        /// update context
                        context = (uint64_t) (context * ALPHABET_SIZE + currSymInt) % maxPlaceValue;
                    }   /// end for
                }   /// end while
            }
            
            FCM::setHashTable(hTable);  /// save the built hash table
        }   /// end case
            break;
        
        default: break;
    }   /// end switch
    
    for (uint8_t i = refsNumber; i--;)  refFilesIn[i].close();      /// close file(s)
}


/***********************************************************
    compress target(s) based on reference(s) model
************************************************************/
void FCM::compressTarget (string tarFileName)
{
    const uint8_t contextDepth  = getContextDepth();    /// get context depth
    const uint16_t alphaDen     = getAlphaDenom();      /// get alpha denominator
////    const double alphaDen     = getAlphaDenom();        /// get alpha denominator
    
    ifstream tarFileIn( tarFileName, ios::in ); /// open target file
    
    if (!tarFileIn)                             /// error occurred while opening file
    {
        cerr << "The file '" << tarFileName << "' cannot be opened, or it is empty.\n";
        tarFileIn.close();                      /// close file
        return;                                 /// exit this function
    }
    
    uint64_t maxPlaceValue = (uint64_t) pow(ALPHABET_SIZE, contextDepth);
    uint64_t tarContext = 0;                    /// context (integer), that slides in the dataset
    
    string tarLine;                             /// keep each line of the file
    
    ////////////////////////////////
    uint64_t nSym;                              /// number of symbols (n_s). To calculate probability
    uint64_t sumNSyms;                          /// sum of number of symbols (sum n_a). To calculate probability
    double   probability = 0;                   /// probability of a symbol, based on an identified context
    double   sumOfEntropies = 0;                /// sum of entropies for different symbols
    uint64_t totalNOfSyms = 0;                  /// number of all symbols in the sequence
    double   averageEntropy = 0;                /// average entropy (H)
    //////////////////////////////////
    
    /*
    /// using macros make this code slower
    htable_t hTable = getHashTable();
//#define X \
         ((mode == 'h') ? (hTable[ tarContext ][ currSymInt ]) : (table[ tarContext * ALPH_SUM_SIZE + currSymInt ]))

#define X(in) do { \
                (mode == 'h') \
                ? in = hTable[ tarContext ][ currSymInt ] \
                : in = table[ tarContext * ALPH_SUM_SIZE + currSymInt ]; \
              } while ( 0 )

#define Y(in) do { \
                (mode == 't') \
                ? in = table[ tarContext * ALPH_SUM_SIZE + ALPHABET_SIZE ] \
                : in = 0; for (uint64_t u : hTable[ tarContext ]) in += u; \
              } while ( 0 )
    */
    
    switch ( getCompressionMode() )
    {
        case 't':
        {
            uint64_t *table = getTable();
        
            while (getline(tarFileIn, tarLine))
            {
        
                //////////////////////////////////
                totalNOfSyms = totalNOfSyms + tarLine.size();   /// number of symbols in each line of dataset
                //////////////////////////////////
        
                /// table includes the number of occurrences of symbols A, C, N, G, T
                for (string::iterator lineIter = tarLine.begin(); lineIter != tarLine.end(); ++lineIter)
                {
                    uint8_t currSymInt = symCharToInt(*lineIter);   /// integer version of the current symbol
        
                    //////////////////////////////////
                    /// number of symbols
                    nSym = table[ tarContext * ALPH_SUM_SIZE + currSymInt ];
                    /*
                    nSym = X;
                    X(nSym);
                    */
                    
                    /// sum of number of symbols
                    sumNSyms = table[ tarContext * ALPH_SUM_SIZE + ALPHABET_SIZE ];
                    /*
                    Y(sumNSyms);
                    */
                    
                    /// P(s|c^t)
                    probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPHABET_SIZE);
        
                    /// sum( log_2 P(s|c^t) )
                    sumOfEntropies = sumOfEntropies + log2(probability);
                    /////////////////////////////////
        
                    /// update context
                    tarContext = (uint64_t) (tarContext * ALPHABET_SIZE + currSymInt) % maxPlaceValue;
                }   /// end for
            }   /// end while
        }   /// end case
        break;
        
        case 'h':
        {
            htable_t hTable = getHashTable();
        
            while (getline(tarFileIn, tarLine))
            {
        
                //////////////////////////////////
                totalNOfSyms = totalNOfSyms + tarLine.size();   /// number of symbols in each line of dataset
                //////////////////////////////////
        
                /// hash table includes the number of occurrences of symbols A, C, N, G, T
                for (string::iterator lineIter = tarLine.begin(); lineIter != tarLine.end(); ++lineIter)
                {
                    uint8_t currSymInt = symCharToInt(*lineIter);   /// integer version of the current symbol
        
                    //////////////////////////////////
//                    if (hTable.find(tarContext) == hTable.end()) { nSym = 0;   sumNSyms = 0; }
//                    else
//                    {
                        /// number of symbols
                        nSym = hTable[ tarContext ][ currSymInt ];
                        /*
                        nSym = X;
                        X(nSym);
                        */
                    
                        /// the idea of adding 'sum' column, makes hash table slower
                        /// sum(n_a)
                        sumNSyms = 0; for (uint64_t u : hTable[ tarContext ])   sumNSyms = sumNSyms + u;
                        /*
                        Y(sumNSyms);
                        */
//                    }
                    
                    /// P(s|c^t)
                    probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPHABET_SIZE);
        
                    /// sum( log_2 P(s|c^t) )
                    sumOfEntropies = sumOfEntropies + log2(probability);
                    /////////////////////////////////
        
                    /// update context
                    tarContext = (uint64_t) (tarContext * ALPHABET_SIZE + currSymInt) % maxPlaceValue;
                }   /// end for
            }   /// end while
        }   /// end case
        break;
        
        default: break;
    }   /// end switch
    
    tarFileIn.close();          /// close file
    
    ////////////////////////////////
    /// H_N = -1/N sum( log_2 P(s|c^t) )
    averageEntropy = (-1) * sumOfEntropies / totalNOfSyms;

//    cout << sumOfEntropies << '\n';
//    cout << totalNOfSyms << '\n';
//    cout << ' ';
    
    /// print reference and target file names in the output
    uint8_t refsAdressesSize = (uint8_t) getRefFilesAddresses().size();
    size_t lastSlash_Ref[ refsAdressesSize ];
    for (uint8_t i = refsAdressesSize; i--;)
        lastSlash_Ref[ i ] = getRefFilesAddresses()[ i ].find_last_of("/");
    size_t lastSlash_Tar = tarFileName.find_last_of("/");
    
    /// mutex lock ========================================================
    mut.lock();
    
    for (int i = refsAdressesSize - 1; i; i--)
        cout << getRefFilesAddresses()[ i ].substr(lastSlash_Ref[ i ] + 1) << ',';
    cout << getRefFilesAddresses()[ 0 ].substr(lastSlash_Ref[ 0 ] + 1) << '\t'
         << tarFileName.substr(lastSlash_Tar + 1) << '\t';
    
    cout << getInvertedRepeat() << '\t'
         << setprecision(5) << (float) 1 / alphaDen << '\t'
         //            cout << (double) 1/alphaDen << '\t'
         << (int) contextDepth << '\t'
         << std::fixed << setprecision(5) << averageEntropy << '\t'
         << std::fixed << setprecision(5) << averageEntropy/LOG2_ALPHABET_SIZE;
    
//    cout.width(2);  cout << std::left << getInvertedRepeat() << "   ";
    
    cout<<'\n';
    
    mut.unlock();
    /// mutex unlock ======================================================
    ////////////////////////////////
    
}


/***********************************************************
    convert char (base) to integer (uint8_t): ACNGT -> 01234
************************************************************/
inline uint8_t FCM::symCharToInt (char ch) const
{
    switch (ch)
    {
        case 'A':   return (uint8_t) 0;
        case 'C':   return (uint8_t) 1;
        case 'G':   return (uint8_t) 3;
        case 'T':   return (uint8_t) 4;
        default:    return (uint8_t) 2;     /// N symbol
    }
        
//    return (uint8_t) (ch % ALPHABET_SIZE);

//    switch (ch)
//    {
//        case 'C':   return (uint8_t) 3;
//        case 'N':   return (uint8_t) 2;
//        default:    return (uint8_t) (ch % ALPHABET_SIZE);
//    }
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
///// version: reference file not considered
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//
//***********************************************************
//    build table
//************************************************************/
//void FCM::buildTable ()
//{
//    const uint8_t contextDepth  = getContextDepth();    /// get context depth
//    const uint16_t alphaDen     = getAlphaDenom();      /// get alpha denominator
//    const bool isInvertedRepeat = getInvertedRepeat();  /// get inverted repeat
//    /// TODO: supprt for both target and reference file addresses
//    string fileName = getTarFileAddress();              /// get target file address
//
//
////    const char* filename= fileName.c_str();;
////    std::FILE *fp = std::fopen(filename, "rb");
////    if (fp)
////    {
////        std::string contents;
////        std::fseek(fp, 0, SEEK_END);
////        contents.resize(std::ftell(fp));
////        std::rewind(fp);
////        std::fread(&contents[ 0 ], 1, contents.size(), fp);
////        std::fclose(fp);
////    }
//
//
//    ifstream fileIn(fileName, ios::in); /// open file located in fileName
//
//    if (!fileIn)                        /// error occurred while opening file
//    {
//        cerr << "The file '" << fileName << "' cannot be opened, or it is empty.\n";
//        fileIn.close();                 /// close file
//        return;                         /// exit this function
//    }
//
//    /// create table
//    /// 5^TABLE_MAX_CONTEXT < 2^32 => uint32_t is used, otherwise uint64_t
//    uint32_t maxPlaceValue = (uint32_t) pow(ALPHABET_SIZE, contextDepth);
//    uint64_t tableSize = maxPlaceValue * ALPH_SUM_SIZE;
//    uint64_t *table = new uint64_t[ tableSize ];
//
//    /// initialize table with 0's
//    memset(table, 0, sizeof(table[0]) * tableSize);
//
//    uint32_t context = 0;                       /// context (integer), that slides in the dataset
//    uint32_t invRepContext = maxPlaceValue - 1; /// inverted repeat context (integer)
//
//    ////////////////////////////////
//    uint64_t nSym;                      /// number of symbols (n_s). To calculate probability
//    uint64_t sumNSyms;                  /// sum of number of symbols (sum n_a). To calculate probability
//    double   probability = 0;           /// probability of a symbol, based on an identified context
//    double   sumOfEntropies = 0;        /// sum of entropies for different symbols
//    uint64_t totalNumberOfSymbols = 0;  /// number of all symbols in the sequence
//    double   averageEntropy = 0;        /// average entropy (H)
//    //////////////////////////////////
//
//    string datasetLine;                 /// keep each line of the file
//
//    while (getline(fileIn, datasetLine))
//    {
//
//        //////////////////////////////////
//        totalNumberOfSymbols += datasetLine.size();    /// number of symbols in each line of dataset
//        //////////////////////////////////
//
//        /// fill hash table by number of occurrences of symbols A, C, N, G, T
//        for (string::iterator lineIter = datasetLine.begin(); lineIter != datasetLine.end(); ++lineIter)
//        {
//            /// htable includes an array of uint64_t numbers
//            char ch = *lineIter;
//            uint8_t currSymInt = (uint8_t) ((ch == 'A') ? 0 :
//                                            (ch == 'C') ? 1 :
//                                            (ch == 'G') ? 3 :
//                                            (ch == 'T') ? 4 : 2);
//
////            uint8_t currSymInt = (ch == 'A') ? (uint8_t) 0 :
////                                 (ch == 'C') ? (uint8_t) 1 :
////                                 (ch == 'G') ? (uint8_t) 3 :
////                                 (ch == 'T') ? (uint8_t) 4 : (uint8_t) 2;
////            uint8_t currSymInt = ch % ALPHABET_SIZE;
////            uint8_t currSymInt = (ch == 'C') ? (uint8_t) 3 :
////                                 (ch == 'N') ? (uint8_t) 2 :
////                                 (uint8_t) (ch % ALPHABET_SIZE);
//
//            /// update table
//            nSym = table[ context * ALPH_SUM_SIZE + currSymInt ]++;
//
//            /// considering inverted repeats to update hash table
//            if (isInvertedRepeat)
//            {
//                /// concatenation of inverted repeat context and current symbol
//                uint32_t iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
//
////                /// to save quotient and reminder of a division
////                div_t iRCtxCurrSymDiv;
////                iRCtxCurrSymDiv = div(iRCtxCurrSym, ALPHABET_SIZE);
//
//                /// update inverted repeat context (integer)
////                invRepContext = (uint32_t) iRCtxCurrSymDiv.quot;
//                invRepContext = (uint32_t) iRCtxCurrSym / ALPHABET_SIZE;
//
//                /// update table considering inverted repeats
////                ++table[ invRepContext*ALPHABET_SIZE + iRCtxCurrSymDiv.rem ];
////                ++table[ invRepContext * ALPHABET_SIZE + iRCtxCurrSym % ALPHABET_SIZE ];
//                ++table[ invRepContext * ALPH_SUM_SIZE + iRCtxCurrSym % ALPHABET_SIZE ];
//                ++table[ invRepContext * ALPH_SUM_SIZE + ALPHABET_SIZE ];
//            }
//
//            //////////////////////////////////
//            /// sum(n_a)
////            uint64_t *pointerToTable = table;   /// pointer to the beginning of table
////            sumNSyms = 0;
////            for (uint8_t i = 0; i < ALPHABET_SIZE; ++i)
////                sumNSyms += *(pointerToTable + context*ALPHABET_SIZE + i);
//            sumNSyms = ++table[ context * ALPH_SUM_SIZE + ALPHABET_SIZE ];
//
//            /// P(s|c^t)
////            probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPHABET_SIZE/alphaDen);
//            probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPHABET_SIZE);
//
//            /// sum( log_2 P(s|c^t) )
//            sumOfEntropies += log2(probability);
//            /////////////////////////////////
//
//            /// update context
//            context = (uint32_t) (context * ALPHABET_SIZE + currSymInt) % maxPlaceValue;
//
//        }   /// end of for
//    }   /// end of while
//
//    fileIn.close();             /// close file
//
//    FCM::setTable(table);       /// save the built table
//
//
//    ////////////////////////////////
//    /// H_N = -1/N sum( log_2 P(s|c^t) )
//    averageEntropy = (-1) * sumOfEntropies / totalNumberOfSymbols;
//

////    cout << sumOfEntropies << '\n';
////    cout << totalNOfSyms << '\n';
////    cout << "  ";
//cout.width(2);  cout << std::left << getInvertedRepeat() << "   ";
//cout.width(6);  cout << std::left << (float) 1/alphaDen << "   ";
////             cout.width(7);  << std::left << (double) 1/alphaDen << "   "
//cout.width(3);  cout << std::left << (int) contextDepth << "   ";
//cout.width(8);  cout << std::left << averageEntropy << "   ";
//cout.width(8);  cout << std::left << averageEntropy/LOG2_ALPHABET_SIZE;
//    ////////////////////////////////
//
//}
//
//
//***********************************************************
//    build hash table
//************************************************************/
//void FCM::buildHashTable ()
//{
//    const uint8_t contextDepth  = getContextDepth();    /// get context depth
//    const uint16_t alphaDen     = getAlphaDenom();      /// get alpha denominator
//    const bool isInvertedRepeat = getInvertedRepeat();  /// get inverted repeat
//    /// TODO: supprt for both target and reference file addresses
//    string fileName = getTarFileAddress();              /// get target file address
//
//
////    const char* filename= fileName.c_str();;
////    std::FILE *fp = std::fopen(filename, "rb");
////    if (fp)
////    {
////        std::string contents;
////        std::fseek(fp, 0, SEEK_END);
////        contents.resize(std::ftell(fp));
////        std::rewind(fp);
////        std::fread(&contents[ 0 ], 1, contents.size(), fp);
////        std::fclose(fp);
////    }
//
//
//    ifstream fileIn(fileName, ios::in);         /// open file located in fileName
//
//    if (!fileIn)                                /// error occurred while opening file
//    {
//        cerr << "The file '" << fileName << "' cannot be opened, or it is empty.\n";
//        fileIn.close();                         /// close file
//        return;                                 /// exit this function
//    }
//
//    uint64_t context = 0;                       /// context, that slides in the dataset
//    uint64_t maxPlaceValue = (uint64_t) pow(ALPHABET_SIZE, contextDepth);
//    uint64_t invRepContext = maxPlaceValue - 1; /// inverted repeat context
//
//    htable_t hTable;                            /// create hash table
//    hTable.insert({context, {0, 0, 0, 0, 0}});  /// initialize hash table with 0's
//
//    //////////////////////////////////
//    uint64_t nSym;                     /// number of symbols (n_s). To calculate probability
//    uint64_t sumNSyms;                 /// sum of number of symbols (sum n_a). To calculate probability
//    double   probability = 0;          /// probability of a symbol, based on an identified context
//    double   sumOfEntropies = 0;       /// sum of entropies for different symbols
//    uint64_t totalNumberOfSymbols = 0; /// number of all symbols in the sequence
//    double   averageEntropy = 0;       /// average entropy (H)
//    //////////////////////////////////
//
//    string datasetLine;                /// keep each line of the file
//
//    while (getline(fileIn, datasetLine))
//    {
//
//        //////////////////////////////////
//        totalNumberOfSymbols += datasetLine.size();    /// number of symbols in each line of dataset
//        //////////////////////////////////
//
//        /// fill hash table by number of occurrences of symbols A, C, N, G, T
//        for (string::iterator lineIter = datasetLine.begin(); lineIter != datasetLine.end(); ++lineIter)
//        {
//            /// htable includes an array of uint64_t numbers
//            char ch = *lineIter;
//            uint8_t currSymInt = (uint8_t) ((ch == 'A') ? 0 :
//                                            (ch == 'C') ? 1 :
//                                            (ch == 'G') ? 3 :
//                                            (ch == 'T') ? 4 : 2);
//
////            uint8_t currSymInt = (ch == 'A') ? (uint8_t) 0 :
////                                 (ch == 'C') ? (uint8_t) 1 :
////                                 (ch == 'G') ? (uint8_t) 3 :
////                                 (ch == 'T') ? (uint8_t) 4 : (uint8_t) 2;
////            uint8_t currSymInt = ch % 5;
////            uint8_t currSymInt = (ch == 'C') ? (uint8_t) 3 :
////                                 (ch == 'N') ? (uint8_t) 2 :
////                                 (uint8_t) (ch % ALPHABET_SIZE);
//
//            /// update hash table
//            nSym = hTable[ context ][ currSymInt ]++;
//
//            /// considering inverted repeats to update hash table
//            if (isInvertedRepeat)
//            {
//                /// concatenation of inverted repeat context and current symbol
//                uint64_t iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
//
//                /// to save quotient and reminder of a division
////                div_t iRCtxCurrSymDiv;
////                iRCtxCurrSymDiv = div(iRCtxCurrSym, ALPHABET_SIZE);
//
//                /// update inverted repeat context (integer)
////                invRepContext =  iRCtxCurrSymDiv.quot;
//                invRepContext = (uint64_t) iRCtxCurrSym / ALPHABET_SIZE;
//
//                /// update table considering inverted repeats
////                ++hTable[ invRepContext ][ iRCtxCurrSymDiv.rem];
//                ++hTable[ invRepContext ][ iRCtxCurrSym % ALPHABET_SIZE ];
//            }
//
//            //////////////////////////////////
//            /// the idea of adding 'sum' column, makes hash table slower
//            /// sum(n_a)
//            sumNSyms = 0;
//            for (uint64_t u : hTable[ context ])    sumNSyms += u;
//
//            /// P(s|c^t)
////            probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPHABET_SIZE/alphaDen);
//            probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPHABET_SIZE);
//
//            /// sum( log_2 P(s|c^t) )
//            sumOfEntropies += log2(probability);
//            /////////////////////////////////
//
//            /// update context
//            context = (uint64_t) (context * ALPHABET_SIZE + currSymInt) % maxPlaceValue;
//
//        }   /// end of for
//    }   /// end of while
//
//    fileIn.close();             /// close file
//
//    FCM::setHashTable(hTable);  /// save the built hash table
//
//    ////////////////////////////////
//    /// H_N = -1/N sum( log_2 P(s|c^t) )
//    averageEntropy = (-1) * sumOfEntropies / totalNumberOfSymbols;
//

////    cout << sumOfEntropies << '\n';
////    cout << totalNOfSyms << '\n';
////    cout << "  ";
//cout.width(2);  cout << std::left << getInvertedRepeat() << "   ";
//cout.width(6);  cout << std::left << (float) 1/alphaDen << "   ";
////             cout.width(7);  << std::left << (double) 1/alphaDen << "   "
//cout.width(3);  cout << std::left << (int) contextDepth << "   ";
//cout.width(8);  cout << std::left << averageEntropy << "   ";
//cout.width(8);  cout << std::left << averageEntropy/LOG2_ALPHABET_SIZE;
//    ////////////////////////////////
//
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
///// end of version: reference file not considered
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


//***********************************************************
//    build hash table (string key)
//************************************************************/
/* COMMENT

void FCM::buildHashTable_str ()
{
    const uint8_t contextDepth  = getContextDepth();    /// get context depth
    const uint16_t alphaDen     = getAlphaDenom();      /// get alpha denominator
    const bool isInvertedRepeat = getInvertedRepeat();  /// get inverted repeat
    /// TODO: supprt for both target and reference file addresses
    string fileName = getTarFileAddress();              /// get target file address


//    const char* filename= fileName.c_str();;
//    std::FILE *fp = std::fopen(filename, "rb");
//    if (fp)
//    {
//        std::string contents;
//        std::fseek(fp, 0, SEEK_END);
//        contents.resize(std::ftell(fp));
//        std::rewind(fp);
//        std::fread(&contents[ 0 ], 1, contents.size(), fp);
//        std::fclose(fp);
//    }


    ifstream fileIn(fileName, ios::in);         /// open file located in fileName

    if (!fileIn)                                /// error occurred while opening file
    {
        cerr << "The file '" << fileName << "' cannot be opened, or it is empty.\n";
        fileIn.close();                         /// close file
        return;                                 /// exit this function
    }

    string context(contextDepth, '0');          /// context, that slides in the dataset

    htable_str_t hTable;                        /// create hash table
    hTable.insert({context, {0, 0, 0, 0, 0}});  /// initialize hash table with 0'z

    ////////////////////////////////
    uint64_t nSym;                     /// number of symbols (n_s). To calculate probability
    uint64_t sumNSyms;                 /// sum of number of symbols (sum n_a). To calculate probability
    double   probability = 0;          /// probability of a symbol, based on an identified context
    double   sumOfEntropies = 0;       /// sum of entropies for different symbols
    uint64_t totalNumberOfSymbols = 0; /// number of all symbols in the sequence
    double   averageEntropy = 0;       /// average entropy (H)
    //////////////////////////////////

    string datasetLine;                /// keep each line of the file

    while (getline(fileIn, datasetLine))
    {

        //////////////////////////////////
        totalNumberOfSymbols += datasetLine.size();    /// number of symbols in each line of dataset
        //////////////////////////////////

        /// fill hash table by number of occurrences of symbols A, C, N, G, T
        for (string::iterator lineIter = datasetLine.begin(); lineIter != datasetLine.end(); ++lineIter)
        {
            /// htable includes an array of uint64_t numbers
            char c = *lineIter;
            uint8_t currSymInt = (c == 'A') ? (uint8_t) 0 :
                                 (c == 'C') ? (uint8_t) 1 :
                                 (c == 'G') ? (uint8_t) 3 :
                                 (c == 'T') ? (uint8_t) 4 : (uint8_t) 2;
//            const uint8_t currSymInt = c % 5;

            /// update hash table
            nSym = hTable[ context ][ currSymInt ]++;

            /// considering inverted repeats to update hash table
            if (isInvertedRepeat)
            {
                /// save inverted repeat context
                string invRepeatContext = to_string(4 - currSymInt);
                /// convert a number from char into integer format. '0'->0. '4'->4 by
                /// 4 - (context[ i ] - 48) = 52 - context[ i ]. 48 is ASCII code of '0'
                for (string::iterator it = context.end() - 1; it != context.begin(); --it)
                    invRepeatContext += to_string(52 - *it);
                /// update hash table considering inverted repeats
                ++hTable[ invRepeatContext ][ 52 - context[ 0 ]];
            }

            //////////////////////////////////
            /// sum(n_a)
            sumNSyms = 0;
            for (uint64_t u : hTable[ context ])    sumNSyms += u;

            /// P(s|c^t)
//            probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPHABET_SIZE/alphaDen);
            probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPHABET_SIZE);

            /// sum( log_2 P(s|c^t) )
            sumOfEntropies += log2(probability);
            /////////////////////////////////

            /// update context
            context = context.substr(1, (unsigned) contextDepth - 1) + to_string(currSymInt);

////            *context.end() = currSymInt;

//////            memcpy(context, context + 1, contextDepth - 1);
//////            context[ contextDepth-1 ] = currSymInt;
//////              *(context+contextDepth-1) = currSymInt;

        }   /// end of for
    }   /// end of while

    fileIn.close();                 /// close file

    FCM::setHashTable_str(hTable);  /// save the built hash table

    ////////////////////////////////
    /// H_N = -1/N sum( log_2 P(s|c^t) )
    averageEntropy = (-1) * sumOfEntropies / totalNumberOfSymbols;


//    cout << sumOfEntropies << '\n';
//    cout << totalNOfSyms << '\n';
//    cout << "  ";
    cout.width(2);  cout << std::left << getInvertedRepeat() << "   ";
    cout.width(6);  cout << std::left << (float) 1/alphaDen << "   ";
//             cout.width(7);  << std::left << (double) 1/alphaDen << "   "
    cout.width(3);  cout << std::left << (int) contextDepth << "   ";
    cout.width(8);  cout << std::left << averageEntropy << "   ";
    cout.width(8);  cout << std::left << averageEntropy/LOG2_ALPHABET_SIZE;
    ////////////////////////////////

}

*/ // end of COMMENT


/***********************************************************
    print hash table
************************************************************/
void FCM::printHashTable () const
{
    htable_t hTable = this->getHashTable();
//    htable_t hTable = getHashTable();
    
    cout
         << " >>> Context order size:\t" << (uint16_t) this->getContextDepth() << '\n'
         << " >>> Alpha denominator:\t\t" << (uint16_t) this->getAlphaDenom() << '\n'
         << " >>> Inverted repeat:\t\t" << (this->getInvertedRepeat() ? "Considered"
                                                                      : "Not considered")
         << '\n'
         << " >>> file address:\t"
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
char     FCM::getCompressionMode () const                   { return compressionMode;                        }
void     FCM::setCompressionMode (char cM)                  { FCM::compressionMode = cM;                     }
uint8_t  FCM::getContextDepth () const                      { return contextDepth;                    }
void     FCM::setContextDepth (uint8_t ctxDp)               { FCM::contextDepth = ctxDp;              }
uint16_t FCM::getAlphaDenom () const                        { return alphaDenom;                      }
void     FCM::setAlphaDenom (uint16_t alphaDen)             { FCM::alphaDenom = alphaDen;             }
//double FCM::getAlphaDenom () const                            { return alphaDenom;             }
//void FCM::setAlphaDenom (double alphaDen)                     { FCM::alphaDenom = alphaDen;    }
bool     FCM::getInvertedRepeat () const                    { return invertedRepeat;                  }
void     FCM::setInvertedRepeat (bool invRep)               { FCM::invertedRepeat = invRep;           }
uint64_t *FCM::getTable () const                            { return table;                           }
void     FCM::setTable (uint64_t *tbl)                      { FCM::table = tbl;                       }
const    htable_t &FCM::getHashTable () const               { return hashTable;                       }
void     FCM::setHashTable (const htable_t &hT)             { FCM::hashTable = hT;                    }
//const htable_str_t &FCM::getHashTable_str () const            { return hashTable_str;          }
//void FCM::setHashTable_str (const htable_str_t &hT_s)         { FCM::hashTable_str = hT_s;     }
const    vector<string> &FCM::getTarFilesAddresses () const { return tarFilesAddresses;               }
void     FCM::pushBackTarFilesAddresses (string tFAs)       { FCM::tarFilesAddresses.push_back(tFAs); }
const    vector<string> &FCM::getRefFilesAddresses () const { return refFilesAddresses;               }
void     FCM::pushBackRefFilesAddresses (string rFAs)       { FCM::refFilesAddresses.push_back(rFAs); }
