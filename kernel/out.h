/*  Directions of wires. This section is the only place that knows how a direction is represented *//* line 1 */
/*  (here: strings). To use integers, change the four constants and direction_label; nothing else changes. *//* line 2 *//* line 7 */
/*  d is the number used for a direction in the diagram JSON: 0 down, 1 across, 2 up, 3 through. *//* line 8 */
/*  Returns ϕ for any other number. */                 /* line 9 */
/*  direction   sender                   receiver */   /* line 28 */
/*  down        the container            a child */    /* line 29 */
/*  across      a child                  a child */    /* line 30 */
/*  up          a child                  the container *//* line 31 */
/*  through     the container            the container *//* line 32 */
/*  a name for messages and debugging */               /* line 45 */
/*  Routing connection (wire) for a container. The `direction` field has no effect *//* line 50 */
/*  on routing; it is there for debugging, or for reading by other tools. *//* line 51 */
#ifndef Connector_H
#define Connector_H
typedef struct s_Connector {
                                                       /* line 52 */
    Dir direction; /*  down, across, up, through */    /* line 53 */
    Sender* sender;                                    /* line 54 */
    Receiver* receiver;                                /* line 55 *//* line 56 */
} Connector;
#endif
                                                       /* line 57 */
/*  `Sender` is matched on the part (identity) and the port name. *//* line 58 */
#ifndef Sender_H
#define Sender_H
typedef struct s_Sender {
                                                       /* line 59 */
    Eh* component;                                     /* line 60 */
    Port port;                                         /* line 61 *//* line 62 */
} Sender;
#endif
                                                       /* line 63 */
/*  `Receiver` is the part and port that a mevent is delivered to. *//* line 64 */
/*  When the receiver is the container itself (up and through wires) the port is an output gate. *//* line 65 */
#ifndef Receiver_H
#define Receiver_H
typedef struct s_Receiver {
                                                       /* line 66 */
    Eh* component;                                     /* line 67 */
    Port port;                                         /* line 68 *//* line 69 */
} Receiver;
#endif
                                                       /* line 70 */
/*  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, *//* line 93 */
/*  The sender port is proto_conn@"source_port" and the receiver port is proto_conn@"target_port". *//* line 94 */
/*  A child sender is proto_conn@"source", a child receiver is proto_conn@"target". *//* line 95 */
/*  Returns ϕ if the wire can not be made. */          /* line 96 */
