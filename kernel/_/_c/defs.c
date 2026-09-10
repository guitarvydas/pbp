/* line 1 *//* line 2 */
int  counter =  0                                      /* line 3 */;
int  ticktime =  0                                     /* line 4 */;/* line 5 */
int  digits = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"]/* line 12 */;/* line 13 *//* line 14 */
void gensymbol (s) {
                                                       /* line 15 */
    static counter                                     /* line 16 */
    name_with_id =  str( s) + subscripted_digit ( counter) /* line 17 */
    counter =  counter+ 1                              /* line 18 */
    return ( name_with_id)                             /* line 19 */;/* line 20 *//* line 21 */}

void subscripted_digit (n) {
                                                       /* line 22 */
    static digits                                      /* line 23 */
    if ( n >=  0 and  n <=  29):                       /* line 24 */
        return ( digits [ n])                          /* line 25 */
    else:                                              /* line 26 */
        return ( str( "₊") + str ( n)                  /* line 27 */)/* line 28 *//* line 29 *//* line 30 */}

typedef struct _Datum {
                                                       /* line 31 */
    v;                                                 /* line 32 */
    clone;                                             /* line 33 */
    reclaim;                                           /* line 34 */
    other; /*  reserved for use on per-project basis  *//* line 35 *//* line 36 */
} Datum;
Datum fresh_Datum () {
    Datum *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->v =  NULL;                                   /* line 32 */
    self->clone =  NULL;                               /* line 33 */
    self->reclaim =  NULL;                             /* line 34 */
    self->other =  NULL; /*  reserved for use on per-project basis  *//* line 35 *//* line 36 */
    return self;
}
                                                       /* line 37 *//* line 38 */
/*  Mevent passed to a leaf component. */              /* line 39 */
/*  */                                                 /* line 40 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 41 */
/*  `payload` is the data attached to this mevent. */  /* line 42 */
typedef struct _Mevent {
                                                       /* line 43 */
    port;                                              /* line 44 */
    payload;                                           /* line 45 *//* line 46 */
} Mevent;
Mevent fresh_Mevent () {
    Mevent *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->port =  NULL;                                /* line 44 */
    self->payload =  NULL;                             /* line 45 *//* line 46 */
    return self;
}
                                                       /* line 47 */
void clone_port (s) {
                                                       /* line 48 */
    return (clone_string ( s)                          /* line 49 */)/* line 50 *//* line 51 */}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 52 */
/*  entering the very top of a network. */             /* line 53 */
void make_mevent (port,datum) {
                                                       /* line 54 */
    p = clone_string ( port)                           /* line 55 */
    m =  Mevent ()                                     /* line 56 */
    m.port =  p                                        /* line 57 */
    m.payload =  datum.clone ()                        /* line 58 */
    return ( m)                                        /* line 59 */;;/* line 60 *//* line 61 */}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 62 */
void mevent_clone (mev) {
                                                       /* line 63 */
    m =  Mevent ()                                     /* line 64 */
    m.port = clone_port ( mev.port)                    /* line 65 */
    m.payload =  mev.payload.clone ()                  /* line 66 */
    return ( m)                                        /* line 67 */;;/* line 68 *//* line 69 */}

/*  Frees a mevent. */                                 /* line 70 */
void destroy_mevent (mev) {
                                                       /* line 71 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 72 */
                                                       /* line 73 *//* line 74 *//* line 75 */}

void destroy_datum (mev) {
                                                       /* line 76 */
                                                       /* line 77 *//* line 78 *//* line 79 */}

void destroy_port (mev) {
                                                       /* line 80 */
                                                       /* line 81 *//* line 82 *//* line 83 */}

/*  */                                                 /* line 84 */
void format_mevent (m) {
                                                       /* line 85 */
    if  m ==  NULL:                                    /* line 86 */
        return ( "{}")                                 /* line 87 */
    else:                                              /* line 88 */
        return ( str( "{%5C”") +  str( m.port) +  str( "%5C”:%5C”") +  str( m.payload.v) +  "%5C”}"    /* line 89 */)/* line 90 *//* line 91 */}

