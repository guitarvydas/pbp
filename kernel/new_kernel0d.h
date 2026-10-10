#include "pbp.h"

#include "pbp.h"                                       /* line 1 */
#include "pbp.h"

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
typedef struct s_Eh {
                                                       /* line 16 */
    Str* name;                                         /* line 17 */
    Queue* inq;
    Queue* outq;
    Container* owner;                                  /* line 20 */
    List_of_Part* children;                            /* line 21 */
    Queue_of_Part* visit_ordering;
    List_ofWire* connections;                          /* line 23 */
    Fhandler handler;                                  /* line 24 */
    Finject finject;                                   /* line 25 */
    Freset reset;                                      /* line 26 */
    any* instance_data;                                /* line 27 *//*  arg needed for probe support  *//* line 28 */
    Str* arg;                                          /* line 29 */
    Str* state;                                        /* line 30 */
    Bool special;                                      /* line 31 *//* line 32 */
} Eh;
                                                       /* line 33 */
#include "pbp.h"
                                                       /* line 8 *//* line 9 *//* line 20 */
#include "pbp.h"
                                                       /* line 1 */
typedef struct s_Datum {
                                                       /* line 2 */
    Payload* v;                                        /* line 3 */
    Fclone clone;                                      /* line 4 */
    Freclaim reclaim;                                  /* line 5 */
    any* other; /*  reserved for use on per-project basis  *//* line 6 *//* line 7 */
} Datum;
                                                       /* line 8 *//* line 9 */
/*  Mevent passed to a leaf component. */              /* line 10 */
/*  */                                                 /* line 11 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 12 */
/*  `payload` is the data attached to this mevent. */  /* line 13 */
typedef struct s_Mevent {
                                                       /* line 14 */
    Port port;                                         /* line 15 */
    Payload* payload;                                  /* line 16 *//* line 17 */
} Mevent;
                                                       /* line 18 */
/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 23 */
/*  entering the very top of a network. */             /* line 24 */
/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 33 */
/*  Frees a mevent. */                                 /* line 41 */
/*  */                                                 /* line 55 */
#include "pbp.h"
                                                       /* line 1 *//* line 6 *//* line 7 */
/*  Routing connection for a container component. The `direction` field has *//* line 8 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 9 */
/*  purposes, or for reading by other tools. */        /* line 10 *//* line 11 */
typedef struct s_Connector {
                                                       /* line 12 */
    Dir direction; /*  down, across, up, through */    /* line 13 */
    Sender* sender;                                    /* line 14 */
    Receiver* receiver;                                /* line 15 *//* line 16 */
} Connector;
                                                       /* line 17 */
/*  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, *//* line 18 */
/*  based on component ID (pointer) and port name. */  /* line 19 *//* line 20 */
typedef struct s_Sender {
                                                       /* line 21 */
    Str* name;                                         /* line 22 */
    Eh* component;                                     /* line 23 */
    Port port;                                         /* line 24 *//* line 25 */
} Sender;
                                                       /* line 26 *//* line 27 *//* line 28 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 29 */
/*  to incoming mevents to this queue. */              /* line 30 *//* line 31 */
typedef struct s_Receiver {
                                                       /* line 32 */
    Str* name;                                         /* line 33 */
    Queue* queue;                                      /* line 34 */
    Port port;                                         /* line 35 */
    Eh* component;                                     /* line 36 *//* line 37 */
} Receiver;
                                                       /* line 38 */
#include "pbp.h"

typedef struct s_Component_Registry {
                                                       /* line 1 */
    Dict_of_Template* templates;                       /* line 2 *//* line 3 */
} Component_Registry;
                                                       /* line 4 */
typedef struct s_Template {
                                                       /* line 5 */
    Str* name;                                         /* line 6 */
    Container* container;                              /* line 7 */
    Finstantiator instantiator;                        /* line 8 *//* line 9 */
} Template;
                                                       /* line 10 *//* line 19 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 20 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 21 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 22 *//* line 23 */
/*  ... by reading the little-net from an external file  *//* line 24 */
/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 31 */
#include "pbp.h"
                                                       /* line 56 */
