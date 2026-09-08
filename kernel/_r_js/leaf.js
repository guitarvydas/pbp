/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
function make_leaf (name,owner,instance_data,arg,handler,reset_handler) {/* line 4 */
    let  eh =  new Eh ();                              /* line 5 */;
    let  nm =  "";                                     /* line 6 */
    if ( null!= owner) {                               /* line 7 */
      nm =  owner.name;                                /* line 8 *//* line 9 */
    }
    eh.name =  ( nm.toString ()+  ( "▹".toString ()+  name.toString ()) .toString ()) /* line 10 */;
    eh.owner =  owner;                                 /* line 11 */
    eh.handler =  handler;                             /* line 12 */
    eh.reset_handler =  reset_handler;                 /* line 13 */
    eh.finject =  injector;                            /* line 14 */
    eh.stop =  leaf_reset;                             /* line 15 */
    eh.instance_data =  instance_data;                 /* line 16 */
    eh.arg =  arg;                                     /* line 17 */
    eh.state =  "idle";                                /* line 18 */
    eh.kind =  "leaf";                                 /* line 19 */
    return  eh;                                        /* line 20 *//* line 21 *//* line 22 */
}

/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 23 */
function leaf_reset (part) {                           /* line 24 */

    part.inq = [];                                     /* line 25 */

    part.outq = [];                                    /* line 26 */
    if (( part.reset_handler!= null)) {                /* line 27 */
      part.reset_handler ( part)                       /* line 28 *//* line 29 */
    }
    part.state =  "idle";                              /* line 30 *//* line 31 */
}
