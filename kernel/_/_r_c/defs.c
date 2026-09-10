/* line 1 *//* line 2 */
int  counter =  0                                      /* line 3 */;
int  ticktime =  0                                     /* line 4 */;/* line 5 */const int
enumDown =  0                                          /* line 6 */;const int
enumAcross =  1                                        /* line 7 */;const int
enumUp =  2                                            /* line 8 */;const int
enumThrough =  3                                       /* line 9 */;/* line 10 *//* line 11 */
/*  Routing connection for a container component. The `direction` field has *//* line 12 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 13 */
/*  purposes, or for reading by other tools. */        /* line 14 *//* line 15 */
typedef struct _Connector {
                                                       /* line 16 */
    direction; /*  down, across, up, through */        /* line 17 */
    sender;                                            /* line 18 */
    receiver;                                          /* line 19 *//* line 20 */
} Connector;
Connector fresh_Connector () {
    Connector *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->direction =  NULL; /*  down, across, up, through *//* line 17 */
    self->sender =  NULL;                              /* line 18 */
    self->receiver =  NULL;                            /* line 19 *//* line 20 */
    return self;
}
                                                       /* line 21 */
/*  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, *//* line 22 */
/*  based on component ID (pointer) and port name. */  /* line 23 *//* line 24 */
typedef struct _Sender {
                                                       /* line 25 */
    name;                                              /* line 26 */
    component;                                         /* line 27 */
    port;                                              /* line 28 *//* line 29 */
} Sender;
Sender fresh_Sender () {
    Sender *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 26 */
    self->component =  NULL;                           /* line 27 */
    self->port =  NULL;                                /* line 28 *//* line 29 */
    return self;
}
                                                       /* line 30 *//* line 31 *//* line 32 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 33 */
/*  to incoming mevents to this queue. */              /* line 34 *//* line 35 */
typedef struct _Receiver {
                                                       /* line 36 */
    name;                                              /* line 37 */
    queue;                                             /* line 38 */
    port;                                              /* line 39 */
    component;                                         /* line 40 *//* line 41 */
} Receiver;
Receiver fresh_Receiver () {
    Receiver *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 37 */
    self->queue =  NULL;                               /* line 38 */
    self->port =  NULL;                                /* line 39 */
    self->component =  NULL;                           /* line 40 *//* line 41 */
    return self;
}
                                                       /* line 42 */
void mkSender (name,component,port) {
                                                       /* line 43 */
    s =  Sender ()                                     /* line 44 */
    s.name =  name                                     /* line 45 */
    s.component =  component                           /* line 46 */
    s.port =  port                                     /* line 47 */
    return ( s)                                        /* line 48 */;;;/* line 49 *//* line 50 */}

void mkReceiver (name,component,port,q) {
                                                       /* line 51 */
    r =  Receiver ()                                   /* line 52 */
    r.name =  name                                     /* line 53 */
    r.component =  component                           /* line 54 */
    r.port =  port                                     /* line 55 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 56 */
    r.queue =  q                                       /* line 57 */
    return ( r)                                        /* line 58 */;;;;/* line 59 *//* line 60 */}
                                                       /* line 61 */
typedef struct _Component_Registry {
                                                       /* line 62 */
    templates;                                         /* line 63 *//* line 64 */
} Component_Registry;
Component_Registry fresh_Component_Registry () {
    Component_Registry *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->templates = {};                              /* line 63 *//* line 64 */
    return self;
}
                                                       /* line 65 */
typedef struct _Template {
                                                       /* line 66 */
    name;                                              /* line 67 */
    container;                                         /* line 68 */
    instantiator;                                      /* line 69 *//* line 70 */
} Template;
Template fresh_Template () {
    Template *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 67 */
    self->container =  NULL;                           /* line 68 */
    self->instantiator =  NULL;                        /* line 69 *//* line 70 */
    return self;
}
                                                       /* line 71 */
void mkTemplate (name,template_data,instantiator) {
                                                       /* line 72 */
    templ =  Template ()                               /* line 73 */
    templ.name =  name                                 /* line 74 */
    templ.template_data =  template_data               /* line 75 */
    templ.instantiator =  instantiator                 /* line 76 */
    return ( templ)                                    /* line 77 */;;;/* line 78 *//* line 79 */}

void make_component_registry () {
                                                       /* line 80 */
    return ( Component_Registry ()                     /* line 81 */)/* line 82 *//* line 83 */}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 84 */
/*  and output queues of mevents. */                   /* line 85 */
/*  */                                                 /* line 86 */
/*  Components can either be a user_supplied function (“leaf“), or a “container“ *//* line 87 */
/*  that routes mevents to child components according to a list of connections *//* line 88 */
/*  that serve as a mevent routing table. */           /* line 89 */
/*  */                                                 /* line 90 */
/*  Child components themselves can be leaves or other containers. *//* line 91 */
/*  */                                                 /* line 92 */
/*  `handler` invokes the code that is attached to this component. *//* line 93 */
/*  */                                                 /* line 94 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 95 */
/*  function may want whenever it is invoked again. */ /* line 96 *//* line 97 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 98 *//* line 99 */
/*  Eh_States :: enum { idle, active } */              /* line 100 */
typedef struct _Eh {
                                                       /* line 101 */
    name;                                              /* line 102 */
    inq;
    outq;
    owner;                                             /* line 105 */
    children;                                          /* line 106 */
    visit_ordering;
    connections;                                       /* line 108 */
    routings;
    handler;                                           /* line 110 */
    reset_instance_data;                               /* line 111 */
    finject;                                           /* line 112 */
    stop;                                              /* line 113 */
    instance_data;                                     /* line 114 *//*  arg needed for probe support  *//* line 115 */
    arg;                                               /* line 116 */
    state;                                             /* line 117 */
    special;                                           /* line 118 *//*  bootstrap debugging *//* line 119 */
    kind; /*  enum { container, leaf, } */             /* line 120 *//* line 121 */
} Eh;
Eh fresh_Eh () {
    Eh *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  "";                                  /* line 102 */
    self->inq =  deque ([])                            /* line 103 */;
    self->outq =  deque ([])                           /* line 104 */;
    self->owner =  NULL;                               /* line 105 */
    self->children = [];                               /* line 106 */
    self->visit_ordering =  deque ([])                 /* line 107 */;
    self->connections = [];                            /* line 108 */
    self->routings =  deque ([])                       /* line 109 */;
    self->handler =  NULL;                             /* line 110 */
    self->reset_instance_data =  NULL;                 /* line 111 */
    self->finject =  NULL;                             /* line 112 */
    self->stop =  NULL;                                /* line 113 */
    self->instance_data =  NULL;                       /* line 114 *//*  arg needed for probe support  *//* line 115 */
    self->arg =  "";                                   /* line 116 */
    self->state =  "idle";                             /* line 117 */
    self->special =  False;                            /* line 118 *//*  bootstrap debugging *//* line 119 */
    self->kind =  NULL; /*  enum { container, leaf, } *//* line 120 *//* line 121 */
    return self;
}
                                                       /* line 122 */
int  load_errors =  False                              /* line 123 */;
int  runtime_errors =  False                           /* line 124 */;/* line 125 */
void clone_string (s) {
                                                       /* line 126 */
    return ( s)                                        /* line 127 *//* line 128 *//* line 129 */}

void injector (eh,mevent) {
                                                       /* line 130 */
    eh.handler ( eh, mevent)                           /* line 131 *//* line 132 *//* line 133 */}
