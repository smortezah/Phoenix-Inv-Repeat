#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>      /// setw, setprecision
#include <cstring>      /// memset, memcpy
#include <stdlib.h>
#include <vector>

#include "FCM.h"
#include "bitio.h"
#include "bitio.c"
#include "arith.h"
#include "arith.c"
#include "arith_aux.h"
#include "arith_aux.c"

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
using std::fill_n;

// TODO: TEST
//using std::chrono::high_resolution_clock;


/***********************************************************
    constructor
************************************************************/
FCM::FCM ()
{
    n_threads  = DEFAULT_N_THREADS;                 /// number of threads
    gamma      = DEFAULT_GAMMA;                     /// gamma
    decompFlag = false;                             /// decompression flag
}


/***********************************************************
    build reference(s) model
************************************************************/
void FCM::buildModel (bool invRepeat, U8 ctxDepth, U8 modelIndex)
{
    vector< string > refFilesNames = getRefAddr();  /// reference file(s) address(es)
    U8 refsNumber = (U8) refFilesNames.size();      /// number of references

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
////    U64 maxPlaceValue = (U64) pow(ALPH_SIZE, ctxDepth);
    U64 maxPlaceValue = POWER5[ctxDepth];
    U64 invRepContext = maxPlaceValue - 1;  /// inverted repeat context (integer)
                                            
    U64 iRCtxCurrSym;                       /// concat of inverted repeat context and current symbol
    U8  currSymInt;                         /// current symbol integer
                                            
    string refLine;                         /// keep each line of a file
                                            
    switch ( compMode )                     /// build model based on 't'=table, or 'h'=hash table
    {
        case 't':
        {
            U64 tableSize = maxPlaceValue * ALPH_SUM_SIZE;
            U64 *table = new U64[ tableSize ];  /// already initialized with 0's
            /*
            /// initialize table with 0's
//            memset(table, 0, sizeof(table[ 0 ]) * tableSize);
//            std::fill_n(table,tableSize,(double) 1/alphaDenom);
            */
            U64 rowIndex;                   /// to update table
            
            for (U8 i = refsNumber; i--;)
            {
                context = 0;                /// reset in the beginning of each reference file

                while ( getline(refFilesIn[ i ], refLine) )
                {
                    /// fill table by number of occurrences of symbols A, C, N, G, T
                    for (string::iterator lineIt = refLine.begin(); lineIt != refLine.end(); ++lineIt)
                    {
                        currSymInt = symCharToInt(*lineIt);

                        if (invRepeat)      /// considering inverted repeats to update table
                        {
                            /// concatenation of inverted repeat context and current symbol
                            iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
                            /// update inverted repeat context (integer)
                            invRepContext = (U64) iRCtxCurrSym / ALPH_SIZE;

                            /// update table, including 'sum' column, considering inverted repeats
                            rowIndex = invRepContext * ALPH_SUM_SIZE;
                            ++table[ rowIndex + iRCtxCurrSym % ALPH_SIZE ]; /// update table
                            ++table[ rowIndex + ALPH_SIZE ];                /// update 'sum' column
                        }
                        
                        rowIndex = context * ALPH_SUM_SIZE;
                        ++table[ rowIndex + currSymInt ];                   /// update table
                        ++table[ rowIndex + ALPH_SIZE ];                    /// update 'sum' column
                        
                        /// update context. (rowIndex - context) == (context * ALPH_SIZE)
                        context = (U64) (rowIndex - context + currSymInt) % maxPlaceValue;
                    }
                }   /// end while
            }   /// end for
    
            mut.lock(); setTable(table, modelIndex);    mut.unlock();       /// set table
        }   /// end case
            break;
            
        case 'h':       /// adding 'sum' column, makes hash table slower
        {
            htable_t hashTable;
            
            for (int i = refsNumber; i--;)
            {
                context = 0;    /// reset in the beginning of each reference file

                while ( getline(refFilesIn[ i ], refLine) )
                {
                    /// fill hash table by number of occurrences of symbols A, C, N, G, T
                    for (string::iterator lineIt = refLine.begin(); lineIt != refLine.end(); ++lineIt)
                    {
                        currSymInt = symCharToInt(*lineIt);

                        /// considering inverted repeats to update hash table
                        if (invRepeat)
                        {
                            /// concatenation of inverted repeat context and current symbol
                            iRCtxCurrSym = (4 - currSymInt) * maxPlaceValue + invRepContext;
                            /// update inverted repeat context (integer)
                            invRepContext = (U64) iRCtxCurrSym / ALPH_SIZE;

                            /// update hash table considering inverted repeats
                            ++hashTable[ invRepContext ][ iRCtxCurrSym % ALPH_SIZE ];
                        }
                        
                        ++hashTable[ context ][ currSymInt ];   /// update hash table
                        /// update context
                        context = (U64) (context * ALPH_SIZE + currSymInt) % maxPlaceValue;
                    }
                }
            }   /// end for
    
            mut.lock();  setHashTable(hashTable, modelIndex);  mut.unlock();    /// set hash table
        }   /// end case
            break;
            
        default: break;
    }   /// end switch
    
    for (U8 i = refsNumber; i--;)  refFilesIn[i].close();       /// close file(s)
}


