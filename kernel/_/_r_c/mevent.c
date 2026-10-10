#include "pbp.h"
                                                       /* line 1 */
Datum* fresh_Datum () {
    Datum *self;
    self = (Datum*)malloc(sizeof(Datum*));
    self->v =  NULL;                                   /* line 3 */
    self->clone =  NULL;                               /* line 4 */
    self->reclaim =  NULL;                             /* line 5 *//* line 6 */
    return self;
}
                                                       /* line 7 *//* line 8 */
/*  Mevent passed to a leaf component. */              /* line 9 */
/*  */                                                 /* line 10 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 11 */
/*  `payload` is the data attached to this mevent. */  /* line 12 */
Mevent* fresh_Mevent () {
    Mevent *self;
    self = (Mevent*)malloc(sizeof(Mevent*));
    self->port =  NULL;                                /* line 14 */
    self->payload =  NULL;                             /* line 15 *//* line 16 */
    return self;
}
                                                       /* line 17 */
Port clone_port (Port s) {
                                                       /* line 18 */
    return (clone_string ( s)                          /* line 19 */)/* line 20 *//* line 21 */}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 22 */
/*  entering the very top of a network. */             /* line 23 */
Mevent* make_mevent (Port port,Datum* datum) {
                                                       /* line 24 */
    Port p = clone_string ( port)                      /* line 25 */
    Mevent*  m =  fresh_Mevent ()                      /* line 26 */;
    (*m).port =  p;                                    /* line 27 */
    (*m).payload =   (*datum).clone ();                /* line 28 */
    return ( m)                                        /* line 29 */;;/* line 30 *//* line 31 */}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 32 */
Mevent* mevent_clone (Mevent* mev) {
                                                       /* line 33 */
    Mevent*  m =  fresh_Mevent ()                      /* line 34 */;
    (*m).port = clone_port (  (*mev).port)             /* line 35 */;
    (*m).payload =    (*mev).payload.clone ();         /* line 36 */
    return ( m)                                        /* line 37 */;;/* line 38 *//* line 39 */}

/*  Frees a mevent. */                                 /* line 40 */
void destroy_mevent (Mevent* mev) {
                                                       /* line 41 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 42 */
                                                       /* line 43 *//* line 44 *//* line 45 */}

void destroy_datum (Mevent* mev) {
                                                       /* line 46 */
                                                       /* line 47 *//* line 48 *//* line 49 */}

void destroy_port (Mevent* mev) {
                                                       /* line 50 */
                                                       /* line 51 *//* line 52 *//* line 53 */}

/*  */                                                 /* line 54 */
Str* format_mevent (Mevent* m) {
                                                       /* line 55 */
    if  m ==  NULL:                                    /* line 56 */
        return ( counted("{}"))                        /* line 57 */
    else:                                              /* line 58 */
        return ( str( counted("{%5C”")) +  str(  (*m).port) +  str( counted("%5C”:%5C”")) +  str(   (*m).payload.v) +  counted("%5C”}")    /* line 59 */)/* line 60 *//* line 61 */}

Str* format_mevent_raw (Mevent* m) {
                                                       /* line 62 */
    if  m ==  NULL:                                    /* line 63 */
        return ( counted(""))                          /* line 64 */
    else:                                              /* line 65 */
        return (   (*m).payload.v)                     /* line 66 *//* line 67 *//* line 68 */}
