/*------------------------------------------------------------------------------

Copyright 2010 IEETA / University of Aveiro, All Rights Reserved.
 
These programs are supplied free of charge for research purposes only,
and may not be sold or incorporated into any commercial product. There is
ABSOLUTELY NO WARRANTY of any sort, nor any undertaking that they are
fit for ANY PURPOSE WHATSOEVER. Use them at your own risk. If you do
happen to find a bug, or have modifications to suggest, please report
the same to Armando J. Pinho, ap@ua.pt. The copyright notice above
and this statement of conditions must remain an integral part of each
and every copy made of these files.

------------------------------------------------------------------------------*/

#ifndef ARITH_AUX_H_INCLUDED
#define ARITH_AUX_H_INCLUDED

//#include "defs.h"
#include "def.h"


void GetInterval(U64 *low, U64 *high, U64 *count, U64 symbol);
U8 GetSymbol(U64 *low, U64 *high, U64 *count, U64 target, U64 nSymbols);
void WriteNBits(U64 bits, U64 nBits, FILE *oFp);
U64 ReadNBits(U64 nBits, FILE *iFp);
void AESym(U64 symbol, U64 *counters, U64 totalCount, FILE *oFp);
U8 ArithDecodeSymbol(U64 nSymbols, U64 *counters, U64 totalCount, FILE *iFp);

#endif /* ARITH_AUX_H_INCLUDED */