/***********************************************************
    compress target(s) based on reference(s) model
************************************************************/
void FCM::compress (const string &tarFileName)
{
    /// alpha and ALPH_SIZE*alpha: used in P numerator and denominator
    double alpha[ n_models ], sumAlphas[ n_models ];
    for (U8 i = n_models; i--;)
    {
        alpha[ i ] = (double) 1 / alphaDens[ i ];
        sumAlphas[ i ] = ALPH_SIZE * alpha[ i ];
    }

    /// open target file
    ifstream tarFileIn( tarFileName, ios::in );

    mut.lock();///========================================================
    if (!tarFileIn)                     /// error occurred while opening file
    {
        cerr << "The file '" << tarFileName << "' cannot be opened, or it is empty.\n";
        tarFileIn.close();              /// close file
        return;                         /// exit this function
    }
    mut.unlock();///======================================================

    U64 maxPlaceValue[ n_models ];
////    for (U8 i = n_models; i--;)  maxPlaceValue[ i ] = (U64) pow( ALPH_SIZE, ctxDepths[ i ] );
    for (U8 i = n_models; i--;)  maxPlaceValue[ i ] = POWER5[ ctxDepths[i] ];
    /// context(s) (integer) sliding through the dataset
    U64     tarContext[ n_models ];     fill_n(tarContext, n_models, 0);
    string  tarLine;                    /// keep each line of the file
    U64     nSym;                       /// number of symbols (n_s). in probability numerator
    U64     sumNSym;                    /// sum of number of symbols (sum n_a). in probability denominator
    double  prob_i;                     /// probability of a symbol for each model
    double  rawWeight[ n_models ];      /// weight before normalization for each model. init: 1/M
    double  weight[ n_models ];  fill_n(weight, n_models, (double) 1 / n_models);   /// each model weight
    double  probability;                /// final probability of a symbol
    double  sumOfEntropies;             /// sum of entropies for different symbols
    U64     n_fileLines = 0;            /// number of file lines
    U64     file_size = fileSize(tarFileName);      /// size of file, including '\n' symbols
    double  averageEntropy = 0;         /// average entropy (H)
    double  sumOfWeights;               /// sum of weights. used for normalization
    double  freqsDouble[ ALPH_SIZE ];   /// frequencies of each symbol (double)
    int     freqs[ ALPH_SIZE ];         /// frequencies of each symbol (integer)
    U64     sumFreqs;

    /*
    /// using macros make this code slower
    #define X \
         ((compMode == 'h') ? (hashTable[ tarContext ][ currSymInt ]) \
                                   : (table[ tarContext * ALPH_SUM_SIZE + currSymInt ]))
    #define Y(in) do { \
                (compMode == 't') \
                ? in = table[ tarContext * ALPH_SUM_SIZE + ALPH_SIZE ] \
                : in = 0; for (U64 u : hashTable[ tarContext ]) in += u; \
              } while ( 0 )
    */

    size_t lastSlash_Tar = tarFileName.find_last_of("/");           /// find the position of last slash
    string tarNamePure = tarFileName.substr(lastSlash_Tar + 1);     /// target file name without slash
    const char *tar = (tarNamePure + ".co").c_str();                /// convert string to char*

//    FILE *Writer = fopen(tar, "w");     /// to save compressed file
//
//    startoutputtingbits();              /// start arithmetic encoding process
//    start_encode();
//
//    /// model(s) properties, to be sent to decoder
//    WriteNBits( WATERMARK,                26, Writer );
//    WriteNBits( file_size,                46, Writer );
//    WriteNBits( (int) (gamma * 65536),    32, Writer );
//    WriteNBits( n_models,                 16, Writer );
//    for (U8 n = 0; n < n_models; ++n)
//    {
//        WriteNBits( (U8) invRepeats[ n ],  1, Writer );
//        WriteNBits( ctxDepths[ n ],       16, Writer );
//        WriteNBits( alphaDens[ n ],       16, Writer );
////        WriteNBits( compMode,           1, Writer );
//    }
    
    switch ( compMode )
    {
        case 't':
        {
            U64 rowIndex;                   /// index of a row in the table
            sumOfEntropies = 0;             /// sum of entropies

            while ( getline(tarFileIn, tarLine) )
            {
                ++n_fileLines;                              /// number of file lines

                /// table includes the number of occurrences of symbols A, C, N, G, T
                for (string::iterator lineIt = tarLine.begin(); lineIt != tarLine.end(); ++lineIt)
                {
                    fill_n( freqsDouble, ALPH_SIZE, 0 );    /// reset array of frequencies

                    U8 currSymInt = symCharToInt(*lineIt);  /// integer version of the current symbol

                    probability  = 0;
                    sumOfWeights = 0;

                    for (U8 i = n_models; i--;)
                    {
                        rowIndex = tarContext[ i ] * ALPH_SUM_SIZE;

                        /// frequencies (double)
////                        for (U8 j = ALPH_SIZE; j--;)
////                            freqsDouble[ j ] += weight[ i ] * tables[ i ][ rowIndex + j ];
                        freqsDouble[ 0 ] += weight[ i ] * tables[ i ][ rowIndex ];
                        freqsDouble[ 1 ] += weight[ i ] * tables[ i ][ rowIndex + 1 ];
                        freqsDouble[ 2 ] += weight[ i ] * tables[ i ][ rowIndex + 2 ];
                        freqsDouble[ 3 ] += weight[ i ] * tables[ i ][ rowIndex + 3 ];
                        freqsDouble[ 4 ] += weight[ i ] * tables[ i ][ rowIndex + 4 ];

                        nSym = tables[ i ][ rowIndex + currSymInt ];    /// number of symbols
////                          nSym = X;
                        sumNSym = tables[ i ][ rowIndex + ALPH_SIZE ];  /// sum of number of symbols
////                          Y(sumNSyms);
                        prob_i = (nSym + alpha[ i ]) / (sumNSym + sumAlphas[ i ]);  /// P(s|c^t)
                        probability = probability + weight[ i ] * prob_i;       /// P_1*W_1 + P_2*W_2 + ...

                        /// weight before normalization
                        rawWeight[ i ] = fastPow(weight[ i ], gamma) * prob_i;
                        sumOfWeights = sumOfWeights + rawWeight[ i ];   /// sum of weights. used for normalization

                        /// update context. (rowIndex - tarContext[i]) == (tarContext[i] * ALPH_SIZE)
                        tarContext[ i ] = (U64) (rowIndex - tarContext[i] + currSymInt) % maxPlaceValue[ i ];
////                        tarContext[ i ] = (U64) (tarContext[i] * ALPH_SIZE + currSymInt) % maxPlaceValue[ i ];
                    }
                    /// update weights
                    for (U8 i = n_models; i--;) weight[ i ] = rawWeight[ i ] / sumOfWeights;

                    sumOfEntropies = sumOfEntropies + log2(probability);            /// sum( log_2 P(s|c^t) )

                    /// frequencies (integer)
////                    for (U8 j = ALPH_SIZE; j--;) freqs[ j ] = (int) (1 + (freqsDouble[j] * DOUBLE_TO_INT));
                    freqs[ 0 ] = (int) (1 + (freqsDouble[0] * DOUBLE_TO_INT));
                    freqs[ 1 ] = (int) (1 + (freqsDouble[1] * DOUBLE_TO_INT));
                    freqs[ 2 ] = (int) (1 + (freqsDouble[2] * DOUBLE_TO_INT));
                    freqs[ 3 ] = (int) (1 + (freqsDouble[3] * DOUBLE_TO_INT));
                    freqs[ 4 ] = (int) (1 + (freqsDouble[4] * DOUBLE_TO_INT));
                    
                    sumFreqs = 0;   for (int f : freqs) sumFreqs += f;  /// sum of frequencies


//                    for (int j = 0; j < 30; ++j)
//                        cout << tables[ 0 ][ j ] << ' ';
//                    cout << '\n';


//                    for(U8 i=0;i<ALPH_SIZE;++i)printf("%d\t",freqs[i]);printf("***\n");

                    
                    
                    
                    
//                    AESym( currSymInt, freqs, (int) sumFreqs, Writer ); /// Arithmetic encoder
                }   /// end for
            }   /// end while
        }   /// end case
        break;

        case 'h':
        {
            array< U64, ALPH_SIZE > hTRowArray;     /// hash table row array -- to save a row of hTable
            sumOfEntropies = 0;                     /// sum of entropies

            while ( getline(tarFileIn, tarLine) )
            {
                ++n_fileLines;                              /// number of file lines

                /// hash table includes the number of occurrences of symbols A, C, N, G, T
                for (string::iterator lineIt = tarLine.begin(); lineIt != tarLine.end(); ++lineIt)
                {
                    fill_n( freqsDouble, ALPH_SIZE, 0 );    /// reset array of frequencies

                    U8 currSymInt = symCharToInt(*lineIt);  /// integer version of the current symbol

                    probability  = 0;
                    sumOfWeights = 0;

                    for (U8 i = n_models; i--;)
                    {
                        /// save the row of hash table into an array
                        hTRowArray = hashTables[ i ][ tarContext[i] ];

                        /// frequencies (double)
////                        for (U8 j = ALPH_SIZE; j--;) freqsDouble[ j ] += weight[ i ] * hTRowArray[ j ];
                        freqsDouble[ 0 ] += weight[ i ] * hTRowArray[ 0 ];
                        freqsDouble[ 1 ] += weight[ i ] * hTRowArray[ 1 ];
                        freqsDouble[ 2 ] += weight[ i ] * hTRowArray[ 2 ];
                        freqsDouble[ 3 ] += weight[ i ] * hTRowArray[ 3 ];
                        freqsDouble[ 4 ] += weight[ i ] * hTRowArray[ 4 ];

                        /// sum of number of symbols
                        sumNSym = 0;    for (U64 u : hTRowArray) sumNSym = sumNSym + u;
////                        Y(sumNSym);
                        nSym = hTRowArray[ currSymInt ];        /// number of symbols
////                          nSym = X;
////                          X(nSym);
                        prob_i = (nSym + alpha[ i ]) / (sumNSym + sumAlphas[ i ]);  /// P(s|c^t)
                        probability = probability + weight[ i ] * prob_i;   /// P_1*W_1 + P_2*W_2 + ...

                        /// weight before normalization
                        rawWeight[ i ] = fastPow(weight[ i ], gamma) * prob_i;
                        sumOfWeights = sumOfWeights + rawWeight[ i ];       /// sum of weights. used for normalization

                        /// update context
                        tarContext[ i ] = (U64) (tarContext[i] * ALPH_SIZE + currSymInt) % maxPlaceValue[ i ];
                    }
                    /// update weights
                    for (U8 i = n_models; i--;) weight[ i ] = rawWeight[ i ] / sumOfWeights;

                    sumOfEntropies = sumOfEntropies + log2(probability);    /// sum( log_2 P(s|c^t) )

                    /// frequencies (integer)
////                    for (U8 j = ALPH_SIZE; j--;) freqs[ j ] = (int) (1 + (freqsDouble[j] * DOUBLE_TO_INT));
                    freqs[ 0 ] = (int) (1 + (freqsDouble[0] * DOUBLE_TO_INT));
                    freqs[ 1 ] = (int) (1 + (freqsDouble[1] * DOUBLE_TO_INT));
                    freqs[ 2 ] = (int) (1 + (freqsDouble[2] * DOUBLE_TO_INT));
                    freqs[ 3 ] = (int) (1 + (freqsDouble[3] * DOUBLE_TO_INT));
                    freqs[ 4 ] = (int) (1 + (freqsDouble[4] * DOUBLE_TO_INT));
                    
                    sumFreqs = 0;   for (int f : freqs) sumFreqs += f;      /// sum of frequencies

//                    AESym( currSymInt, freqs, (int) sumFreqs, Writer );     /// Arithmetic encoding
                }   /// end for
            }   /// end while
        }   /// end case
        break;

        default: break;
    }   /// end switch
    
//    finish_encode( Writer );
//    doneoutputtingbits( Writer );   /// encode the last bit
//    fclose( Writer );               /// close compressed file
    
    tarFileIn.close();              /// close target file
    
    /// (file_size - n_fileLines) is number of symbols in file
    /// n_fileLines is number of '\n's, which are accounted in file_size
    /// H_N = -1/N sum( log_2 P(s|c^t) )
    averageEntropy = (double) (-1) * sumOfEntropies / (file_size - n_fileLines);

    /// print reference and target file names
    U8 refsAdressesSize = (U8) getRefAddr().size();
    size_t lastSlash_Ref[ refsAdressesSize ];
    string refNamesPure[ refsAdressesSize ];
    for (U8 i = refsAdressesSize; i--;)
    {
        lastSlash_Ref[ i ] = getRefAddr()[ i ].find_last_of("/");
        refNamesPure[i]    = getRefAddr()[ i ].substr(lastSlash_Ref[ i ] + 1);
    }

    mut.lock();///========================================================
    for (int i = 0; i < refsAdressesSize - 1; ++i)  cout << refNamesPure[ i ] << ',';
    cout << refNamesPure[ refsAdressesSize-1 ] << '\t'
         << tarNamePure << '\t'
////         << invertedRepeat << '\t'
////         << std::fixed << setprecision(4) << alpha << '\t'
////         << (int) contextDepth << '\t'
         << std::fixed << setprecision(5) << averageEntropy << '\t'
         << std::fixed << setprecision(5) << averageEntropy / LOG2_ALPH_SIZE << '\n'
            ;

////    cout.width(2);  cout << std::left << getInvertedRepeat() << "   ";
    mut.unlock();///======================================================
}


