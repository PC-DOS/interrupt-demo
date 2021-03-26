/* MathFunctions.c
 *
 * Some small math functions.
 */
#include "MathFunctions.h"
 
inline long GetMin(long iNum1, long iNum2){
	return iNum1<iNum2 ? iNum1 : iNum2;
}

inline long GetMax(long iNum1, long iNum2){
	return iNum1>iNum2 ? iNum1 : iNum2;
}

inline long GetAbs(long iNum){
	return iNum>=0 ? iNum : -iNum;
}