/*  The default handler for container components. */   /* line 92 */
/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 99 */
/*  Frees the given container and associated data. */  /* line 110 */
/*  Checks if two senders match, by pointer equality and port name matching. *//* line 114 */
/*  Delivers the given mevent to the receiver of this connector. *//* line 121 *//* line 122 */
/*  Routes a single mevent to all matching destinations, according to *//* line 207 */
/*  the container;s connection network. */             /* line 208 *//* line 209 *//* line 250 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 251 */
/*  whose handler function is `container_handler`. */  /* line 252 */
/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 265 */
/*  of the given component. */                         /* line 266 *//* line 267 */
#include "pbp.h"

/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 22 */
#include "pbp.h"

/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
#include "pbp.h"
                                                       /* line 5 */
typedef struct s_TwoMevents {
                                                       /* line 15 */
    Mevent* firstmev;                                  /* line 16 */
    Mevent* secondmev;                                 /* line 17 *//* line 18 */
} TwoMevents;
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
typedef struct s_Deracer_Instance_Data {
                                                       /* line 21 */
    State state;                                       /* line 22 */
    TwoMevents* buffer;                                /* line 23 *//* line 24 */
} Deracer_Instance_Data;
                                                       /* line 25 */
typedef struct s_Syncfilewrite_Data {
                                                       /* line 108 */
    Str* filename;                                     /* line 109 *//* line 110 */
} Syncfilewrite_Data;
                                                       /* line 111 */
/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
typedef struct s_StringConcat_Instance_Data {
                                                       /* line 140 */
    Str* buffer1;                                      /* line 141 */
    Str* buffer2;                                      /* line 142 *//* line 143 */
} StringConcat_Instance_Data;
                                                       /* line 144 */
/*  */                                                 /* line 189 *//* line 190 *//* line 192 *//* line 214 *//* line 221 */
typedef struct s_Switch1star_Instance_Data {
                                                       /* line 222 */
    Str* state;                                        /* line 223 *//* line 224 */
} Switch1star_Instance_Data;
                                                       /* line 225 */
typedef struct s_StringAccumulator {
                                                       /* line 256 */
    Str* s;                                            /* line 257 *//* line 258 */
} StringAccumulator;
                                                       /* line 259 */
/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
#include "pbp.h"
                                                       /* line 4 *//* line 19 */
#include "pbp.h"                                       /* line 1 */
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
typedef struct s_Eh {
                                                       /* line 16 */
    Str* name;                                         /* line 17 */
    Queue* inq;
    Queue* outq;
    Container* owner;                                  /* line 20 */
    List_of_Part* children;                            /* line 21 */
    Queue_of_Part* visit_ordering;
    List_ofWire* connections;                          /* line 23 */
    Fhandler handler;                                  /* line 24 */
    Finject finject;                                   /* line 25 */
    Freset reset;                                      /* line 26 */
    any* instance_data;                                /* line 27 *//*  arg needed for probe support  *//* line 28 */
    Str* arg;                                          /* line 29 */
    Str* state;                                        /* line 30 */
    Bool special;                                      /* line 31 *//* line 32 */
} Eh;
                                                       /* line 33 */
/* line 8 *//* line 9 *//* line 20 */
/* line 1 */
typedef struct s_Datum {
                                                       /* line 2 */
    Payload* v;                                        /* line 3 */
    Fclone clone;                                      /* line 4 */
    Freclaim reclaim;                                  /* line 5 */
    any* other; /*  reserved for use on per-project basis  *//* line 6 *//* line 7 */
} Datum;
                                                       /* line 8 *//* line 9 */
/*  Mevent passed to a leaf component. */              /* line 10 */
/*  */                                                 /* line 11 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 12 */
/*  `payload` is the data attached to this mevent. */  /* line 13 */
typedef struct s_Mevent {
                                                       /* line 14 */
    Port port;                                         /* line 15 */
    Payload* payload;                                  /* line 16 *//* line 17 */
} Mevent;
                                                       /* line 18 */
/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 23 */
/*  entering the very top of a network. */             /* line 24 */
/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 33 */
/*  Frees a mevent. */                                 /* line 41 */
/*  */                                                 /* line 55 */
/* line 1 *//* line 6 *//* line 7 */
/*  Routing connection for a container component. The `direction` field has *//* line 8 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 9 */
/*  purposes, or for reading by other tools. */        /* line 10 *//* line 11 */
typedef struct s_Connector {
                                                       /* line 12 */
    Dir direction; /*  down, across, up, through */    /* line 13 */
    Sender* sender;                                    /* line 14 */
    Receiver* receiver;                                /* line 15 *//* line 16 */
} Connector;
                                                       /* line 17 */
