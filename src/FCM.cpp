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
    vector< string > refFilesNames = getRefAddresses();     /// reference file(s) address(es)
    U8 refsNumber = (U8) refFilesNames.size();              /// number of references

    /// set compression mode: 't'=table, 'h'=hash table. supports multi-references case
    compressionMode = ((U64) refsNumber > (U64) pow(ALPH_SIZE, TABLE_MAX_CTX - contextDepth)) ? 'h' : 't';
    
    /// check if reference(s) file(s) cannot be opened, or are empty
    ifstream refFilesIn[ refsNumber ];

    for (U8 i = refsNumber; i--;)
    {
        refFilesIn[ i ].open( refFilesNames[ i ], ios::in );
        if (!refFilesIn[ i ])               /// error occurred while opening file(s)
        {
            cerr << "The file '" << refFilesNames[ i ] << "' cannot be opened, or it is empty.\n";
            refFilesIn[ i ].close();        /// close file(s)
            return;                         /// exit this function
        }
    }
    
    U64 context;                       	    /// context (integer), that slides in the dataset
    U64 maxPlaceValue = (U64) pow(ALPH_SIZE, contextDepth);
    U64 invRepContext = maxPlaceValue - 1;  /// inverted repeat context (integer)
    
    U64 iRCtxCurrSym;                       /// concatenation of inverted repeat context and current symbol
    U8 currSymInt;                          /// current symbol integer
    
    string refLine;                         /// keep each line of a file
    
    switch ( compressionMode )              /// build model based on 't'=table, or 'h'=hash table
    {
        case 't':
        {
            U64 tableSize = refsNumber * maxPlaceValue * ALPH_SUM_SIZE; /// create table
            table = new U64[ tableSize ];                               /// already initialized with 0's
            /*
            /// initialize table with 0's
//            memset(table, 0, sizeof(table[ 0 ]) * tableSize);
//            std::fill_n(table,tableSize,(double) 1/alphaDenom);
            */
            
            for (U8 i = refsNumber; i--;)
            {
                context = 0;    /// reset in the beginning of each reference file
                
                while ( getline(refFilesIn[ i ], refLine) )
                {
                    /// fill table by number of occurrences of symbols A, C, N, G, T
                    for (string::iterator lineIter = refLine.begin(); lineIter != refLine.end(); ++lineIter)
                    {
                        currSymInt = symCharToInt(*lineIter);
                        
                        if (invertedRepeat)   /// considering inverted repeats to update table
                        {
                            /// concatenation of inverted repeat context and current symbol
                            iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
                            /// update inverted repeat context (integer)
                            invRepContext = (U64) iRCtxCurrSym / ALPH_SIZE;
                            
                            /// update table, including 'sum' column, considering inverted repeats
                            updateTable( invRepContext * ALPH_SUM_SIZE, iRCtxCurrSym % ALPH_SIZE );
                        }
                        
                        updateTable( context * ALPH_SUM_SIZE, currSymInt ); /// update table, including 'sum' column
                        context = (U64) (context * ALPH_SIZE + currSymInt) % maxPlaceValue; /// update context
                    }
                }
            }   /// end for
        }   /// end case
            break;
            
        case 'h':               /// adding 'sum' column, makes hash table slower
        {
            for (int i = refsNumber; i--;)
            {
                context = 0;    /// reset in the beginning of each reference file
                
                while ( getline(refFilesIn[ i ], refLine) )
                {
                    /// fill hash table by number of occurrences of symbols A, C, N, G, T
                    for (string::iterator lineIter = refLine.begin(); lineIter != refLine.end(); ++lineIter)
                    {
                        currSymInt = symCharToInt(*lineIter);
                        
                        /// considering inverted repeats to update hash table
                        if (invertedRepeat)
                        {
                            /// concatenation of inverted repeat context and current symbol
                            iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
                            /// update inverted repeat context (integer)
                            invRepContext = (U64) iRCtxCurrSym / ALPH_SIZE;

                            /// update hash table considering inverted repeats
                            ++hashTable[ invRepContext ][ iRCtxCurrSym % ALPH_SIZE ];
                        }
                        
                        ++hashTable[ context ][ currSymInt ];                               /// update hash table
                        context = (U64) (context * ALPH_SIZE + currSymInt) % maxPlaceValue; /// update context
                    }
                }
            }   /// end for
        }   /// end case
            break;

        default: break;
    }   /// end switch

    for (U8 i = refsNumber; i--;)  refFilesIn[i].close();   /// close file(s)
}


