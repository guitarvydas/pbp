/* line 1 */
class Datum {
  constructor () {                                     /* line 2 */

    this.v =  null;                                    /* line 3 */
    this.clone =  null;                                /* line 4 */
    this.reclaim =  null;                              /* line 5 */
    this.other =  null;/*  reserved for use on per-project basis  *//* line 6 *//* line 7 */
  }
}
                                                       /* line 8 *//* line 9 */
/*  Mevent passed to a leaf component. */              /* line 10 */
/*  */                                                 /* line 11 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 12 */
/*  `payload` is the data attached to this mevent. */  /* line 13 */
class Mevent {
  constructor () {                                     /* line 14 */

    this.port =  null;                                 /* line 15 */
    this.payload =  null;                              /* line 16 *//* line 17 */
  }
}
                                                       /* line 18 */
function clone_port (s) {                              /* line 19 */
    return clone_string ( s)                           /* line 20 */;/* line 21 *//* line 22 */
}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 23 */
/*  entering the very top of a network. */             /* line 24 */
function make_mevent (port,datum) {                    /* line 25 */
    let p = clone_string ( port)                       /* line 26 */;
    let  m =  new Mevent ();                           /* line 27 */;
    m.port =  p;                                       /* line 28 */
    m.payload =  datum.clone ();                       /* line 29 */
    return  m;                                         /* line 30 *//* line 31 *//* line 32 */
}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 33 */
function mevent_clone (mev) {                          /* line 34 */
    let  m =  new Mevent ();                           /* line 35 */;
    m.port = clone_port ( mev.port)                    /* line 36 */;
    m.payload =  mev.payload.clone ();                 /* line 37 */
    return  m;                                         /* line 38 *//* line 39 *//* line 40 */
}

/*  Frees a mevent. */                                 /* line 41 */
function destroy_mevent (mev) {                        /* line 42 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 43 *//* line 44 *//* line 45 *//* line 46 */
}

function destroy_datum (mev) {                         /* line 47 *//* line 48 *//* line 49 *//* line 50 */
}

function destroy_port (mev) {                          /* line 51 *//* line 52 *//* line 53 *//* line 54 */
}

/*  */                                                 /* line 55 */
function format_mevent (m) {                           /* line 56 */
    if ( m ==  null) {                                 /* line 57 */
      return  "{}";                                    /* line 58 */
    }
    else {                                             /* line 59 */
      return  ( "{%5C”".toString ()+  ( m.port.toString ()+  ( "%5C”:%5C”".toString ()+  ( m.payload.v.toString ()+  "%5C”}".toString ()) .toString ()) .toString ()) .toString ()) /* line 60 */;/* line 61 */
    }                                                  /* line 62 */
}

function format_mevent_raw (m) {                       /* line 63 */
    if ( m ==  null) {                                 /* line 64 */
      return  "";                                      /* line 65 */
    }
    else {                                             /* line 66 */
      return  m.payload.v;                             /* line 67 *//* line 68 */
    }                                                  /* line 69 */
}
