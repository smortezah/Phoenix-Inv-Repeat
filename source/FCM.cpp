#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>      // setw, setprecision
#include <cstring>      // memset, memcpy

#include "FCM.h"
#include "functions.h"

using std::cout;
using std::cerr;
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
    build table or hash table
************************************************************/
void FCM::buildTableOrHashTable ()
{
    const uint8_t contextDepth = getContextDepth();    // get context depth
    const uint16_t alphaDen = getAlphaDenom();      // get alpha denominator
    const bool isInvertedRepeat = getInvertedRepeat();  // get inverted repeat
    // TODO: supprt for both target and reference file addresses
    string fileName = getTarFileAddress();  // get target file address
    
    // 't'=table, 'h'=hash table
//    char mode = (contextDepth > TABLE_MAX_CONTEXT) ? 'h' : 't';
    char mode = 't';
//    char mode = 'h';


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
    
    
    ifstream fileIn(fileName, ios::in);         // open file located in fileName
    
    if (!fileIn)                                // error occurred while opening file
    {
        cerr << "The file '" << fileName << "' cannot be opened, or it is empty.\n";
        fileIn.close();                         // close file
        return;                                 // exit this function
    }
    
    // context, that slides in the dataset
    string context(contextDepth, '0');
    uint32_t contextInt = 0;
    
    htable_t hTable;                            // create hash table
    
    size_t tableNumOfRows = (size_t) pow(5, contextDepth);
////    std::array< array< uint64_t, ALPHABET_SIZE+1 >, tableNumOfRows > table;
    uint64_t *table = new uint64_t[tableNumOfRows * ALPHABET_SIZE];
    
    // initialize table or hash table with 0'z
    if (mode != 't')    hTable.insert({context, {0, 0, 0, 0, 0}});
    else                memset(table, 0, sizeof(table[ 0 ]) * tableNumOfRows * ALPHABET_SIZE);
    
    ////////////////////////////////
    uint64_t nSym;                      // number of symbols (n_s). To calculate probability
    uint64_t sumNSyms;                  // sum of number of symbols (sum n_a). To calculate probability
    double   probability = 0;           // probability of a symbol, based on an identified context
    double   sumOfEntropies = 0;        // sum of entropies for different symbols
    uint64_t totalNumberOfSymbols = 0;  // number of all symbols in the sequence
    double   averageEntropy = 0;        // average entropy (H)
    //////////////////////////////////
    
    string datasetLine;                 // keep each line of the file
    
    while (getline(fileIn, datasetLine))
    {
        
        //////////////////////////////////
        totalNumberOfSymbols += datasetLine.size();    // number of symbols in each line of dataset
        //////////////////////////////////

        // fill hash table by number of occurrences of symbols A, C, N, G, T
        for (string::iterator lineIter = datasetLine.begin(); lineIter != datasetLine.end(); ++lineIter)
        {
            // htable includes an array of uint64_t numbers
            const char c = *lineIter;
            const uint8_t currSymInt = (c == 'A') ? (uint8_t) 0 :
                                       (c == 'C') ? (uint8_t) 1 :
                                       (c == 'G') ? (uint8_t) 3 :
                                       (c == 'T') ? (uint8_t) 4 : (uint8_t) 2;
//            const uint8_t currSymInt = c % 5;
            
            // update table or hash table
            nSym = (mode != 't') ? hTable[ context ][ currSymInt ]++
                                 : table[ contextInt*ALPHABET_SIZE + currSymInt ]++;


////            uint64_t m = 0;
////            for (int i = 0; i != contextDepth; ++i)
////                m += (context[ i ] - 48) * pow(5, contextDepth - i - 1);
////            cout << m;
////            cout << '\n';

            
            // considering inverted repeats to update hash table
            if (isInvertedRepeat)
            {
                // save inverted repeat context
                string invRepeatContext = to_string(4 - currSymInt);
                // convert a number from char into integer format. '0'->0. '4'->4 by
                // 4 - (context[ i ] - 48) = 52 - context[ i ]. 48 is ASCII code of '0'
                for (string::iterator it = context.end() - 1; it != context.begin(); --it)
                    invRepeatContext += to_string(52 - *it);
                // update hash table considering inverted repeats
                (mode != 't') ? ++hTable[ invRepeatContext ][ 52 - context[ 0 ]]
                              : ++hTable[ invRepeatContext ][ 52 - context[ 0 ]];
                
                cout<<invRepeatContext;
            }

            //////////////////////////////////
            // sum(n_a)
            uint64_t *pointerToTable = table;
            sumNSyms = 0;
            if (mode != 't')
                for (uint64_t u : hTable[ context ])    sumNSyms += u;
            else
                for (uint8_t i = 0; i < ALPHABET_SIZE; ++i)
                    sumNSyms += *(pointerToTable + contextInt*ALPHABET_SIZE + i);

            // P(s|c^t)
//            probability = (nSym + (double) 1/alphaDen) / (sumNSyms + (double) ALPHABET_SIZE/alphaDen);
            probability = (double) (alphaDen*nSym + 1) / (alphaDen*sumNSyms + ALPHABET_SIZE);

            // sum( log_2 P(s|c^t) )
            sumOfEntropies += log2(probability);
            /////////////////////////////////
            
            // update context
            if (mode != 't')
                context = context.substr(1, (unsigned) contextDepth - 1) + to_string(currSymInt);
            else
                contextInt = (uint32_t) (contextInt*5 + currSymInt) % tableNumOfRows;
            
            
////            *context.end() = currSymInt;

//////            memcpy(context, context + 1, contextDepth - 1);
//////            context[ contextDepth-1 ] = currSymInt;
//////              *(context+contextDepth-1) = currSymInt;
        }   // end of for
    }   // end of while
    
    fileIn.close();             // close file
    
    if (mode != 't')    FCM::setHashTable(hTable);  // save the built hash table
    
    
//    for (int j = 0; j < tableNumOfRows; ++j)
//    {
//        for (int i = 0; i < 5; ++i)
//            cout << table[ j * ALPHABET_SIZE + i ] << ' ';
//        cout << '\n';
//    }
    
    
    ////////////////////////////////
    // H_N = -1/N sum( log_2 P(s|c^t) )
    averageEntropy = (-1) * sumOfEntropies / totalNumberOfSymbols;

    cout
//            << sumOfEntropies << '\n'
//            << totalNumberOfSymbols << '\n'
            << "  "
            << getInvertedRepeat() << '\t'
            << (float) 1/alphaDen << '\t'
            << (int) contextDepth << '\t'
            << averageEntropy
//            << '\t'
//            << hTable.size()
//            << '\n'
            ;
    ////////////////////////////////
    
}
    

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
         // TODO: this line must be changed
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
char FCM::getTargetOrReference () const         { return targetOrReference;      }
void FCM::setTargetOrReference (char tOrR)      { FCM::targetOrReference = tOrR; }
uint8_t FCM::getContextDepth () const           { return contextDepth;           }
void FCM::setContextDepth (uint8_t ctxDp)       { FCM::contextDepth = ctxDp;     }
uint16_t FCM::getAlphaDenom () const            { return alphaDenom;             }
void FCM::setAlphaDenom (uint16_t alphaDen)     { FCM::alphaDenom = alphaDen;    }
bool FCM::getInvertedRepeat () const            { return invertedRepeat;         }
void FCM::setInvertedRepeat (bool invRep)       { FCM::invertedRepeat = invRep;  }
const htable_t &FCM::getHashTable () const      { return hashTable;              }
void FCM::setHashTable (const htable_t &hT)     { FCM::hashTable = hT;           }
const string &FCM::getTarFileAddress () const   { return tarFileAddress;         }
void FCM::setTarFileAddress (const string &tFA) { FCM::tarFileAddress = tFA;     }
const string &FCM::getRefFileAddress () const   { return refFileAddress;         }
void FCM::setRefFileAddress (const string &rFA) { FCM::refFileAddress = rFA;     }
