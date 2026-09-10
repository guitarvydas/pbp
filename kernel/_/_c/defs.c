/* line 1 *//* line 2 */
int  counter =  0                                      /* line 3 */;
int  ticktime =  0                                     /* line 4 */;/* line 5 */
typedef struct _Datum {
                                                       /* line 6 */
    v;                                                 /* line 7 */
    clone;                                             /* line 8 */
    reclaim;                                           /* line 9 */
    other; /*  reserved for use on per-project basis  *//* line 10 *//* line 11 */
} Datum;
Datum fresh_Datum () {
    Datum *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->v =  NULL;                                   /* line 7 */
    self->clone =  NULL;                               /* line 8 */
    self->reclaim =  NULL;                             /* line 9 */
    self->other =  NULL; /*  reserved for use on per-project basis  *//* line 10 *//* line 11 */
    return self;
}
                                                       /* line 12 *//* line 13 */
/*  Mevent passed to a leaf component. */              /* line 14 */
/*  */                                                 /* line 15 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 16 */
/*  `payload` is the data attached to this mevent. */  /* line 17 */
typedef struct _Mevent {
                                                       /* line 18 */
    port;                                              /* line 19 */
    payload;                                           /* line 20 *//* line 21 */
} Mevent;
Mevent fresh_Mevent () {
    Mevent *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->port =  NULL;                                /* line 19 */
    self->payload =  NULL;                             /* line 20 *//* line 21 */
    return self;
}
                                                       /* line 22 */
void clone_port (s) {
                                                       /* line 23 */
    return (clone_string ( s)                          /* line 24 */)/* line 25 *//* line 26 */}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 27 */
/*  entering the very top of a network. */             /* line 28 */
void make_mevent (port,datum) {
                                                       /* line 29 */
    p = clone_string ( port)                           /* line 30 */
    m =  Mevent ()                                     /* line 31 */
    m.port =  p                                        /* line 32 */
    m.payload =  datum.clone ()                        /* line 33 */
    return ( m)                                        /* line 34 */;;/* line 35 *//* line 36 */}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 37 */
void mevent_clone (mev) {
                                                       /* line 38 */
    m =  Mevent ()                                     /* line 39 */
    m.port = clone_port ( mev.port)                    /* line 40 */
    m.payload =  mev.payload.clone ()                  /* line 41 */
    return ( m)                                        /* line 42 */;;/* line 43 *//* line 44 */}

/*  Frees a mevent. */                                 /* line 45 */
void destroy_mevent (mev) {
                                                       /* line 46 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 47 */
                                                       /* line 48 *//* line 49 *//* line 50 */}

void destroy_datum (mev) {
                                                       /* line 51 */
                                                       /* line 52 *//* line 53 *//* line 54 */}

void destroy_port (mev) {
                                                       /* line 55 */
                                                       /* line 56 *//* line 57 *//* line 58 */}

/*  */                                                 /* line 59 */
void format_mevent (m) {
                                                       /* line 60 */
    if  m ==  NULL:                                    /* line 61 */
        return ( "{}")                                 /* line 62 */
    else:                                              /* line 63 */
        return ( str( "{%5C”") +  str( m.port) +  str( "%5C”:%5C”") +  str( m.payload.v) +  "%5C”}"    /* line 64 */)/* line 65 *//* line 66 */}

void format_mevent_raw (m) {
                                                       /* line 67 */
    if  m ==  NULL:                                    /* line 68 */
        return ( "")                                   /* line 69 */
    else:                                              /* line 70 */
        return ( m.payload.v)                          /* line 71 *//* line 72 *//* line 73 *//* line 74 */}
const int
enumDown =  0                                          /* line 75 */;const int
enumAcross =  1                                        /* line 76 */;const int
enumUp =  2                                            /* line 77 */;const int
enumThrough =  3                                       /* line 78 */;/* line 79 *//* line 80 */
/*  Routing connection for a container component. The `direction` field has *//* line 81 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 82 */
/*  purposes, or for reading by other tools. */        /* line 83 *//* line 84 */
typedef struct _Connector {
                                                       /* line 85 */
    direction; /*  down, across, up, through */        /* line 86 */
    sender;                                            /* line 87 */
    receiver;                                          /* line 88 *//* line 89 */
} Connector;
Connector fresh_Connector () {
    Connector *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->direction =  NULL; /*  down, across, up, through *//* line 86 */
    self->sender =  NULL;                              /* line 87 */
    self->receiver =  NULL;                            /* line 88 *//* line 89 */
    return self;
}
                                                       /* line 90 */
/*  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, *//* line 91 */
/*  based on component ID (pointer) and port name. */  /* line 92 *//* line 93 */
typedef struct _Sender {
                                                       /* line 94 */
    name;                                              /* line 95 */
    component;                                         /* line 96 */
    port;                                              /* line 97 *//* line 98 */
} Sender;
Sender fresh_Sender () {
    Sender *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 95 */
    self->component =  NULL;                           /* line 96 */
    self->port =  NULL;                                /* line 97 *//* line 98 */
    return self;
}
                                                       /* line 99 *//* line 100 *//* line 101 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 102 */
/*  to incoming mevents to this queue. */              /* line 103 *//* line 104 */
typedef struct _Receiver {
                                                       /* line 105 */
    name;                                              /* line 106 */
    queue;                                             /* line 107 */
    port;                                              /* line 108 */
    component;                                         /* line 109 *//* line 110 */
} Receiver;
Receiver fresh_Receiver () {
    Receiver *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 106 */
    self->queue =  NULL;                               /* line 107 */
    self->port =  NULL;                                /* line 108 */
    self->component =  NULL;                           /* line 109 *//* line 110 */
    return self;
}
                                                       /* line 111 */
