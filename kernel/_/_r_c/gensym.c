#include "pbp.h"

Array_of_Str = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"]/* line 7 */;/* line 8 *//* line 9 */
Str* subscripted_digit (Int n) {
                                                       /* line 10 */
    static digits                                      /* line 11 */
    if ( n >=  0 and  n <=  29):                       /* line 12 */
        return ( (*digits) [ (*n)])                    /* line 13 */
    else:                                              /* line 14 */
        return ( str( "₊") + str ( n)                  /* line 15 */)/* line 16 *//* line 17 *//* line 18 */}

Int =  0                                               /* line 19 */;/* line 20 */
Str* gensymbol (Str* s) {
                                                       /* line 21 */
    static counter                                     /* line 22 */
    Str* name_with_id =  str( s) + subscripted_digit ( counter) /* line 23 */
    counter =  counter+ 1                              /* line 24 */
    return ( name_with_id)                             /* line 25 */;/* line 26 */}
