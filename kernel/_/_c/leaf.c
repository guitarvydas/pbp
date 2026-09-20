/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
void make_leaf (name,owner,instance_data,arg,handler,reset_handler) {
                                                       /* line 4 */
    eh =  Eh ()                                        /* line 5 */
    nm =  ""                                           /* line 6 */
    if  NULL!= owner:                                  /* line 7 */
        nm =  owner.name;                              /* line 8 *//* line 9 */
    eh.name =  str( nm) +  str( "▹") +  name           /* line 10 */
    eh.owner =  owner                                  /* line 11 */
    eh.handler =  handler                              /* line 12 */
    eh.reset_handler =  reset_handler                  /* line 13 */
    eh.finject =  injector                             /* line 14 */
    eh.stop =  leaf_reset                              /* line 15 */
    eh.instance_data =  instance_data                  /* line 16 */
    eh.arg =  arg                                      /* line 17 */
    eh.state =  "idle"                                 /* line 18 */
    return ( eh)                                       /* line 19 */;;;;;;;;;/* line 20 *//* line 21 */}

/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 22 */
void leaf_reset (part) {
                                                       /* line 23 */
    external
    part.inq.clear ()                                  /* line 24 */
    external
    part.outq.clear ()                                 /* line 25 */
    if ( part.reset_handler!= NULL):                   /* line 26 */
        part.reset_handler ( part)                     /* line 27 *//* line 28 */
    part.state =  "idle";                              /* line 29 *//* line 30 */}