/***********************************************************
    decompress target(s) based on reference(s) model
************************************************************/
void FCM::decompress (const string &tarFileName)
{
    /// alpha and ALPH_SIZE*alpha: used in P numerator and denominator
    double alpha[ n_models ], sumAlphas[ n_models ];
    for (U8 i = n_models; i--;)
    {
        alpha[ i ] = (double) 1 / alphaDens[ i ];
        sumAlphas[ i ] = ALPH_SIZE * alpha[ i ];
    }

    ifstream tarFileIn( tarFileName, ios::in ); /// open target file

    mut.lock();///========================================================
    if (!tarFileIn)                             /// error occurred while opening file
    {
        cerr << "The file '" << tarFileName << "' cannot be opened, or it is empty.\n";
        tarFileIn.close();                      /// close file
        return;                                 /// exit this function
    }
    mut.unlock();///======================================================

    U64 maxPlaceValue[ n_models ];
//    for (U8 i = n_models; i--;) maxPlaceValue[ i ] = (U64) pow( ALPH_SIZE, ctxDepths[ i ] );
    for (U8 i = n_models; i--;) maxPlaceValue[ i ] = POWER5[ ctxDepths[i] ];
    U64 tarContext[ n_models ]; fill_n(tarContext, n_models, 0); /// context(s) (integer) sliding through the dataset
    U64 tCtx = 0;                               /// temp variable to decrease accessing tarContext[] array
    string tarLine;                             /// keep each line of the file

    ////////////////////////////////
    U64     nSym;                               /// number of symbols (n_s). in probability numerator
    U64     sumNSym;                            /// sum of number of symbols (sum n_a). in probability denominator
    double  prob_i;                             /// each model probability of a symbol
    double  rawWeight[ n_models ];              /// each model weight before normalization. init: 1/M
    double  weight[ n_models ];     fill_n(weight, n_models, (double) 1 / n_models);    /// each model weight
    double  probability;                        /// final probability of a symbol
    double  sumOfEntropies = 0;                 /// sum of entropies for different symbols
    U64     totalNOfSyms = 0;                   /// number of all symbols in the sequence
    double  averageEntropy = 0;                 /// average entropy (H)
    double  sumOfWeights;                       /// sum of weights. used for normalization
    double  freqsDouble[ ALPH_SIZE ];           /// frequencies of each symbol (double)
    int     freqs[ ALPH_SIZE ];                 /// frequencies of each symbol (integer)
    U64     sumFreqs;                           /// sum of frequencies
    int     sym;                                /// temporary variable to save decode result
    ////////////////////////////////

    /*
    /// using macros make this code slower
    #define X \
         ((compMode == 'h') ? (hashTable[ tarContext ][ currSymInt ]) \
                                   : (table[ tarContext * ALPH_SUM_SIZE + currSymInt ]))
    #define Y(in) do { \
                (compMode == 't') \
                ? in = table[ tarContext * ALPH_SUM_SIZE + ALPH_SIZE ] \
                : in = 0; for (U64 u : hashTable[ tarContext ]) in += u; \
              } while ( 0 )
    */

    FILE *Reader = fopen("COMP.co", "r");       /// to process the compressed file
    FILE *Writer = fopen("DECOMP.de", "w");     /// to save decompressed file
    
    int32_t idxOut = 0;
    char *outBuffer = (char *) calloc(BUFFER_SIZE, sizeof(uint8_t));
    
    startinputtingbits();                       /// start arithmetic decoding process
    start_decode(Reader);
    
    
    
//    P[n].watermark = ReadNBits(32, Reader);
//    if(P[n].watermark != WATERMARK){
//        fprintf(stderr, "Error: Invalid compressed file to decompress!\n");
//        return 1;
//    }
//    checksum[n]    = ReadNBits(46, Reader);
//    P[n].size      = ReadNBits(46, Reader);
//    P[n].gamma     = ReadNBits(32, Reader) / 65536.0;
//    P[n].col       = ReadNBits(32, Reader);
//    P[n].nModels   = ReadNBits(16, Reader);
//    P[n].model     = (ModelPar *) Calloc(P[n].nModels, sizeof(ModelPar));
//    for(k = 0 ; k < P[n].nModels ; ++k){
//        P[n].model[k].ctx   = ReadNBits(16, Reader);
//        P[n].model[k].den   = ReadNBits(16, Reader);
//        P[n].model[k].ir    = ReadNBits( 1, Reader);
//        P[n].model[k].edits = ReadNBits( 8, Reader);
//        P[n].model[k].eDen  = ReadNBits(32, Reader);
//        P[n].model[k].type  = ReadNBits( 1, Reader);
//        if(P[n].model[k].type == 1)
//            ++refNModels;
//    }
    
    
//    P[id].watermark        = ReadNBits(32, Reader);
//    garbage                = ReadNBits(46, Reader);
//    P[id].size             = ReadNBits(46, Reader);
//    P[id].gamma            = ReadNBits(32, Reader) / 65536.0;
//    P[id].col              = ReadNBits(32, Reader);
//    P[id].nModels          = ReadNBits(16, Reader);
//    for(k = 0 ; k < P[id].nModels ; ++k){
//        P[id].model[k].ctx   = ReadNBits(16, Reader);
//        P[id].model[k].den   = ReadNBits(16, Reader);
//        P[id].model[k].ir    = ReadNBits( 1, Reader);
//        P[id].model[k].edits = ReadNBits( 8, Reader);
//        P[id].model[k].eDen  = ReadNBits(32, Reader);
//        P[id].model[k].type  = ReadNBits( 1, Reader);
//    }
    
    switch ( compMode )
    {
        case 't':
        {
            U64 rowIndex;                       /// index of a row in the table
            
            while ( getline(tarFileIn, tarLine) )
            {
                
                //////////////////////////////////
                totalNOfSyms = totalNOfSyms + tarLine.size();   /// number of symbols in each line of dataset
                //////////////////////////////////
                
                /// table includes the number of occurrences of symbols A, C, N, G, T
                for (string::iterator lineIt = tarLine.begin(); lineIt != tarLine.end(); ++lineIt)
                {
                    fill_n( freqsDouble, ALPH_SIZE, 0 );        /// reset array of frequencies
                    
                    U8 currSymInt = symCharToInt(*lineIt);    /// integer version of the current symbol
                    
                    ////////////////////////////////
//                    probability  = 0;
                    sumOfWeights = 0;
                    
                    for (U8 i = n_models; i--;)
                    {
                        rowIndex = (tCtx = tarContext[ i ]) * ALPH_SUM_SIZE;
                        
                        /// frequencies (double)
                        for (U8 j = ALPH_SIZE; j--;) freqsDouble[ j ] += weight[ i ] * tables[ i ][ rowIndex + j ];
                        nSym = tables[ i ][ rowIndex + currSymInt ];    /// number of symbols
//                          nSym = X;
                        sumNSym = tables[ i ][ rowIndex + ALPH_SIZE ];  /// sum of number of symbols
//                          Y(sumNSyms);
                        prob_i = (nSym + alpha[ i ]) / (sumNSym + sumAlphas[ i ]);  /// P(s|c^t)
//                        probability = probability + weight[ i ] * prob_i;           /// P_1*W_1 + P_2*W_2 + ...
                        /// weight before normalization
                        rawWeight[ i ] = fastPow(weight[ i ], gamma) * prob_i;
                        sumOfWeights = sumOfWeights + rawWeight[ i ];   /// sum of weights. used for normalization
    
                        /// update context
                        tarContext[ i ] = (U64) (tCtx * ALPH_SIZE + currSymInt) % maxPlaceValue[ i ];
                    }
                    /// update weights
                    for (U8 i = n_models; i--;) weight[ i ] = rawWeight[ i ] / sumOfWeights;
                    
//                    sumOfEntropies = sumOfEntropies + log2(probability);            /// sum( log_2 P(s|c^t) )
                    /////////////////////////////////
                    
                    /// frequencies (integer)
                    for (U8 j = ALPH_SIZE; j--;) freqs[ j ] = (int) (1 + (freqsDouble[ j ] * DOUBLE_TO_INT));
//                    sumFreqs = 0;   for (int f : freqs) sumFreqs += f;                  /// sum of frequencies
    
                    sym = ArithDecodeSymbol(ALPH_SIZE, freqs, Reader);                  /// Arithmetic decoder
//                    sym = ArithDecodeSymbol(ALPH_SIZE, freqs, (int) sumFreqs, Reader);  /// Arithmetic decoder
                    outBuffer[ idxOut ] = symIntToChar(sym);                            /// output buffer
                    
                    if (++idxOut == BUFFER_SIZE)
                    {
                        fwrite( outBuffer, 1, idxOut, Writer );                         /// write output
                        idxOut = 0;
                    }
                }   /// end for
            }   /// end while
        }   /// end case
            break;
    
        case 'h':
        {
            array< U64, ALPH_SIZE > hTRowArray;                 /// hash table row array -- to save a row of hTable
        
            while ( getline(tarFileIn, tarLine) )
            {
            
                //////////////////////////////////
                totalNOfSyms = totalNOfSyms + tarLine.size();   /// number of symbols in each line of dataset
                //////////////////////////////////
            
                /// hash table includes the number of occurrences of symbols A, C, N, G, T
                for (string::iterator lineIt = tarLine.begin(); lineIt != tarLine.end(); ++lineIt)
                {
                    fill_n( freqsDouble, ALPH_SIZE, 0 );        /// reset array of frequencies
                
                    U8 currSymInt = symCharToInt(*lineIt);    /// integer version of the current symbol
                
                    //////////////////////////////////
//                    probability  = 0;
                    sumOfWeights = 0;
                
                    for (U8 i = n_models; i--;)
                    {
                        tCtx = tarContext[ i ];
                        hTRowArray = hashTables[ i ][ tCtx ];   /// save the row of hash table into an array
                        /// frequencies (double)
                        for (U8 j = ALPH_SIZE; j--;) freqsDouble[ j ] += weight[ i ] * hTRowArray[ j ];
                        /// sum of number of symbols
                        sumNSym = 0;    for (U64 u : hTRowArray) sumNSym = sumNSym + u;
////                        Y(sumNSym);
                        nSym = hTRowArray[ currSymInt ];        /// number of symbols
////                          nSym = X;
////                          X(nSym);
                        prob_i = (nSym + alpha[ i ]) / (sumNSym + sumAlphas[ i ]);  /// P(s|c^t)
//                        probability = probability + weight[ i ] * prob_i;           /// P_1*W_1 + P_2*W_2 + ...
                        /// weight before normalization
                        rawWeight[ i ] = fastPow(weight[ i ], gamma) * prob_i;
                        sumOfWeights = sumOfWeights + rawWeight[ i ];   /// sum of weights. used for normalization
                        
                        /// update context
                        tarContext[ i ] = (U64) (tCtx * ALPH_SIZE + currSymInt) % maxPlaceValue[ i ];
                    }
                    /// update weights
                    for (U8 i = n_models; i--;) weight[ i ] = rawWeight[ i ] / sumOfWeights;
                    
//                    sumOfEntropies = sumOfEntropies + log2(probability);            /// sum( log_2 P(s|c^t) )
                    /////////////////////////////////
                    
                    /// frequencies (integer)
                    for (U8 j = ALPH_SIZE; j--;) freqs[ j ] = (int) (1 + (freqsDouble[ j ] * DOUBLE_TO_INT));
//                    sumFreqs = 0;   for (int f : freqs) sumFreqs += f;                  /// sum of frequencies
    
                    sym = ArithDecodeSymbol(ALPH_SIZE, freqs, Reader);                  /// Arithmetic decoder
//                    sym = ArithDecodeSymbol(ALPH_SIZE, freqs, (int) sumFreqs, Reader);  /// Arithmetic decoder
                    outBuffer[ idxOut ] = symIntToChar(sym);                            /// output buffer
                    
                    if (++idxOut == BUFFER_SIZE)
                    {
                        fwrite( outBuffer, 1, idxOut, Writer );                         /// write output
                        idxOut = 0;
                    }
                }   /// end for
            }   /// end while
        }   /// end case
            
        default: break;
    }   /// end switch
    
    if (idxOut != 0)    fwrite(outBuffer, 1, idxOut, Writer);
    
    finish_decode();
    doneinputtingbits();                                        /// decode last bit
    fclose( Reader );                                           /// close compressed file
    fclose( Writer );                                           /// close decompressed file
    
    tarFileIn.close();                                          /// close target file
}


