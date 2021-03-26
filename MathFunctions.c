/* MathFunctions.c
 *
 * Some small math functions.
 */
 
inline long GetMin(long iNum1, long iNum2){
	return iNum1<iNum2 ? iNum1 : iNum2;
}
EXPORT_SYMBOL(GetMin);

inline long GetMax(long iNum1, long iNum2){
	return iNum1>iNum2 ? iNum1 : iNum2;
}
EXPORT_SYMBOL(GetMax);

inline long GetAbs(long iNum){
	return iNum>=0 ? iNum : -iNum;
}
EXPORT_SYMBOL(GetAbs);