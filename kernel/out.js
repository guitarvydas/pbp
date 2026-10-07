let  digits = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"];/* line 7 *//* line 8 *//* line 9 */
function subscripted_digit (n) {                       /* line 10 *//* line 11 */
    if (((( n >=  0) && ( n <=  29)))) {               /* line 12 */
      return  digits [ n];                             /* line 13 */
    }
    else {                                             /* line 14 */
      return  ( "₊".toString ()+ `${ n}`.toString ())  /* line 15 */;/* line 16 */
    }                                                  /* line 17 *//* line 18 */
}

let  counter =  0;                                     /* line 19 *//* line 20 */
function gensymbol (s) {                               /* line 21 *//* line 22 */
    let name_with_id =  ( s.toString ()+ subscripted_digit ( counter).toString ()) /* line 23 */;
    counter =  counter+ 1;                             /* line 24 */
    return  name_with_id;                              /* line 25 *//* line 26 *//* line 27 */
}
