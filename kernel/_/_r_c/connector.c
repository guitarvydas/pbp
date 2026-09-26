/* line 1 */const int
enumDown =  0                                          /* line 2 */;const int
enumAcross =  1                                        /* line 3 */;const int
enumUp =  2                                            /* line 4 */;const int
enumThrough =  3                                       /* line 5 */;/* line 6 *//* line 7 */
/*  Routing connection for a container component. The `direction` field has *//* line 8 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 9 */
/*  purposes, or for reading by other tools. */        /* line 10 *//* line 11 */
typedef struct _Connector {
                                                       /* line 12 */
    direction; /*  down, across, up, through */        /* line 13 */
    sender;                                            /* line 14 */
    receiver;                                          /* line 15 *//* line 16 */
} Connector;
Connector fresh_Connector () {
    Connector *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->direction =  NULL; /*  down, across, up, through *//* line 13 */
    self->sender =  NULL;                              /* line 14 */
    self->receiver =  NULL;                            /* line 15 *//* line 16 */
    return self;
}
                                                       /* line 17 */
/*  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, *//* line 18 */
/*  based on component ID (pointer) and port name. */  /* line 19 *//* line 20 */
typedef struct _Sender {
                                                       /* line 21 */
    name;                                              /* line 22 */
    component;                                         /* line 23 */
    port;                                              /* line 24 *//* line 25 */
} Sender;
Sender fresh_Sender () {
    Sender *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 22 */
    self->component =  NULL;                           /* line 23 */
    self->port =  NULL;                                /* line 24 *//* line 25 */
    return self;
}
                                                       /* line 26 *//* line 27 *//* line 28 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 29 */
/*  to incoming mevents to this queue. */              /* line 30 *//* line 31 */
typedef struct _Receiver {
                                                       /* line 32 */
    name;                                              /* line 33 */
    queue;                                             /* line 34 */
    port;                                              /* line 35 */
    component;                                         /* line 36 *//* line 37 */
} Receiver;
Receiver fresh_Receiver () {
    Receiver *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 33 */
    self->queue =  NULL;                               /* line 34 */
    self->port =  NULL;                                /* line 35 */
    self->component =  NULL;                           /* line 36 *//* line 37 */
    return self;
}
                                                       /* line 38 */
void mkSender (name,component,port) {
                                                       /* line 39 */
    s =  Sender ()                                     /* line 40 */
    s.name =  name                                     /* line 41 */
    s.component =  component                           /* line 42 */
    s.port =  port                                     /* line 43 */
    return ( s)                                        /* line 44 */;;;/* line 45 *//* line 46 */}

void mkReceiver (name,component,port,q) {
                                                       /* line 47 */
    r =  Receiver ()                                   /* line 48 */
    r.name =  name                                     /* line 49 */
    r.component =  component                           /* line 50 */
    r.port =  port                                     /* line 51 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 52 */
    r.queue =  q                                       /* line 53 */
    return ( r)                                        /* line 54 */;;;;/* line 55 */}