/*  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, *//* line 18 */
/*  based on component ID (pointer) and port name. */  /* line 19 *//* line 20 */
typedef struct s_Sender {
                                                       /* line 21 */
    Str* name;                                         /* line 22 */
    Eh* component;                                     /* line 23 */
    Port port;                                         /* line 24 *//* line 25 */
} Sender;
                                                       /* line 26 *//* line 27 *//* line 28 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 29 */
/*  to incoming mevents to this queue. */              /* line 30 *//* line 31 */
typedef struct s_Receiver {
                                                       /* line 32 */
    Str* name;                                         /* line 33 */
    Queue* queue;                                      /* line 34 */
    Port port;                                         /* line 35 */
    Eh* component;                                     /* line 36 *//* line 37 */
} Receiver;
                                                       /* line 38 */
typedef struct s_Component_Registry {
                                                       /* line 1 */
    Dict_of_Template* templates;                       /* line 2 *//* line 3 */
} Component_Registry;
                                                       /* line 4 */
typedef struct s_Template {
                                                       /* line 5 */
    Str* name;                                         /* line 6 */
    Container* container;                              /* line 7 */
    Finstantiator instantiator;                        /* line 8 *//* line 9 */
} Template;
                                                       /* line 10 *//* line 19 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 20 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 21 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 22 *//* line 23 */
/*  ... by reading the little-net from an external file  *//* line 24 */
/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 31 */
/* line 56 */
/*  The default handler for container components. */   /* line 92 */
/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 99 */
/*  Frees the given container and associated data. */  /* line 110 */
/*  Checks if two senders match, by pointer equality and port name matching. *//* line 114 */
/*  Delivers the given mevent to the receiver of this connector. *//* line 121 *//* line 122 */
/*  Routes a single mevent to all matching destinations, according to *//* line 207 */
/*  the container;s connection network. */             /* line 208 *//* line 209 *//* line 250 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 251 */
/*  whose handler function is `container_handler`. */  /* line 252 */
/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 265 */
/*  of the given component. */                         /* line 266 *//* line 267 */
/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 22 */
/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
/* line 5 */
typedef struct s_TwoMevents {
                                                       /* line 15 */
    Mevent* firstmev;                                  /* line 16 */
    Mevent* secondmev;                                 /* line 17 *//* line 18 */
} TwoMevents;
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
typedef struct s_Deracer_Instance_Data {
                                                       /* line 21 */
    State state;                                       /* line 22 */
    TwoMevents* buffer;                                /* line 23 *//* line 24 */
} Deracer_Instance_Data;
                                                       /* line 25 */
typedef struct s_Syncfilewrite_Data {
                                                       /* line 108 */
    Str* filename;                                     /* line 109 *//* line 110 */
} Syncfilewrite_Data;
                                                       /* line 111 */
/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
typedef struct s_StringConcat_Instance_Data {
                                                       /* line 140 */
    Str* buffer1;                                      /* line 141 */
    Str* buffer2;                                      /* line 142 *//* line 143 */
} StringConcat_Instance_Data;
                                                       /* line 144 */
/*  */                                                 /* line 189 *//* line 190 *//* line 192 *//* line 214 *//* line 221 */
typedef struct s_Switch1star_Instance_Data {
                                                       /* line 222 */
    Str* state;                                        /* line 223 *//* line 224 */
} Switch1star_Instance_Data;
                                                       /* line 225 */
typedef struct s_StringAccumulator {
                                                       /* line 256 */
    Str* s;                                            /* line 257 *//* line 258 */
} StringAccumulator;
                                                       /* line 259 */
/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
/* line 4 *//* line 19 */
#include "pbp.h"                                       /* line 1 */
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
    Queue* inq;
    Queue* outq;
    Container* owner;                                  /* line 20 */
    List_of_Part* children;                            /* line 21 */
    Queue_of_Part* visit_ordering;
    List_of_Wire* connections;                         /* line 23 */
    Fhandler handler;                                  /* line 24 */
    Finject finject;                                   /* line 25 */
    Freset reset;                                      /* line 26 */
    any* instance_data;                                /* line 27 *//*  arg needed for probe support  *//* line 28 */
    Str* arg;                                          /* line 29 */
    Str* state;                                        /* line 30 */
    Bool special;                                      /* line 31 *//* line 32 */
} Eh;
#endif
                                                       /* line 33 */
