/*  Runtime data of a Part. */                         /* line 1 */
/*  Parts are either leaves (a user-supplied handler) or containers (a handler that routes mevents to children). *//* line 2 */
/*  `handler` is called as handler (eh, mev). */       /* line 3 */
/*  `instance_data` is private to the leaf; the handler finds it again each time it is invoked. *//* line 4 */
/*  `visit_queue` (containers only) holds the children that have been sent a mevent, once for each mevent. *//* line 5 */
/*  `tick` is a function (eh) that continues the processing of an active part. make_container sets it to container_tick; the programmer supplies it to make_leaf. *//* line 6 */
/*  `reset` is a function (eh) that puts the part back into a known state. make_container sets it to container_reset; the programmer supplies it to make_leaf. *//* line 7 */
/*  Eh_States :: enum { idle, active } */              /* line 8 */
#ifndef Eh_H
#define Eh_H
typedef struct s_Eh {
                                                       /* line 9 */
    Str* name;                                         /* line 10 */
    Queue_of_Mevent* inq;
    Queue_of_Mevent* outq;
    Container* owner;                                  /* line 13 */
    List_of_Part* children;                            /* line 14 */
    Queue_of_Part* visit_queue;
    List_of_Wire* connections;                         /* line 16 */
    Fhandler handler;                                  /* line 17 */
    Ftick tick;                                        /* line 18 */
    Freset reset;                                      /* line 19 */
    Any* instance_data;                                /* line 20 */
    Str* arg;                                          /* line 21 */
    Str* state;                                        /* line 22 */
    Bool vip;                                          /* line 23 *//* line 24 */
} Eh;
#endif
