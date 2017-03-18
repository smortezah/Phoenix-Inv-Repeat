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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "arith.h"
#include "bitio.h"
#include "arith_aux.h"


/*----------------------------------------------------------------------------*/

void GetInterval (U64 *low, U64 *high, U64 *count, U64 symbol)
{
    U64 n;
    
    *low = 0;
    for (n = 0; n < symbol; n++)
        *low += count[ n ];
    
    *high = *low + count[ symbol ];
}

/*----------------------------------------------------------------------------*/

U8 GetSymbol (U64 *low, U64 *high, U64 *count, U64 target, U64 nSymbols)
{
    U8 n;
    
    *low = 0;
    for (n = 0; n < nSymbols; n++)
    {
        if (*low + count[ n ] > target)
            break;
        
        *low += count[ n ];
    }
    
    *high = *low + count[ n ];
    return (n);
}

/*----------------------------------------------------------------------------*/

void WriteNBits (U64 bits, U64 nBits, FILE *oFp)
{
    while (nBits--)
    {
        if ((bits >> nBits) & 0x1)
            arithmetic_encode(1, 2, 2, oFp);
        
        else
            arithmetic_encode(0, 1, 2, oFp);
        
    }
    
}

/*----------------------------------------------------------------------------*/

U64 ReadNBits (U64 nBits, FILE *iFp)
{
    U64 bits = 0;
    U64 target, low, high, count[2] = {1, 1};
    
    while (nBits--)
    {
        bits <<= 1;
        target = arithmetic_decode_target(2);
        bits |= GetSymbol(&low, &high, count, target, 2);
        arithmetic_decode(low, high, 2, iFp);
    }
    
    return bits;
}

/*----------------------------------------------------------------------------*/

void AESym (U64 symbol, U64 *counters, U64 totalCount, FILE *oFp)
{
    U64 low, high;
    
    GetInterval(&low, &high, counters, symbol);
    arithmetic_encode(low, high, totalCount, oFp);
}

/*----------------------------------------------------------------------------*/

U8 ArithDecodeSymbol (U64 nSymbols, U64 *counters, U64 totalCount, FILE *iFp)
{
    U64 low, high;
    U8 symbol = GetSymbol(&low, &high, counters,
                           arithmetic_decode_target(totalCount), nSymbols);
    
    arithmetic_decode(low, high, totalCount, iFp);
    return symbol;
}