void format_mevent_raw (m) {
                                                       /* line 92 */
    if  m ==  NULL:                                    /* line 93 */
        return ( "")                                   /* line 94 */
    else:                                              /* line 95 */
        return ( m.payload.v)                          /* line 96 *//* line 97 *//* line 98 *//* line 99 */}
const int
enumDown =  0                                          /* line 100 */;const int
enumAcross =  1                                        /* line 101 */;const int
enumUp =  2                                            /* line 102 */;const int
enumThrough =  3                                       /* line 103 */;/* line 104 *//* line 105 */
/*  Routing connection for a container component. The `direction` field has *//* line 106 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 107 */
/*  purposes, or for reading by other tools. */        /* line 108 *//* line 109 */
typedef struct _Connector {
                                                       /* line 110 */
    direction; /*  down, across, up, through */        /* line 111 */
    sender;                                            /* line 112 */
    receiver;                                          /* line 113 *//* line 114 */
} Connector;
Connector fresh_Connector () {
    Connector *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->direction =  NULL; /*  down, across, up, through *//* line 111 */
    self->sender =  NULL;                              /* line 112 */
    self->receiver =  NULL;                            /* line 113 *//* line 114 */
    return self;
}
                                                       /* line 115 */
/*  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, *//* line 116 */
/*  based on component ID (pointer) and port name. */  /* line 117 *//* line 118 */
typedef struct _Sender {
                                                       /* line 119 */
    name;                                              /* line 120 */
    component;                                         /* line 121 */
    port;                                              /* line 122 *//* line 123 */
} Sender;
Sender fresh_Sender () {
    Sender *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 120 */
    self->component =  NULL;                           /* line 121 */
    self->port =  NULL;                                /* line 122 *//* line 123 */
    return self;
}
                                                       /* line 124 *//* line 125 *//* line 126 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 127 */
/*  to incoming mevents to this queue. */              /* line 128 *//* line 129 */
typedef struct _Receiver {
                                                       /* line 130 */
    name;                                              /* line 131 */
    queue;                                             /* line 132 */
    port;                                              /* line 133 */
    component;                                         /* line 134 *//* line 135 */
} Receiver;
Receiver fresh_Receiver () {
    Receiver *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 131 */
    self->queue =  NULL;                               /* line 132 */
    self->port =  NULL;                                /* line 133 */
    self->component =  NULL;                           /* line 134 *//* line 135 */
    return self;
}
                                                       /* line 136 */
void mkSender (name,component,port) {
                                                       /* line 137 */
    s =  Sender ()                                     /* line 138 */
    s.name =  name                                     /* line 139 */
    s.component =  component                           /* line 140 */
    s.port =  port                                     /* line 141 */
    return ( s)                                        /* line 142 */;;;/* line 143 *//* line 144 */}

void mkReceiver (name,component,port,q) {
                                                       /* line 145 */
    r =  Receiver ()                                   /* line 146 */
    r.name =  name                                     /* line 147 */
    r.component =  component                           /* line 148 */
    r.port =  port                                     /* line 149 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 150 */
    r.queue =  q                                       /* line 151 */
    return ( r)                                        /* line 152 */;;;;/* line 153 *//* line 154 */}
                                                       /* line 155 */
typedef struct _Component_Registry {
                                                       /* line 156 */
    templates;                                         /* line 157 *//* line 158 */
} Component_Registry;
Component_Registry fresh_Component_Registry () {
    Component_Registry *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->templates = {};                              /* line 157 *//* line 158 */
    return self;
}
                                                       /* line 159 */