/* line 8 *//* line 9 *//* line 20 */
/* line 1 */
#ifndef Datum_H
#define Datum_H
typedef struct s_Datum {
                                                       /* line 2 */
    Payload* v;                                        /* line 3 */
    Fclone clone;                                      /* line 4 */
    Freclaim reclaim;                                  /* line 5 */
    any* other; /*  reserved for use on per-project basis  *//* line 6 *//* line 7 */
} Datum;
#endif
                                                       /* line 8 *//* line 9 */
/*  Mevent passed to a leaf component. */              /* line 10 */
/*  */                                                 /* line 11 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 12 */
/*  `payload` is the data attached to this mevent. */  /* line 13 */
#ifndef Mevent_H
#define Mevent_H
typedef struct s_Mevent {
                                                       /* line 14 */
    Port port;                                         /* line 15 */
    Payload* payload;                                  /* line 16 *//* line 17 */
} Mevent;
#endif
                                                       /* line 18 */
/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 23 */
/*  entering the very top of a network. */             /* line 24 */
/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 33 */
/*  Frees a mevent. */                                 /* line 41 */
/*  */                                                 /* line 55 */
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
#ifndef Component_Registry_H
#define Component_Registry_H
typedef struct s_Component_Registry {
                                                       /* line 1 */
    Dict_of_Template* templates;                       /* line 2 *//* line 3 */
} Component_Registry;
#endif
                                                       /* line 4 */
#ifndef Template_H
#define Template_H
typedef struct s_Template {
                                                       /* line 5 */
    Str* name;                                         /* line 6 */
    Container* container;                              /* line 7 */
    Finstantiator instantiator;                        /* line 8 *//* line 9 */
} Template;
#endif
                                                       /* line 10 *//* line 19 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 20 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 21 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 22 *//* line 23 */
/*  ... by reading the little-net from an external file  *//* line 24 */
/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 31 */
/* line 56 */
/*  The default handler for container components. */   /* line 92 */
/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 99 */
/*  Frees the given container and associated data. */  /* line 110 */
/*  Checks if two senders match, by pointer equality and port name matching. *//* line 114 */
/*  Delivers the given mevent to the receiver of this connector. *//* line 121 *//* line 122 */
/*  Routes a single mevent to all matching destinations, according to *//* line 207 */
/*  the container;s connection network. */             /* line 208 *//* line 209 *//* line 250 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 251 */
/*  whose handler function is `container_handler`. */  /* line 252 */
/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 265 */
/*  of the given component. */                         /* line 266 *//* line 267 */
/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 22 */
/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
/* line 5 */
#ifndef TwoMevents_H
#define TwoMevents_H
typedef struct s_TwoMevents {
                                                       /* line 15 */
    Mevent* firstmev;                                  /* line 16 */
    Mevent* secondmev;                                 /* line 17 *//* line 18 */
} TwoMevents;
#endif
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
#ifndef Deracer_Instance_Data_H
#define Deracer_Instance_Data_H
typedef struct s_Deracer_Instance_Data {
                                                       /* line 21 */
    State state;                                       /* line 22 */
    TwoMevents* buffer;                                /* line 23 *//* line 24 */
} Deracer_Instance_Data;
#endif
                                                       /* line 25 */
#ifndef Syncfilewrite_Data_H
#define Syncfilewrite_Data_H
typedef struct s_Syncfilewrite_Data {
                                                       /* line 108 */
    Str* filename;                                     /* line 109 *//* line 110 */
} Syncfilewrite_Data;
#endif
                                                       /* line 111 */
/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
#ifndef StringConcat_Instance_Data_H
#define StringConcat_Instance_Data_H
typedef struct s_StringConcat_Instance_Data {
                                                       /* line 140 */
    Str* buffer1;                                      /* line 141 */
    Str* buffer2;                                      /* line 142 *//* line 143 */
} StringConcat_Instance_Data;
#endif
                                                       /* line 144 */
/*  */                                                 /* line 189 *//* line 190 *//* line 192 *//* line 214 *//* line 221 */
#ifndef Switch1star_Instance_Data_H
#define Switch1star_Instance_Data_H
typedef struct s_Switch1star_Instance_Data {
                                                       /* line 222 */
    Str* state;                                        /* line 223 *//* line 224 */
} Switch1star_Instance_Data;
#endif
                                                       /* line 225 */
