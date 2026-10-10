/*  Data for an asyncronous component _ effectively, a function with input *//* line 1 */
/*  and output queues of mevents. */                   /* line 2 */
/*  */                                                 /* line 3 */
/*  Components can either be a user_supplied function ("leaf“), or a “container“ *//* line 4 */
/*  that routes mevents to child components according to a list of connections *//* line 5 */
/*  that serve as a mevent routing table. */           /* line 6 */
/*  */                                                 /* line 7 */
/*  Child components themselves can be leaves or other containers. *//* line 8 */
/*  */                                                 /* line 9 */
/*  `handler` invokes the code that is attached to this component. *//* line 10 */
/*  */                                                 /* line 11 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 12 */
/*  function may want whenever it is invoked again. */ /* line 13 *//* line 14 */
/*  Eh_States :: enum { idle, active } */              /* line 15 */
#ifndef Eh_H
#define Eh_H
typedef struct s_Eh {
                                                       /* line 16 */
    Str* name;                                         /* line 17 */
    Queue_of_Mevent* inq;
    Queue_of_Mevent* outq;
    Container* owner;                                  /* line 20 */
    List_of_Part* children;                            /* line 21 */
    Queue_of_Part* visit_ordering;
    List_of_Wire* connections;                         /* line 23 */
    Fhandler handler;                                  /* line 24 */
    Finject finject;                                   /* line 25 */
    Freset reset;                                      /* line 26 */
    Any* instance_data;                                /* line 27 *//*  arg needed for probe support  *//* line 28 */
    Str* arg;                                          /* line 29 */
    Str* state;                                        /* line 30 */
    Bool special;                                      /* line 31 *//* line 32 */
} Eh;
#endif
                                                       /* line 33 */
