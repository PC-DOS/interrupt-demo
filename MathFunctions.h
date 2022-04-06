/* MathFunctions.h
 * 
 * This header file contains some small math functions. 
 */

#ifndef _MATH_FUNCTIONS_H
#define _MATH_FUNCTIONS_H

static inline long GetMin(long iNum1, long iNum2){return iNum1<iNum2 ? iNum1 : iNum2;}
static inline long GetMax(long iNum1, long iNum2){return iNum1>iNum2 ? iNum1 : iNum2;}
static inline long GetAbs(long iNum){return iNum>=0 ? iNum : -iNum;}

#endif