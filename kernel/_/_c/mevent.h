/* line 1 */
#ifndef Datum_H
#define Datum_H
typedef struct s_Datum {
                                                       /* line 2 */
    Payload* v;                                        /* line 3 */
    Fclone clone;                                      /* line 4 */
    Freclaim reclaim;                                  /* line 5 *//* line 6 */
} Datum;
#endif
                                                       /* line 7 *//* line 8 */
/*  Mevent passed to a leaf component. */              /* line 9 */
/*  */                                                 /* line 10 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 11 */
/*  `payload` is the data attached to this mevent. */  /* line 12 */
#ifndef Mevent_H
#define Mevent_H
typedef struct s_Mevent {
                                                       /* line 13 */
    Port port;                                         /* line 14 */
    Payload* payload;                                  /* line 15 *//* line 16 */
} Mevent;
#endif
                                                       /* line 17 */
/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 22 */
/*  entering the very top of a network. */             /* line 23 */
/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 32 */
/*  Frees a mevent. */                                 /* line 40 */
/*  */                                                 /* line 54 */
