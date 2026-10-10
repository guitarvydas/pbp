/* line 1 */
class Datum {
  constructor () {                                     /* line 2 */

    this.v =  null;                                    /* line 3 */
    this.clone =  null;                                /* line 4 */
    this.reclaim =  null;                              /* line 5 *//* line 6 */
  }
}
                                                       /* line 7 *//* line 8 */
/*  Mevent passed to a leaf component. */              /* line 9 */
/*  */                                                 /* line 10 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 11 */
/*  `payload` is the data attached to this mevent. */  /* line 12 */
class Mevent {
  constructor () {                                     /* line 13 */

    this.port =  null;                                 /* line 14 */
    this.payload =  null;                              /* line 15 *//* line 16 */
  }
}
                                                       /* line 17 */
function clone_port (s) {                              /* line 18 */
    return clone_string ( s)                           /* line 19 */;/* line 20 *//* line 21 */
}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 22 */
/*  entering the very top of a network. */             /* line 23 */
function make_mevent (port,datum) {                    /* line 24 */
    let p = clone_string ( port)                       /* line 25 */;
    let  m =  new Mevent ();                           /* line 26 */;
    m.port =  p;                                       /* line 27 */
    m.payload =  datum.clone ();                       /* line 28 */
    return  m;                                         /* line 29 *//* line 30 *//* line 31 */
}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 32 */
function mevent_clone (mev) {                          /* line 33 */
    let  m =  new Mevent ();                           /* line 34 */;
    m.port = clone_port ( mev.port)                    /* line 35 */;
    m.payload =  mev.payload.clone ();                 /* line 36 */
    return  m;                                         /* line 37 *//* line 38 *//* line 39 */
}

/*  Frees a mevent. */                                 /* line 40 */
function destroy_mevent (mev) {                        /* line 41 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 42 *//* line 43 *//* line 44 *//* line 45 */
}

function destroy_datum (mev) {                         /* line 46 *//* line 47 *//* line 48 *//* line 49 */
}

function destroy_port (mev) {                          /* line 50 *//* line 51 *//* line 52 *//* line 53 */
}

/*  */                                                 /* line 54 */
function format_mevent (m) {                           /* line 55 */
    if ( m ==  null) {                                 /* line 56 */
      return  "{}";                                    /* line 57 */
    }
    else {                                             /* line 58 */
      return  ( "{%5C”".toString ()+  ( m.port.toString ()+  ( "%5C”:%5C”".toString ()+  ( m.payload.v.toString ()+  "%5C”}".toString ()) .toString ()) .toString ()) .toString ()) /* line 59 */;/* line 60 */
    }                                                  /* line 61 */
}

function format_mevent_raw (m) {                       /* line 62 */
    if ( m ==  null) {                                 /* line 63 */
      return  "";                                      /* line 64 */
    }
    else {                                             /* line 65 */
      return  m.payload.v;                             /* line 66 *//* line 67 */
    }                                                  /* line 68 */
}