typedef struct _Template {
                                                       /* line 160 */
    name;                                              /* line 161 */
    container;                                         /* line 162 */
    instantiator;                                      /* line 163 *//* line 164 */
} Template;
Template fresh_Template () {
    Template *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 161 */
    self->container =  NULL;                           /* line 162 */
    self->instantiator =  NULL;                        /* line 163 *//* line 164 */
    return self;
}
                                                       /* line 165 */
void mkTemplate (name,template_data,instantiator) {
                                                       /* line 166 */
    templ =  Template ()                               /* line 167 */
    templ.name =  name                                 /* line 168 */
    templ.template_data =  template_data               /* line 169 */
    templ.instantiator =  instantiator                 /* line 170 */
    return ( templ)                                    /* line 171 */;;;/* line 172 *//* line 173 */}

void make_component_registry () {
                                                       /* line 174 */
    return ( Component_Registry ()                     /* line 175 */)/* line 176 *//* line 177 */}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 178 */
/*  and output queues of mevents. */                   /* line 179 */
/*  */                                                 /* line 180 */
/*  Components can either be a user_supplied function (“leaf“), or a “container“ *//* line 181 */
/*  that routes mevents to child components according to a list of connections *//* line 182 */
/*  that serve as a mevent routing table. */           /* line 183 */
/*  */                                                 /* line 184 */
/*  Child components themselves can be leaves or other containers. *//* line 185 */
/*  */                                                 /* line 186 */
/*  `handler` invokes the code that is attached to this component. *//* line 187 */
/*  */                                                 /* line 188 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 189 */
/*  function may want whenever it is invoked again. */ /* line 190 *//* line 191 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 192 *//* line 193 */
/*  Eh_States :: enum { idle, active } */              /* line 194 */
typedef struct _Eh {
                                                       /* line 195 */
    name;                                              /* line 196 */
    inq;
    outq;
    owner;                                             /* line 199 */
    children;                                          /* line 200 */
    visit_ordering;
    connections;                                       /* line 202 */
    routings;
    handler;                                           /* line 204 */
    reset_instance_data;                               /* line 205 */
    finject;                                           /* line 206 */
    stop;                                              /* line 207 */
    instance_data;                                     /* line 208 *//*  arg needed for probe support  *//* line 209 */
    arg;                                               /* line 210 */
    state;                                             /* line 211 */
    special;                                           /* line 212 *//*  bootstrap debugging *//* line 213 */
    kind; /*  enum { container, leaf, } */             /* line 214 *//* line 215 */
} Eh;
Eh fresh_Eh () {
    Eh *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  "";                                  /* line 196 */
    self->inq =  deque ([])                            /* line 197 */;
    self->outq =  deque ([])                           /* line 198 */;
    self->owner =  NULL;                               /* line 199 */
    self->children = [];                               /* line 200 */
    self->visit_ordering =  deque ([])                 /* line 201 */;
    self->connections = [];                            /* line 202 */
    self->routings =  deque ([])                       /* line 203 */;
    self->handler =  NULL;                             /* line 204 */
    self->reset_instance_data =  NULL;                 /* line 205 */
    self->finject =  NULL;                             /* line 206 */
    self->stop =  NULL;                                /* line 207 */
    self->instance_data =  NULL;                       /* line 208 *//*  arg needed for probe support  *//* line 209 */
    self->arg =  "";                                   /* line 210 */
    self->state =  "idle";                             /* line 211 */
    self->special =  False;                            /* line 212 *//*  bootstrap debugging *//* line 213 */
    self->kind =  NULL; /*  enum { container, leaf, } *//* line 214 *//* line 215 */
    return self;
}
                                                       /* line 216 */
int  load_errors =  False                              /* line 217 */;
int  runtime_errors =  False                           /* line 218 */;/* line 219 */
void clone_string (s) {
                                                       /* line 220 */
    return ( s)                                        /* line 221 *//* line 222 *//* line 223 */}

void injector (eh,mevent) {
                                                       /* line 224 */
    eh.handler ( eh, mevent)                           /* line 225 *//* line 226 *//* line 227 */}