#ifndef StringAccumulator_H
#define StringAccumulator_H
typedef struct s_StringAccumulator {
                                                       /* line 256 */
    Str* s;                                            /* line 257 *//* line 258 */
} StringAccumulator;
#endif
                                                       /* line 259 */
/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
/* line 4 *//* line 19 */
#include "pbp.h"                                       /* line 1 */
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
    any* instance_data;                                /* line 27 *//*  arg needed for probe support  *//* line 28 */
    Str* arg;                                          /* line 29 */
    Str* state;                                        /* line 30 */
    Bool special;                                      /* line 31 *//* line 32 */
} Eh;
#endif
                                                       /* line 33 */
/* line 8 *//* line 9 *//* line 20 */
/* line 1 */
#ifndef Datum_H
#define Datum_H
typedef struct s_Datum {
                                                       /* line 2 */
    Payload* v;                                        /* line 3 */
    Fclone clone;                                      /* line 4 */
    Freclaim reclaim;                                  /* line 5 */
    any* other; /*  reserved for use on per-project basis  *//* line 6 *//* line 7 */
} Datum;
#endif
                                                       /* line 8 *//* line 9 */
/*  Mevent passed to a leaf component. */              /* line 10 */
/*  */                                                 /* line 11 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 12 */
/*  `payload` is the data attached to this mevent. */  /* line 13 */
#ifndef Mevent_H
#define Mevent_H
typedef struct s_Mevent {
                                                       /* line 14 */
    Port port;                                         /* line 15 */
    Payload* payload;                                  /* line 16 *//* line 17 */
} Mevent;
#endif
                                                       /* line 18 */
/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 23 */
/*  entering the very top of a network. */             /* line 24 */
/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 33 */
/*  Frees a mevent. */                                 /* line 41 */
/*  */                                                 /* line 55 */
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
#ifndef Component_Registry_H
#define Component_Registry_H
typedef struct s_Component_Registry {
                                                       /* line 1 */
    Dict_of_Template* templates;                       /* line 2 *//* line 3 */
} Component_Registry;
#endif
                                                       /* line 4 */
#ifndef Template_H
#define Template_H
typedef struct s_Template {
                                                       /* line 5 */
    Str* name;                                         /* line 6 */
    Container* container;                              /* line 7 */
    Finstantiator instantiator;                        /* line 8 *//* line 9 */
} Template;
#endif
                                                       /* line 10 *//* line 19 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 20 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 21 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 22 *//* line 23 */
/*  ... by reading the little-net from an external file  *//* line 24 */
/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 31 */
/* line 56 */
/*  The default handler for container components. */   /* line 92 */
/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 99 */
/*  Frees the given container and associated data. */  /* line 110 */
/*  Checks if two senders match, by pointer equality and port name matching. *//* line 114 */
/*  Delivers the given mevent to the receiver of this connector. *//* line 121 *//* line 122 */
/*  Routes a single mevent to all matching destinations, according to *//* line 207 */
/*  the container;s connection network. */             /* line 208 *//* line 209 *//* line 250 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 251 */
/*  whose handler function is `container_handler`. */  /* line 252 */
/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 265 */
/*  of the given component. */                         /* line 266 *//* line 267 */
/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 22 */
/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
/* line 5 */
#ifndef TwoMevents_H
#define TwoMevents_H
typedef struct s_TwoMevents {
                                                       /* line 15 */
    Mevent* firstmev;                                  /* line 16 */
    Mevent* secondmev;                                 /* line 17 *//* line 18 */
} TwoMevents;
#endif
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
#ifndef Deracer_Instance_Data_H
#define Deracer_Instance_Data_H
typedef struct s_Deracer_Instance_Data {
                                                       /* line 21 */
    State state;                                       /* line 22 */
    TwoMevents* buffer;                                /* line 23 *//* line 24 */
} Deracer_Instance_Data;
#endif
                                                       /* line 25 */
#ifndef Syncfilewrite_Data_H
#define Syncfilewrite_Data_H
typedef struct s_Syncfilewrite_Data {
                                                       /* line 108 */
    Str* filename;                                     /* line 109 *//* line 110 */
} Syncfilewrite_Data;
#endif
                                                       /* line 111 */
