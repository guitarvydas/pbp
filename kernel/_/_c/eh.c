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
typedef struct _Eh {
                                                       /* line 16 */
    name;                                              /* line 17 */
    inq;
    outq;
    owner;                                             /* line 20 */
    children;                                          /* line 21 */
    visit_ordering;
    connections;                                       /* line 23 */
    handler;                                           /* line 24 */
    finject;                                           /* line 25 */
    reset;                                             /* line 26 */
    instance_data;                                     /* line 27 *//*  arg needed for probe support  *//* line 28 */
    arg;                                               /* line 29 */
    state;                                             /* line 30 */
    special;                                           /* line 31 *//* line 32 */
} Eh;
Eh fresh_Eh () {
    Eh *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  "";                                  /* line 17 */
    self->inq =  deque ([])                            /* line 18 */;
    self->outq =  deque ([])                           /* line 19 */;
    self->owner =  NULL;                               /* line 20 */
    self->children = [];                               /* line 21 */
    self->visit_ordering =  deque ([])                 /* line 22 */;
    self->connections = [];                            /* line 23 */
    self->handler =  NULL;                             /* line 24 */
    self->finject =  NULL;                             /* line 25 */
    self->reset =  NULL;                               /* line 26 */
    self->instance_data =  NULL;                       /* line 27 *//*  arg needed for probe support  *//* line 28 */
    self->arg =  "";                                   /* line 29 */
    self->state =  "idle";                             /* line 30 */
    self->special =  False;                            /* line 31 *//* line 32 */
    return self;
}
                                                       /* line 33 */
void injector (eh,mevent) {
                                                       /* line 34 */
    eh.handler ( eh, mevent)                           /* line 35 *//* line 36 *//* line 37 */}