/***********************************************************
    convert char base to integer (U8): ACNGT -> 01234
************************************************************/
inline U8 FCM::symCharToInt (char charSym) const
{
    switch (charSym)
    {
        case 'A':   return 0;
        case 'C':   return 1;
        case 'T':   return 4;
        case 'G':   return 3;
        case 'N':   return 2;
        default:    cerr << "ERROR: unknown symbol '" << charSym << "'\n";  exit(1);
    }

    /*
    switch (ch)
    {
        case 'C':   return (U8) 3;
        case 'N':   return (U8) 2;
        default:    return (U8) (ch % ALPH_SIZE);
    }
     */
}


/***********************************************************
    convert inteter (U8) base to char: 01234 -> ACNGT
************************************************************/
inline char FCM::symIntToChar (U8 intSym) const
{
    switch(intSym)
    {
        case 0:     return 'A';
        case 1:     return 'C';
        case 4:     return 'T';
        case 3:     return 'G';
        case 2:     return 'N';
        default:    cerr << "ERROR: unknown integer '" << intSym << "'\n";  exit(1);
    }
}


/***********************************************************
    fast power
    by Martin Ankerl
    http://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
************************************************************/
inline double FCM::fastPow (double base, double exponent)
{
    union
    {
        double d;
        int x[2];
    } u = {base};
    
    u.x[ 1 ] = (int) (exponent * (u.x[ 1 ] - 1072632447) + 1072632447);
    u.x[ 0 ] = 0;
    
    return u.d;
}


