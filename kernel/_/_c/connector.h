/* line 1 *//* line 6 *//* line 7 */
/*  Routing connection for a container component. The `direction` field has *//* line 8 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 9 */
/*  purposes, or for reading by other tools. */        /* line 10 *//* line 11 */
#ifndef Connector_H
#define Connector_H
typedef struct s_Connector {
                                                       /* line 12 */
    Dir direction; /*  down, across, up, through */    /* line 13 */
    Sender* sender;                                    /* line 14 */
    Receiver* receiver;                                /* line 15 *//* line 16 */
} Connector;
#endif
                                                       /* line 17 */
/*  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, *//* line 18 */
/*  based on component ID (pointer) and port name. */  /* line 19 *//* line 20 */
#ifndef Sender_H
#define Sender_H
typedef struct s_Sender {
                                                       /* line 21 */
    Str* name;                                         /* line 22 */
    Eh* component;                                     /* line 23 */
    Port port;                                         /* line 24 *//* line 25 */
} Sender;
#endif
                                                       /* line 26 *//* line 27 *//* line 28 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 29 */
/*  to incoming mevents to this queue. */              /* line 30 *//* line 31 */
#ifndef Receiver_H
#define Receiver_H
typedef struct s_Receiver {
                                                       /* line 32 */
    Str* name;                                         /* line 33 */
    Queue* queue;                                      /* line 34 */
    Port port;                                         /* line 35 */
    Eh* component;                                     /* line 36 *//* line 37 */
} Receiver;
#endif
                                                       /* line 38 */
