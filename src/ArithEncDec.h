/******************************************************************************
Authors:   John Carpinelli   (johnfc@ecr.mu.oz.au)
           Wayne Salamonsen  (wbs@mundil.cs.mu.oz.au)
           Lang Stuiver      (langs@cs.mu.oz.au)
           Radford Neal      (radford@ai.toronto.edu)

Purpose:   Data compression using a revised arithmetic coding method.

Based on:  A. Moffat, R. Neal, I.H. Witten, "Arithmetic Coding Revisted",
           Proc. IEEE Data Compression Conference, Snowbird, Utah, March 1995.

           Low-Precision Arithmetic Coding Implementation by Radford M. Neal

Copyright 1995 John Carpinelli and Wayne Salamonsen, All Rights Reserved.
Copyright 1996 Lang Stuiver, All Rights Reserved.

These programs are supplied free of charge for research purposes only,
and may not sold or incorporated into any commercial product.  There is
ABSOLUTELY NO WARRANTY of any sort, nor any undertaking that they are
fit for ANY PURPOSE WHATSOEVER.  Use them at your own risk.  If you do
happen to find a bug, or have modifications to suggest, please report
the same to Alistair Moffat, alistair@cs.mu.oz.au.  The copyright
notice above and this statement of conditions must remain an integral
part of each and every copy made of these files.

  $Log: arith.c,v $
  Revision 1.2  2008-01-23 15:59:09  ap
  No real changes...

  Revision 1.1  2005/07/01 09:22:27  ap

  New files.

  Revision 1.1  1996/08/07 01:34:11  langs
  Initial revision

******************************************************************************

Code adapted by Armando J. Pinho and Morteza Hosseini
ap@ua.pt, seyedmorteza@ua.pt
University of Aveiro, DETI/IEETA, 3810-193 Aveiro, Portugal
December 1999, March 2017

******************************************************************************/


#ifndef PHOENIX_ARITHENCDEC_H
#define PHOENIX_ARITHENCDEC_H


#include <stdio.h>
#include <stdlib.h>
#include "def.h"
//#include "arith.h"
#include "bitio.h"


/* ================= USER ADJUSTABLE PARAMETERS =================== */

/* Default B_bits and F_bits */
#ifndef B_BITS
#define		B_BITS		32
#endif

#ifndef F_BITS
#define		F_BITS		27
#endif

/* Change these types for different precision calculations.  They may affect
 * the speed of the arithmetic operations (multiplcation, division, shift,
 * etc).
 * The way the stats module is implemented, the type of freq_value
 * must be able to accomodate f_bits+1 bits, instead of f_bits, to avoid
 * overflows.  Ie: For an f_bits of up to 31, type freq_value must be 32 bits.
 */
typedef unsigned long   code_value;	/* B_BITS of precision */
typedef unsigned long	freq_value;	/* F_BITS+1 of precision */
typedef unsigned long	div_value;	/* B_BITS-F_BITS of precision */
//typedef U64	code_value;	/* B_BITS of precision */
//typedef U64	freq_value;	/* F_BITS+1 of precision */
//typedef U64	div_value;	/* B_BITS-F_BITS of precision */


/* MAX_BITS_OUTSTANDING is a bound on bits_outstanding
 * If bits_outstanding ever gets above this number (extremely unlikely)
 * the program will abort with an error message.  (See arith.c for details).
 */
#define 	MAX_BITS_OUTSTANDING	((unsigned long)1<<31)
//#define 	MAX_BITS_OUTSTANDING	((U64)1<<63)

/* ================= END USER ADJUSTABLE PARAMETERS =================== */

/* Determine maximum bits allowed, based on size of the types used
 * to store them.  Also, that MAX_F_BITS <= MAX_B_BITS-2
 */

#define		MAX_B_BITS   (int) (sizeof(code_value) * 8)
//#define		MAX_B_BITS   (U64)( sizeof(code_value) * 8)
#define		MAX_F_BITS   (int)((sizeof(freq_value)*8)-1 < MAX_B_BITS - 2\
				?  (sizeof(freq_value)*8)-1 : MAX_B_BITS - 2)