/***********************************************************
    size of file
************************************************************/
inline U64 FCM::fileSize (const string &fileName)
{
    /// ios::ate seeks to end immediately after opening
    ifstream fileIn( fileName, ios::in | ios::ate );
    return (U64) fileIn.tellg();
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
//        for (string::iterator lineIt = datasetLine.begin(); lineIt != datasetLine.end(); ++lineIt)
//        {
//            /// htable includes an array of U64 numbers
//            char ch = *lineIt;
//            U8 currSymInt = (U8) ((ch == 'A') ? 0 :
//                                            (ch == 'C') ? 1 :
//                                            (ch == 'G') ? 3 :
//                                            (ch == 'T') ? 4 : 2);
//
////            U8 currSymInt = (ch == 'A') ? (U8) 0 :
////                                 (ch == 'C') ? (U8) 1 :
////                                 (ch == 'G') ? (U8) 3 :
////                                 (ch == 'T') ? (U8) 4 : (U8) 2;
////            U8 currSymInt = ch % ALPH_SIZE;
////            U8 currSymInt = (ch == 'C') ? (U8) 3 :
////                                 (ch == 'N') ? (U8) 2 :
////                                 (U8) (ch % ALPH_SIZE);
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
////                iRCtxCurrSymDiv = div(iRCtxCurrSym, ALPH_SIZE);
//
//                /// update inverted repeat context (integer)
////                invRepContext = (U32) iRCtxCurrSymDiv.quot;
//                invRepContext = (U32) iRCtxCurrSym / ALPH_SIZE;
//
//                /// update table considering inverted repeats
////                ++table[ invRepContext*ALPH_SIZE + iRCtxCurrSymDiv.rem ];
////                ++table[ invRepContext * ALPH_SIZE + iRCtxCurrSym % ALPH_SIZE ];
//                ++table[ invRepContext * ALPH_SUM_SIZE + iRCtxCurrSym % ALPH_SIZE ];
//                ++table[ invRepContext * ALPH_SUM_SIZE + ALPH_SIZE ];
//            }
//
//            //////////////////////////////////
//            /// sum(n_a)
////            U64 *pointerToTable = table;   /// pointer to the beginning of table
////            sumNSyms = 0;
////            for (U8 i = 0; i < ALPH_SIZE; ++i)
////                sumNSyms += *(pointerToTable + context*ALPH_SIZE + i);
//            sumNSyms = ++table[ context * ALPH_SUM_SIZE + ALPH_SIZE ];
//
//            /// P(s|c^t)
////            probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPH_SIZE/alphaDen);
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
//        for (string::iterator lineIt = datasetLine.begin(); lineIt != datasetLine.end(); ++lineIt)
//        {
//            /// htable includes an array of U64 numbers
//            char ch = *lineIt;
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
////                                 (U8) (ch % ALPH_SIZE);
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
////                iRCtxCurrSymDiv = div(iRCtxCurrSym, ALPH_SIZE);
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
////            probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPH_SIZE/alphaDen);
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
        for (string::iterator lineIt = datasetLine.begin(); lineIt != datasetLine.end(); ++lineIt)
        {
            /// htable includes an array of U64 numbers
            char c = *lineIt;
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
//
////    for (int i = 0; i < 125; ++i)
////    {
////
////        cout<<tables[0][i];
////    }
//
//
//    htable_t::iterator b=hashTables[0].begin();
//    htable_t::iterator e=hashTables[0].end();
////    htable_t::iterator b=hashTables[1].begin();
////    htable_t::iterator e=hashTables[1].end();
//    for (htable_t::iterator it = b; it != e; ++it)
//    {
//        cout << it->first << "\t";
//        for (U64 i : it->second)    cout << i << "\t";
//        cout << '\n';
//    }
    
//**********************************************************************
    
//    cout
//         << " >>> Context order size:\t" << (U16) contextDepth << '\n'
//         << " >>> Alpha denominator:\t\t" << (U16) alphaDenom << '\n'
//         << " >>> Inverted repeat:\t\t" << (invertedRepeat ? "Considered"
//                                                           : "Not considered")
//         << '\n'
//         << " >>> file address:\t"
//         << "\n\n";
//
//    cout << "\tA\tC\tN\tG\tT"
//         //              << "\tP_A\tP_C\tP_N\tP_G\tP_T"
//         << '\n'
//         << "\t-----------------------------------"
//         //              << "------------------------------------------"
//         << '\n';
//
//    for (htable_t::iterator it = hTable.begin(); it != hTable.end(); ++it)
//    {
//        cout << it->first;
//        cout << "\t";
//        for (U64 i : it->second)    cout << i << "\t";
//        cout << '\n';
//    }
//    cout << '\n';
}


/***********************************************************
    getters and setters
************************************************************/
bool   FCM::getDecompFlag  ()                    const  { return decompFlag;            }
void   FCM::setDecompFlag  (bool dF)                    { FCM::decompFlag = dF;         }
U8     FCM::getN_threads   ()                    const  { return n_threads;             }
void   FCM::setN_threads   (U8 nT)                      { n_threads = nT;               }
void   FCM::setCompMode    (char cM)                    { compMode = cM;                }
void   FCM::setN_models    (U8 n)                       { n_models = n;                 }
U8     FCM::getN_models    ()                    const  { return n_models;              }
void   FCM::setGamma       (double g)                   { gamma = g;                    }
const  vector<bool>        &FCM::getIR()         const  { return invRepeats;            }
const  vector<U8>          &FCM::getCtxDepth ()  const  { return ctxDepths;             }
const  vector<string>      &FCM::getTarAddr  ()  const  { return tarAddr;               }
void   FCM::pushTarAddr    (const string &tFAs)         { tarAddr.push_back(tFAs);      }
const  vector<string>      &FCM::getRefAddr  ()  const  { return refAddr;               }
void   FCM::pushRefAddr    (const string &rFAs)         { refAddr.push_back(rFAs);      }
void   FCM::initTables     ()                           { tables.reserve(n_models);     }
void   FCM::setTable       (U64 *tbl, U8 idx)           { tables[ idx ] = tbl;          }
void   FCM::initHashTables ()                           { hashTables.reserve(n_models); }
void   FCM::setHashTable   (const htable_t &ht, U8 idx) { hashTables[ idx ] = ht;       }
void   FCM::pushParams     (bool iR, U8 ctx, U16 aD)    { invRepeats.push_back(iR);
                                                          ctxDepths.push_back(ctx);
                                                          alphaDens.push_back(aD);      }
