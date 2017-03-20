/*------------------------------------------------------------------------------

Arithmetic Encoder and Decoder
 
Armando J. Pinho, Morteza Hosseini
IEETA, University of Aveiro
Copyright 2010,2017, All Rights Reserved.
 
These programs are supplied free of charge for research purposes only,
and may not be sold or incorporated into any commercial product. There is
ABSOLUTELY NO WARRANTY of any sort, nor any undertaking that they are
fit for ANY PURPOSE WHATSOEVER. Use them at your own risk. If you do
happen to find a bug, or have modifications to suggest, please report
the same to Armando J. Pinho, ap@ua.pt. The copyright notice above
and this statement of conditions must remain an integral part of each
and every copy made of these files.

------------------------------------------------------------------------------*/

#ifndef PHOENIX_ARITHENCDEC_H
#define PHOENIX_ARITHENCDEC_H


#include "def.h"


class ArithEncDec
{
public:
    ArithEncDec ();                                 /// constructor
            
    void GetInterval (int*, int*, int*, U8);
    U8   GetSymbol   (int*, int*, int*, int, U8);
    void WriteNBits  (U64,  int,  FILE*);           /// write N bits to a file
    int  ReadNBits   (U32,  FILE*);                 /// read N bits from a file
    void AESym       (U8,   int*, int,  FILE*);     /// arithmetic encoding
    U8   ADSym       (U8,   int*, int,  FILE*);     /// arithmetic decoding
};


#endif //PHOENIX_ARITHENCDEC_H