/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
#ifndef StringConcat_Instance_Data_H
#define StringConcat_Instance_Data_H
typedef struct s_StringConcat_Instance_Data {
                                                       /* line 140 */
    Str* buffer1;                                      /* line 141 */
    Str* buffer2;                                      /* line 142 *//* line 143 */
} StringConcat_Instance_Data;
#endif
                                                       /* line 144 */
/*  */                                                 /* line 189 *//* line 190 *//* line 192 *//* line 214 *//* line 221 */
#ifndef Switch1star_Instance_Data_H
#define Switch1star_Instance_Data_H
typedef struct s_Switch1star_Instance_Data {
                                                       /* line 222 */
    Str* state;                                        /* line 223 *//* line 224 */
} Switch1star_Instance_Data;
#endif
                                                       /* line 225 */
#ifndef StringAccumulator_H
#define StringAccumulator_H
typedef struct s_StringAccumulator {
                                                       /* line 256 */
    Str* s;                                            /* line 257 *//* line 258 */
} StringAccumulator;
#endif
                                                       /* line 259 */
/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
/* line 4 *//* line 19 */
#include "pbp.h"                                       /* line 1 */
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
    any* instance_data;                                /* line 27 *//*  arg needed for probe support  *//* line 28 */
    Str* arg;                                          /* line 29 */
    Str* state;                                        /* line 30 */
    Bool special;                                      /* line 31 *//* line 32 */
} Eh;
#endif
                                                       /* line 33 */
/* line 8 *//* line 9 *//* line 20 */
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
#ifndef Component_Registry_H
#define Component_Registry_H
typedef struct s_Component_Registry {
                                                       /* line 1 */
    Dict_of_Template* templates;                       /* line 2 *//* line 3 */
} Component_Registry;
#endif
                                                       /* line 4 */
#ifndef Template_H
#define Template_H
typedef struct s_Template {
                                                       /* line 5 */
    Str* name;                                         /* line 6 */
    Container* container;                              /* line 7 */
    Finstantiator instantiator;                        /* line 8 *//* line 9 */
} Template;
#endif
                                                       /* line 10 *//* line 19 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 20 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 21 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 22 *//* line 23 */
/*  ... by reading the little-net from an external file  *//* line 24 */
/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 31 */
/* line 56 */
/*  The default handler for container components. */   /* line 92 */
/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 99 */
/*  Frees the given container and associated data. */  /* line 110 */
/*  Checks if two senders match, by pointer equality and port name matching. *//* line 114 */
/*  Delivers the given mevent to the receiver of this connector. *//* line 121 *//* line 122 */
/*  Routes a single mevent to all matching destinations, according to *//* line 207 */
/*  the container;s connection network. */             /* line 208 *//* line 209 *//* line 250 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 251 */
/*  whose handler function is `container_handler`. */  /* line 252 */
/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 265 */
/*  of the given component. */                         /* line 266 *//* line 267 */
/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 22 */
/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
/* line 5 */
#ifndef TwoMevents_H
#define TwoMevents_H
typedef struct s_TwoMevents {
                                                       /* line 15 */
    Mevent* firstmev;                                  /* line 16 */
    Mevent* secondmev;                                 /* line 17 *//* line 18 */
} TwoMevents;
#endif
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
#ifndef Deracer_Instance_Data_H
#define Deracer_Instance_Data_H
typedef struct s_Deracer_Instance_Data {
                                                       /* line 21 */
    State state;                                       /* line 22 */
    TwoMevents* buffer;                                /* line 23 *//* line 24 */
} Deracer_Instance_Data;
#endif
                                                       /* line 25 */
#ifndef Syncfilewrite_Data_H
#define Syncfilewrite_Data_H
typedef struct s_Syncfilewrite_Data {
                                                       /* line 108 */
    Str* filename;                                     /* line 109 *//* line 110 */
} Syncfilewrite_Data;
#endif
                                                       /* line 111 */
/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
#ifndef StringConcat_Instance_Data_H
#define StringConcat_Instance_Data_H
typedef struct s_StringConcat_Instance_Data {
                                                       /* line 140 */
    Str* buffer1;                                      /* line 141 */
    Str* buffer2;                                      /* line 142 *//* line 143 */
} StringConcat_Instance_Data;
#endif
                                                       /* line 144 */
