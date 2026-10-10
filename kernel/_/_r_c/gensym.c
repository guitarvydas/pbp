#include "pbp.h"

Array_of_Str = [ counted("₀"), counted("₁"), counted("₂"), counted("₃"), counted("₄"), counted("₅"), counted("₆"), counted("₇"), counted("₈"), counted("₉"), counted("₁₀"), counted("₁₁"), counted("₁₂"), counted("₁₃"), counted("₁₄"), counted("₁₅"), counted("₁₆"), counted("₁₇"), counted("₁₈"), counted("₁₉"), counted("₂₀"), counted("₂₁"), counted("₂₂"), counted("₂₃"), counted("₂₄"), counted("₂₅"), counted("₂₆"), counted("₂₇"), counted("₂₈"), counted("₂₉")]/* line 7 */;/* line 8 *//* line 9 */
Str* subscripted_digit (Int n) {
                                                       /* line 10 */
    static digits                                      /* line 11 */
    if ( n >=  0 and  n <=  29):                       /* line 12 */
        return ( (*digits) [ (*n)])                    /* line 13 */
    else:                                              /* line 14 */
        return ( str( counted("₊")) + str ( n)         /* line 15 */)/* line 16 *//* line 17 *//* line 18 */}

Int =  0                                               /* line 19 */;/* line 20 */
Str* gensymbol (Str* s) {
                                                       /* line 21 */
    static counter                                     /* line 22 */
    Str* name_with_id =  str( s) + subscripted_digit ( counter) /* line 23 */
    counter =  counter+ 1;                             /* line 24 */
    return ( name_with_id)                             /* line 25 */;/* line 26 */}