void mkSender (name,component,port) {
                                                       /* line 112 */
    s =  Sender ()                                     /* line 113 */
    s.name =  name                                     /* line 114 */
    s.component =  component                           /* line 115 */
    s.port =  port                                     /* line 116 */
    return ( s)                                        /* line 117 */;;;/* line 118 *//* line 119 */}

void mkReceiver (name,component,port,q) {
                                                       /* line 120 */
    r =  Receiver ()                                   /* line 121 */
    r.name =  name                                     /* line 122 */
    r.component =  component                           /* line 123 */
    r.port =  port                                     /* line 124 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 125 */
    r.queue =  q                                       /* line 126 */
    return ( r)                                        /* line 127 */;;;;/* line 128 *//* line 129 */}
                                                       /* line 130 */
typedef struct _Component_Registry {
                                                       /* line 131 */
    templates;                                         /* line 132 *//* line 133 */
} Component_Registry;
Component_Registry fresh_Component_Registry () {
    Component_Registry *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->templates = {};                              /* line 132 *//* line 133 */
    return self;
}
                                                       /* line 134 */
typedef struct _Template {
                                                       /* line 135 */
    name;                                              /* line 136 */
    container;                                         /* line 137 */
    instantiator;                                      /* line 138 *//* line 139 */
} Template;
Template fresh_Template () {
    Template *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 136 */
    self->container =  NULL;                           /* line 137 */
    self->instantiator =  NULL;                        /* line 138 *//* line 139 */
    return self;
}
                                                       /* line 140 */
void mkTemplate (name,template_data,instantiator) {
                                                       /* line 141 */
    templ =  Template ()                               /* line 142 */
    templ.name =  name                                 /* line 143 */
    templ.template_data =  template_data               /* line 144 */
    templ.instantiator =  instantiator                 /* line 145 */
    return ( templ)                                    /* line 146 */;;;/* line 147 *//* line 148 */}

void make_component_registry () {
                                                       /* line 149 */
    return ( Component_Registry ()                     /* line 150 */)/* line 151 *//* line 152 */}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 153 */
/*  and output queues of mevents. */                   /* line 154 */
/*  */                                                 /* line 155 */
/*  Components can either be a user_supplied function (“leaf“), or a “container“ *//* line 156 */
/*  that routes mevents to child components according to a list of connections *//* line 157 */
/*  that serve as a mevent routing table. */           /* line 158 */
/*  */                                                 /* line 159 */
/*  Child components themselves can be leaves or other containers. *//* line 160 */
/*  */                                                 /* line 161 */
/*  `handler` invokes the code that is attached to this component. *//* line 162 */
/*  */                                                 /* line 163 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 164 */
/*  function may want whenever it is invoked again. */ /* line 165 *//* line 166 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 167 *//* line 168 */
/*  Eh_States :: enum { idle, active } */              /* line 169 */
typedef struct _Eh {
                                                       /* line 170 */
    name;                                              /* line 171 */
    inq;
    outq;
    owner;                                             /* line 174 */
    children;                                          /* line 175 */
    visit_ordering;
    connections;                                       /* line 177 */
    routings;
    handler;                                           /* line 179 */
    reset_instance_data;                               /* line 180 */
    finject;                                           /* line 181 */
    stop;                                              /* line 182 */
    instance_data;                                     /* line 183 *//*  arg needed for probe support  *//* line 184 */
    arg;                                               /* line 185 */
    state;                                             /* line 186 */
    special;                                           /* line 187 *//*  bootstrap debugging *//* line 188 */
    kind; /*  enum { container, leaf, } */             /* line 189 *//* line 190 */
} Eh;
Eh fresh_Eh () {
    Eh *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  "";                                  /* line 171 */
    self->inq =  deque ([])                            /* line 172 */;
    self->outq =  deque ([])                           /* line 173 */;
    self->owner =  NULL;                               /* line 174 */
    self->children = [];                               /* line 175 */
    self->visit_ordering =  deque ([])                 /* line 176 */;
    self->connections = [];                            /* line 177 */
    self->routings =  deque ([])                       /* line 178 */;
    self->handler =  NULL;                             /* line 179 */
    self->reset_instance_data =  NULL;                 /* line 180 */
    self->finject =  NULL;                             /* line 181 */
    self->stop =  NULL;                                /* line 182 */
    self->instance_data =  NULL;                       /* line 183 *//*  arg needed for probe support  *//* line 184 */
    self->arg =  "";                                   /* line 185 */
    self->state =  "idle";                             /* line 186 */
    self->special =  False;                            /* line 187 *//*  bootstrap debugging *//* line 188 */
    self->kind =  NULL; /*  enum { container, leaf, } *//* line 189 *//* line 190 */
    return self;
}
                                                       /* line 191 */
int  load_errors =  False                              /* line 192 */;
int  runtime_errors =  False                           /* line 193 */;/* line 194 */
void clone_string (s) {
                                                       /* line 195 */
    return ( s)                                        /* line 196 *//* line 197 *//* line 198 */}

void injector (eh,mevent) {
                                                       /* line 199 */
    eh.handler ( eh, mevent)                           /* line 200 *//* line 201 *//* line 202 */}