/*  */                                                 /* line 189 *//* line 190 *//* line 192 *//* line 214 *//* line 221 */
#ifndef Switch1star_Instance_Data_H
#define Switch1star_Instance_Data_H
typedef struct s_Switch1star_Instance_Data {
                                                       /* line 222 */
    Str* state;                                        /* line 223 *//* line 224 */
} Switch1star_Instance_Data;
#endif
                                                       /* line 225 */
#ifndef StringAccumulator_H
#define StringAccumulator_H
typedef struct s_StringAccumulator {
                                                       /* line 256 */
    Str* s;                                            /* line 257 *//* line 258 */
} StringAccumulator;
#endif
                                                       /* line 259 */
/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
/* line 4 *//* line 19 */
#include "pbp.h"                                       /* line 1 */
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
/* line 8 *//* line 9 *//* line 20 */
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
#ifndef Component_Registry_H
#define Component_Registry_H
typedef struct s_Component_Registry {
                                                       /* line 1 */
    Dict_of_Template* templates;                       /* line 2 *//* line 3 */
} Component_Registry;
#endif
                                                       /* line 4 */
#ifndef Template_H
#define Template_H
typedef struct s_Template {
                                                       /* line 5 */
    Str* name;                                         /* line 6 */
    Container* container;                              /* line 7 */
    Finstantiator instantiator;                        /* line 8 *//* line 9 */
} Template;
#endif
                                                       /* line 10 *//* line 19 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 20 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 21 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 22 *//* line 23 */
/*  ... by reading the little-net from an external file  *//* line 24 */
/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 31 */
/* line 56 */
/*  The default handler for container components. */   /* line 92 */
/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 99 */
/*  Frees the given container and associated data. */  /* line 110 */
/*  Checks if two senders match, by pointer equality and port name matching. *//* line 114 */
/*  Delivers the given mevent to the receiver of this connector. *//* line 121 *//* line 122 */
/*  Routes a single mevent to all matching destinations, according to *//* line 207 */
/*  the container;s connection network. */             /* line 208 *//* line 209 *//* line 250 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 251 */
/*  whose handler function is `container_handler`. */  /* line 252 */
/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 265 */
/*  of the given component. */                         /* line 266 *//* line 267 */
/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 22 */
/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
/* line 5 */
#ifndef TwoMevents_H
#define TwoMevents_H
typedef struct s_TwoMevents {
                                                       /* line 15 */
    Mevent* firstmev;                                  /* line 16 */
    Mevent* secondmev;                                 /* line 17 *//* line 18 */
} TwoMevents;
#endif
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
#ifndef Deracer_Instance_Data_H
#define Deracer_Instance_Data_H
typedef struct s_Deracer_Instance_Data {
                                                       /* line 21 */
    State state;                                       /* line 22 */
    TwoMevents* buffer;                                /* line 23 *//* line 24 */
} Deracer_Instance_Data;
#endif
                                                       /* line 25 */
#ifndef Syncfilewrite_Data_H
#define Syncfilewrite_Data_H
typedef struct s_Syncfilewrite_Data {
                                                       /* line 108 */
    Str* filename;                                     /* line 109 *//* line 110 */
} Syncfilewrite_Data;
#endif
                                                       /* line 111 */
/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
#ifndef StringConcat_Instance_Data_H
#define StringConcat_Instance_Data_H
typedef struct s_StringConcat_Instance_Data {
                                                       /* line 140 */
    Str* buffer1;                                      /* line 141 */
    Str* buffer2;                                      /* line 142 *//* line 143 */
} StringConcat_Instance_Data;
#endif
                                                       /* line 144 */
/*  */                                                 /* line 189 *//* line 190 *//* line 192 *//* line 214 *//* line 221 */
#ifndef Switch1star_Instance_Data_H
#define Switch1star_Instance_Data_H
typedef struct s_Switch1star_Instance_Data {
                                                       /* line 222 */
    Str* state;                                        /* line 223 *//* line 224 */
} Switch1star_Instance_Data;
#endif
                                                       /* line 225 */
#ifndef StringAccumulator_H
#define StringAccumulator_H
typedef struct s_StringAccumulator {
                                                       /* line 256 */
    Str* s;                                            /* line 257 *//* line 258 */
} StringAccumulator;
#endif
                                                       /* line 259 */
/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
/* line 4 *//* line 19 */