/* If varying bits, variables are B_bits, F_bits, Half and Quarter,
 *	otherwise #define them
 * These variables will be read (and possibly changed) by main.c and
 *  stats.c
 */

#define		B_bits		B_BITS
#define		F_bits  	F_BITS

extern char *coder_desc;

///-------------------------------------------------------------------------------------

#define		Half		((code_value) 1 << (B_bits-1))
#define		Quarter		((code_value) 1 << (B_bits-2))

/* Separate input and output */
/* Input decoding state */
static code_value	in_R;				/* code range */
static code_value	in_D;				/* = V-L (V offset)*/
static div_value	in_r;				/* normalized range */

/* Output encoding state */
static code_value	out_L;				/* lower bound */
static code_value	out_R;				/* code range */
static unsigned long	out_bits_outstanding;		/* follow bit count */


/*
 * BIT_PLUS_FOLLOW(b, s)
 * responsible for outputting the bit passed to it and an opposite number of
 * bit equal to the value stored in bits_outstanding
 *
 */
#define ORIG_BIT_PLUS_FOLLOW(b, s)	\
do                                      \
{ 	  			        \
    OUTPUT_BIT((b), s);           	\
    while (out_bits_outstanding > 0)	\
    { 					\
	OUTPUT_BIT(!(b), s);      	\
	out_bits_outstanding--;    	\
    } 	                		\
} while (0)


#  define BIT_PLUS_FOLLOW(x, s)	ORIG_BIT_PLUS_FOLLOW(x, s)

/*
 * ENCODE_RENORMALISE
 * output code bits until the range has been expanded
 * to above QUARTER
 * With FRUGAL_BITS option, ignore first zero bit output
 * (a redundant zero will otherwise be emitted every time the encoder is
 * started)
 */
#define ENCODE_RENORMALISE(s)		\
do {					\
    while (out_R <= Quarter)		\
    {					\
        if (out_L >= Half)		\
    	{				\
    	    BIT_PLUS_FOLLOW(1, s);	\
    	    out_L -= Half;		\
    	}				\
    	else if (out_L+out_R <= Half)	\
    	{				\
    	    BIT_PLUS_FOLLOW(0, s);	\
    	}				\
    	else 				\
    	{				\
    	    out_bits_outstanding++;	\
    	    out_L -= Quarter;		\
    	}				\
    	out_L <<= 1;			\
    	out_R <<= 1;			\
    }					\
} while (0)


/*
 * DECODE_RENORMALISE
 * input code bits until range has been expanded to
 * more than QUARTER. Mimics encoder.
 * FRUGAL_BITS option also keeps track of bitstream input so it can work out
 * exactly how many disambiguating bits the encoder put out (1,2 or 3).
 */
#define DECODE_RENORMALISE(s)			\
do {						\
    while (in_R <= Quarter)			\
    {						\
    	in_R <<= 1;				\
    	ADD_NEXT_INPUT_BIT(in_D, 0, s);		\
    }						\
} while (0)


///-------------------------------------------------------------------------------------

class ArithEncDec
{
public:
    ArithEncDec ();                                 /// constructor
    
    void GetInterval       (int*, int*, int*, U8);
    U8   GetSymbol         (int*, int*, int*, int, U8);
    void WriteNBits        (U64,  int,  FILE*);           /// write N bits to a file
    int  ReadNBits         (U32,  FILE*);                 /// read N bits from a file
    void AESym             (U8,   int*, int,  FILE*);     /// arithmetic encoding
    U8   ADSym             (U8,   int*, int,  FILE*);     /// arithmetic decoding
    void start_encode      (void);
    void finish_encode     (FILE*);
    void start_decode      (FILE*);
    void finish_decode     (void);
    void arithmetic_encode (freq_value, freq_value, freq_value, FILE*);
    void arithmetic_decode (freq_value, freq_value, freq_value, FILE*);
    freq_value arithmetic_decode_target (freq_value);
//    void binary_arithmetic_encode(freq_value, freq_value, int, FILE*);
//    int  binary_arithmetic_decode(freq_value, freq_value, FILE*);
};


#endif //PHOENIX_ARITHENCDEC_H