/***********************************************************
    compress target(s) based on reference(s) model
************************************************************/
void FCM::compressTarget (string tarFileName)
{
    const double alpha = (double) 1/alphaDenom; /// alpha -- used in P denominator
    const double sumAlphas = ALPH_SIZE * alpha; /// used in P numerator
    
    ifstream tarFileIn( tarFileName, ios::in ); /// open target file
    
    /// mutex lock ========================================================
    mut.lock();
    if (!tarFileIn)                             /// error occurred while opening file
    {
        cerr << "The file '" << tarFileName << "' cannot be opened, or it is empty.\n";
        tarFileIn.close();                      /// close file
        return;                                 /// exit this function
    }
    mut.unlock();
    /// mutex unlock ======================================================
    
    U64 maxPlaceValue = (U64) pow(ALPH_SIZE, contextDepth);
    U64 tarContext = 0;                         /// context (integer), that slides in the dataset
    
    string tarLine;                             /// keep each line of the file
    
    ////////////////////////////////
    U64    nSym;                                /// number of symbols (n_s). To calculate probability
    U64    sumNSyms;                            /// sum of number of symbols (sum n_a). To calculate probability
    double probability = 0;                     /// probability of a symbol, based on an identified context
    double sumOfEntropies = 0;                  /// sum of entropies for different symbols
    U64    totalNOfSyms = 0;                    /// number of all symbols in the sequence
    double averageEntropy = 0;                  /// average entropy (H)
    ////////////////////////////////
    
    /*
    /// using macros make this code slower
    #define X \
         ((compressionMode == 'h') ? (hashTable[ tarContext ][ currSymInt ]) \
                                   : (table[ tarContext * ALPH_SUM_SIZE + currSymInt ]))
    #define Y(in) do { \
                (compressionMode == 't') \
                ? in = table[ tarContext * ALPH_SUM_SIZE + ALPH_SIZE ] \
                : in = 0; for (U64 u : hashTable[ tarContext ]) in += u; \
              } while ( 0 )
    */
    
    switch ( compressionMode )
    {
        case 't':
        {
            while (getline(tarFileIn, tarLine))
            {
                
                //////////////////////////////////
                totalNOfSyms = totalNOfSyms + tarLine.size();   /// number of symbols in each line of dataset
                //////////////////////////////////
        
                /// table includes the number of occurrences of symbols A, C, N, G, T
                for (string::iterator lineIter = tarLine.begin(); lineIter != tarLine.end(); ++lineIter)
                {
                    U8 currSymInt = symCharToInt(*lineIter);   /// integer version of the current symbol
                    
                    //////////////////////////////////
                    nSym = table[ tarContext * ALPH_SUM_SIZE + currSymInt ];    /// number of symbols
//                    nSym = X;
                    sumNSyms = table[ tarContext * ALPH_SUM_SIZE + ALPH_SIZE ]; /// sum of number of symbols
//                    Y(sumNSyms);
                    
//                    probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPH_SIZE);
                    probability = (nSym + alpha) / (sumNSyms + sumAlphas);      /// P(s|c^t)
                    sumOfEntropies = sumOfEntropies + log2(probability);        /// sum( log_2 P(s|c^t) )
                    /////////////////////////////////
                    
                    tarContext = (U64) (tarContext * ALPH_SIZE + currSymInt) % maxPlaceValue;   /// update context
                }
            }   /// end while
        }   /// end case
        break;

        case 'h':
        {
            while (getline(tarFileIn, tarLine))
            {

                //////////////////////////////////
                totalNOfSyms = totalNOfSyms + tarLine.size();   /// number of symbols in each line of dataset
                //////////////////////////////////

                /// hash table includes the number of occurrences of symbols A, C, N, G, T
                for (string::iterator lineIter = tarLine.begin(); lineIter != tarLine.end(); ++lineIter)
                {
                    U8 currSymInt = symCharToInt(*lineIter);   /// integer version of the current symbol

                    //////////////////////////////////
//                    if (hTable.find(tarContext) == hTable.end()) { nSym = 0;   sumNSyms = 0; }
//                    else
//                    {
                        nSym = hashTable[ tarContext ][ currSymInt ];       /// number of symbols
                        /*
                        nSym = X;
                        X(nSym);
                        */
                        sumNSyms = 0; for (U64 u : hashTable[ tarContext ])   sumNSyms = sumNSyms + u;  /// sum(n_a)
                        /*
                        Y(sumNSyms);
                        */
//                    }
//                    probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPH_SIZE);
                    probability = (nSym + alpha) / (sumNSyms + sumAlphas);  /// P(s|c^t)
                    sumOfEntropies = sumOfEntropies + log2(probability);    /// sum( log_2 P(s|c^t) )
                    /////////////////////////////////

                    tarContext = (U64) (tarContext * ALPH_SIZE + currSymInt) % maxPlaceValue;   /// update context
                }
            }   /// end while
        }   /// end case
        break;

        default: break;
    }   /// end switch
    
    tarFileIn.close();  /// close file
    
    ////////////////////////////////
    averageEntropy = (double) (-1) * sumOfEntropies / totalNOfSyms;     /// H_N = -1/N sum( log_2 P(s|c^t) )

//    cout << sumOfEntropies << '\n';
//    cout << totalNOfSyms << '\n';
//    cout << ' ';
    
    /// to print reference and target file names in the output
    U8 refsAdressesSize = (U8) getRefAddresses().size();
    size_t lastSlash_Ref[ refsAdressesSize ];
    for (U8 i = refsAdressesSize; i--;)
        lastSlash_Ref[ i ] = getRefAddresses()[ i ].find_last_of("/");
    size_t lastSlash_Tar = tarFileName.find_last_of("/");
    
    /// mutex lock ========================================================
    mut.lock();
    
    for (int i = refsAdressesSize - 1; i; --i)
        cout << getRefAddresses()[ i ].substr(lastSlash_Ref[ i ] + 1) << ',';
    cout << getRefAddresses()[ 0 ].substr(lastSlash_Ref[ 0 ] + 1) << '\t'
         << tarFileName.substr(lastSlash_Tar + 1) << '\t';
    
    cout << invertedRepeat << '\t'
         << std::fixed << setprecision(4) << alpha << '\t'
         << (int) contextDepth << '\t'
         << std::fixed << setprecision(5) << averageEntropy << '\t'
         << std::fixed << setprecision(5) << averageEntropy/LOG2_ALPH_SIZE;
    
//    cout.width(2);  cout << std::left << getInvertedRepeat() << "   ";
    
    cout << '\n';
    
    mut.unlock();
    /// mutex unlock ======================================================
    ////////////////////////////////
    
}


/***********************************************************
    convert char (base) to integer (U8): ACNGT -> 01234
************************************************************/
inline U8 FCM::symCharToInt (char ch) const
{
    switch (ch)
    {
        case 'A':   return 0;
        case 'C':   return 1;
        case 'G':   return 3;
        case 'T':   return 4;
        default:    return 2;  /// 'N' symbol
    }
    
//    switch (ch)
//    {
//        case 'A':   return (U8) 0;
//        case 'C':   return (U8) 1;
//        case 'G':   return (U8) 3;
//        case 'T':   return (U8) 4;
//        default:    return (U8) 2;  /// 'N' symbol
//    }

//    return (U8) (ch % ALPH_SIZE);

//    switch (ch)
//    {
//        case 'C':   return (U8) 3;
//        case 'N':   return (U8) 2;
//        default:    return (U8) (ch % ALPH_SIZE);
//    }
}


/***********************************************************
    update table, including 'sum' column
************************************************************/
inline void FCM::updateTable (U64 rowIndex, U64 column)
{
    ++table[ rowIndex + column ];    /// update table
    ++table[ rowIndex + ALPH_SIZE ]; /// update 'sum' column
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
//    const U8 contextDepth  = getContextDepth();    /// get context depth
//    const U16 alphaDen     = getAlphaDenom();      /// get alpha denominator
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
//    /// 5^TABLE_MAX_CTX < 2^32 => U32 is used, otherwise U64
//    U32 maxPlaceValue = (U32) pow(ALPH_SIZE, contextDepth);
//    U64 tableSize = maxPlaceValue * ALPH_SUM_SIZE;
//    U64 *table = new U64[ tableSize ];
//
//    /// initialize table with 0's
//    memset(table, 0, sizeof(table[0]) * tableSize);
//
//    U32 context = 0;                       /// context (integer), that slides in the dataset
//    U32 invRepContext = maxPlaceValue - 1; /// inverted repeat context (integer)
//
//    ////////////////////////////////
//    U64 nSym;                      /// number of symbols (n_s). To calculate probability
//    U64 sumNSyms;                  /// sum of number of symbols (sum n_a). To calculate probability
//    double   probability = 0;           /// probability of a symbol, based on an identified context
//    double   sumOfEntropies = 0;        /// sum of entropies for different symbols
//    U64 totalNumberOfSymbols = 0;  /// number of all symbols in the sequence
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
//            /// htable includes an array of U64 numbers
//            char ch = *lineIter;
//            U8 currSymInt = (U8) ((ch == 'A') ? 0 :
//                                            (ch == 'C') ? 1 :
//                                            (ch == 'G') ? 3 :
//                                            (ch == 'T') ? 4 : 2);
//
////            U8 currSymInt = (ch == 'A') ? (U8) 0 :
////                                 (ch == 'C') ? (U8) 1 :
////                                 (ch == 'G') ? (U8) 3 :
////                                 (ch == 'T') ? (U8) 4 : (U8) 2;
////            U8 currSymInt = ch % ALPHABET_SIZE;
////            U8 currSymInt = (ch == 'C') ? (U8) 3 :
////                                 (ch == 'N') ? (U8) 2 :
////                                 (U8) (ch % ALPHABET_SIZE);
//
//            /// update table
//            nSym = table[ context * ALPH_SUM_SIZE + currSymInt ]++;
//
//            /// considering inverted repeats to update hash table
//            if (isInvertedRepeat)
//            {
//                /// concatenation of inverted repeat context and current symbol
//                U32 iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
//
////                /// to save quotient and reminder of a division
////                div_t iRCtxCurrSymDiv;
////                iRCtxCurrSymDiv = div(iRCtxCurrSym, ALPHABET_SIZE);
//
//                /// update inverted repeat context (integer)
////                invRepContext = (U32) iRCtxCurrSymDiv.quot;
//                invRepContext = (U32) iRCtxCurrSym / ALPH_SIZE;
//
//                /// update table considering inverted repeats
////                ++table[ invRepContext*ALPHABET_SIZE + iRCtxCurrSymDiv.rem ];
////                ++table[ invRepContext * ALPHABET_SIZE + iRCtxCurrSym % ALPHABET_SIZE ];
//                ++table[ invRepContext * ALPH_SUM_SIZE + iRCtxCurrSym % ALPH_SIZE ];
//                ++table[ invRepContext * ALPH_SUM_SIZE + ALPH_SIZE ];
//            }
//
//            //////////////////////////////////
//            /// sum(n_a)
////            U64 *pointerToTable = table;   /// pointer to the beginning of table
////            sumNSyms = 0;
////            for (U8 i = 0; i < ALPHABET_SIZE; ++i)
////                sumNSyms += *(pointerToTable + context*ALPHABET_SIZE + i);
//            sumNSyms = ++table[ context * ALPH_SUM_SIZE + ALPH_SIZE ];
//
//            /// P(s|c^t)
////            probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPHABET_SIZE/alphaDen);
//            probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPH_SIZE);
//
//            /// sum( log_2 P(s|c^t) )
//            sumOfEntropies += log2(probability);
//            /////////////////////////////////
//
//            /// update context
//            context = (U32) (context * ALPH_SIZE + currSymInt) % maxPlaceValue;
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
//cout.width(8);  cout << std::left << averageEntropy/LOG2_ALPH_SIZE;
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
//    const U8 contextDepth  = getContextDepth();    /// get context depth
//    const U16 alphaDen     = getAlphaDenom();      /// get alpha denominator
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
//    U64 context = 0;                       /// context, that slides in the dataset
//    U64 maxPlaceValue = (U64) pow(ALPH_SIZE, contextDepth);
//    U64 invRepContext = maxPlaceValue - 1; /// inverted repeat context
//
//    htable_t hTable;                            /// create hash table
//    hTable.insert({context, {0, 0, 0, 0, 0}});  /// initialize hash table with 0's
//
//    //////////////////////////////////
//    U64 nSym;                     /// number of symbols (n_s). To calculate probability
//    U64 sumNSyms;                 /// sum of number of symbols (sum n_a). To calculate probability
//    double   probability = 0;          /// probability of a symbol, based on an identified context
//    double   sumOfEntropies = 0;       /// sum of entropies for different symbols
//    U64 totalNumberOfSymbols = 0; /// number of all symbols in the sequence
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
//            /// htable includes an array of U64 numbers
//            char ch = *lineIter;
//            U8 currSymInt = (U8) ((ch == 'A') ? 0 :
//                                            (ch == 'C') ? 1 :
//                                            (ch == 'G') ? 3 :
//                                            (ch == 'T') ? 4 : 2);
//
////            U8 currSymInt = (ch == 'A') ? (U8) 0 :
////                                 (ch == 'C') ? (U8) 1 :
////                                 (ch == 'G') ? (U8) 3 :
////                                 (ch == 'T') ? (U8) 4 : (U8) 2;
////            U8 currSymInt = ch % 5;
////            U8 currSymInt = (ch == 'C') ? (U8) 3 :
////                                 (ch == 'N') ? (U8) 2 :
////                                 (U8) (ch % ALPHABET_SIZE);
//
//            /// update hash table
//            nSym = hTable[ context ][ currSymInt ]++;
//
//            /// considering inverted repeats to update hash table
//            if (isInvertedRepeat)
//            {
//                /// concatenation of inverted repeat context and current symbol
//                U64 iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
//
//                /// to save quotient and reminder of a division
////                div_t iRCtxCurrSymDiv;
////                iRCtxCurrSymDiv = div(iRCtxCurrSym, ALPHABET_SIZE);
//
//                /// update inverted repeat context (integer)
////                invRepContext =  iRCtxCurrSymDiv.quot;
//                invRepContext = (U64) iRCtxCurrSym / ALPH_SIZE;
//
//                /// update table considering inverted repeats
////                ++hTable[ invRepContext ][ iRCtxCurrSymDiv.rem];
//                ++hTable[ invRepContext ][ iRCtxCurrSym % ALPH_SIZE ];
//            }
//
//            //////////////////////////////////
//            /// the idea of adding 'sum' column, makes hash table slower
//            /// sum(n_a)
//            sumNSyms = 0;
//            for (U64 u : hTable[ context ])    sumNSyms += u;
//
//            /// P(s|c^t)
////            probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPHABET_SIZE/alphaDen);
//            probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPH_SIZE);
//
//            /// sum( log_2 P(s|c^t) )
//            sumOfEntropies += log2(probability);
//            /////////////////////////////////
//
//            /// update context
//            context = (U64) (context * ALPH_SIZE + currSymInt) % maxPlaceValue;
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
//cout.width(8);  cout << std::left << averageEntropy/LOG2_ALPH_SIZE;
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
    const U8 contextDepth  = getContextDepth();    /// get context depth
    const U16 alphaDen     = getAlphaDenom();      /// get alpha denominator
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
    U64 nSym;                     /// number of symbols (n_s). To calculate probability
    U64 sumNSyms;                 /// sum of number of symbols (sum n_a). To calculate probability
    double   probability = 0;          /// probability of a symbol, based on an identified context
    double   sumOfEntropies = 0;       /// sum of entropies for different symbols
    U64 totalNumberOfSymbols = 0; /// number of all symbols in the sequence
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
            /// htable includes an array of U64 numbers
            char c = *lineIter;
            U8 currSymInt = (c == 'A') ? (U8) 0 :
                                 (c == 'C') ? (U8) 1 :
                                 (c == 'G') ? (U8) 3 :
                                 (c == 'T') ? (U8) 4 : (U8) 2;
//            const U8 currSymInt = c % 5;

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
            for (U64 u : hTable[ context ])    sumNSyms += u;

            /// P(s|c^t)
//            probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPH_SIZE/alphaDen);
            probability = (double) (alphaDen * nSym + 1) / (alphaDen * sumNSyms + ALPH_SIZE);

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
    cout.width(8);  cout << std::left << averageEntropy/LOG2_ALPH_SIZE;
    ////////////////////////////////

}

*/ // end of COMMENT


/***********************************************************
    print hash table
************************************************************/
void FCM::printHashTable () const
{
    htable_t hTable = hashTable;
    
    cout
         << " >>> Context order size:\t" << (U16) contextDepth << '\n'
         << " >>> Alpha denominator:\t\t" << (U16) alphaDenom << '\n'
         << " >>> Inverted repeat:\t\t" << (invertedRepeat ? "Considered"
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
        for (U64 i : it->second)    cout << i << "\t";
        cout << '\n';
    }
    cout << '\n';
}


/***********************************************************
    getters and setters
************************************************************/
void  FCM::setParams (U16 aD, U8 ctx, bool iR)       { FCM::alphaDenom = aD;
                                                       FCM::contextDepth = ctx;
                                                       FCM::invertedRepeat = iR;          }
const vector<string> &FCM::getTarAddresses () const  { return tarAddresses;               }
void  FCM::pushBackTarAddresses (const string &tFAs) { FCM::tarAddresses.push_back(tFAs); }
const vector<string> &FCM::getRefAddresses () const  { return refAddresses;               }
void  FCM::pushBackRefAddresses (const string &rFAs) { FCM::refAddresses.push_back(rFAs); }
