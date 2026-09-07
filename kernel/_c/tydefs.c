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
    char* v;                                           /* line 32 */
    char* clone;                                       /* line 33 */
    char* reclaim;                                     /* line 34 */
    char* other; /*  reserved for use on per-project basis  *//* line 35 *//* line 36 */
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
    char* port;                                        /* line 44 */
    char* payload;                                     /* line 45 *//* line 46 */
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
typedef struct _Component_Registry {
                                                       /* line 106 */
    char* templates;                                   /* line 107 *//* line 108 */
} Component_Registry;
Component_Registry fresh_Component_Registry () {
    Component_Registry *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->templates = {};                              /* line 107 *//* line 108 */
    return self;
}
                                                       /* line 109 */
/*  Routing connection for a container component. The `direction` field has *//* line 110 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 111 */
/*  purposes, or for reading by other tools. */        /* line 112 *//* line 113 */
typedef struct _Connector {
                                                       /* line 114 */
    char* direction; /*  down, across, up, through */  /* line 115 */
    char* sender;                                      /* line 116 */
    char* receiver;                                    /* line 117 *//* line 118 */
} Connector;
Connector fresh_Connector () {
    Connector *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->direction =  NULL; /*  down, across, up, through *//* line 115 */
    self->sender =  NULL;                              /* line 116 */
    self->receiver =  NULL;                            /* line 117 *//* line 118 */
    return self;
}
                                                       /* line 119 */
/*  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, *//* line 120 */
/*  based on component ID (pointer) and port name. */  /* line 121 *//* line 122 */
typedef struct _Sender {
                                                       /* line 123 */
    char* name;                                        /* line 124 */
    char* component;                                   /* line 125 */
    char* port;                                        /* line 126 *//* line 127 */
} Sender;
Sender fresh_Sender () {
    Sender *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 124 */
    self->component =  NULL;                           /* line 125 */
    self->port =  NULL;                                /* line 126 *//* line 127 */
    return self;
}
                                                       /* line 128 *//* line 129 *//* line 130 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 131 */
/*  to incoming mevents to this queue. */              /* line 132 *//* line 133 */
typedef struct _Receiver {
                                                       /* line 134 */
    char* name;                                        /* line 135 */
    char* queue;                                       /* line 136 */
    char* port;                                        /* line 137 */
    char* component;                                   /* line 138 *//* line 139 */
} Receiver;
Receiver fresh_Receiver () {
    Receiver *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 135 */
    self->queue =  NULL;                               /* line 136 */
    self->port =  NULL;                                /* line 137 */
    self->component =  NULL;                           /* line 138 *//* line 139 */
    return self;
}
                                                       /* line 140 */
void mkSender (name,component,port) {
                                                       /* line 141 */
    s =  Sender ()                                     /* line 142 */
    s.name =  name                                     /* line 143 */
    s.component =  component                           /* line 144 */
    s.port =  port                                     /* line 145 */
    return ( s)                                        /* line 146 */;;;/* line 147 *//* line 148 */}

void mkReceiver (name,component,port,q) {
                                                       /* line 149 */
    r =  Receiver ()                                   /* line 150 */
    r.name =  name                                     /* line 151 */
    r.component =  component                           /* line 152 */
    r.port =  port                                     /* line 153 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 154 */
    r.queue =  q                                       /* line 155 */
    return ( r)                                        /* line 156 */;;;;/* line 157 *//* line 158 */}
                                                       /* line 159 */
typedef struct _Component_Registry {
                                                       /* line 160 */
    char* templates;                                   /* line 161 *//* line 162 */
} Component_Registry;
Component_Registry fresh_Component_Registry () {
    Component_Registry *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->templates = {};                              /* line 161 *//* line 162 */
    return self;
}
                                                       /* line 163 */
typedef struct _Template {
                                                       /* line 164 */
    char* name;                                        /* line 165 */
    char* container;                                   /* line 166 */
    char* instantiator;                                /* line 167 *//* line 168 */
} Template;
Template fresh_Template () {
    Template *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 165 */
    self->container =  NULL;                           /* line 166 */
    self->instantiator =  NULL;                        /* line 167 *//* line 168 */
    return self;
}
                                                       /* line 169 */
void mkTemplate (name,template_data,instantiator) {
                                                       /* line 170 */
    templ =  Template ()                               /* line 171 */
    templ.name =  name                                 /* line 172 */
    templ.template_data =  template_data               /* line 173 */
    templ.instantiator =  instantiator                 /* line 174 */
    return ( templ)                                    /* line 175 */;;;/* line 176 *//* line 177 */}

void make_component_registry () {
                                                       /* line 178 */
    return ( Component_Registry ()                     /* line 179 */)/* line 180 *//* line 181 */}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 182 */
/*  and output queues of mevents. */                   /* line 183 */
/*  */                                                 /* line 184 */
/*  Components can either be a user_supplied function (“leaf“), or a “container“ *//* line 185 */
/*  that routes mevents to child components according to a list of connections *//* line 186 */
/*  that serve as a mevent routing table. */           /* line 187 */
/*  */                                                 /* line 188 */
/*  Child components themselves can be leaves or other containers. *//* line 189 */
/*  */                                                 /* line 190 */
/*  `handler` invokes the code that is attached to this component. *//* line 191 */
/*  */                                                 /* line 192 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 193 */
/*  function may want whenever it is invoked again. */ /* line 194 *//* line 195 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 196 *//* line 197 */
/*  Eh_States :: enum { idle, active } */              /* line 198 */
typedef struct _Eh {
                                                       /* line 199 */
    char* name;                                        /* line 200 */
    char* inq;
    char* outq;
    char* owner;                                       /* line 203 */
    char* children;                                    /* line 204 */
    char* visit_ordering;
    char* connections;                                 /* line 206 */
    char* routings;
    char* handler;                                     /* line 208 */
    char* reset_instance_data;                         /* line 209 */
    char* finject;                                     /* line 210 */
    char* stop;                                        /* line 211 */
    char* instance_data;                               /* line 212 *//*  arg needed for probe support  *//* line 213 */
    char* arg;                                         /* line 214 */
    char* state;                                       /* line 215 */
    char* special;                                     /* line 216 *//*  bootstrap debugging *//* line 217 */
    char* kind; /*  enum { container, leaf, } */       /* line 218 *//* line 219 */
} Eh;
Eh fresh_Eh () {
    Eh *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  "";                                  /* line 200 */
    self->inq =  deque ([])                            /* line 201 */;
    self->outq =  deque ([])                           /* line 202 */;
    self->owner =  NULL;                               /* line 203 */
    self->children = [];                               /* line 204 */
    self->visit_ordering =  deque ([])                 /* line 205 */;
    self->connections = [];                            /* line 206 */
    self->routings =  deque ([])                       /* line 207 */;
    self->handler =  NULL;                             /* line 208 */
    self->reset_instance_data =  NULL;                 /* line 209 */
    self->finject =  NULL;                             /* line 210 */
    self->stop =  NULL;                                /* line 211 */
    self->instance_data =  NULL;                       /* line 212 *//*  arg needed for probe support  *//* line 213 */
    self->arg =  "";                                   /* line 214 */
    self->state =  "idle";                             /* line 215 */
    self->special =  False;                            /* line 216 *//*  bootstrap debugging *//* line 217 */
    self->kind =  NULL; /*  enum { container, leaf, } *//* line 218 *//* line 219 */
    return self;
}
                                                       /* line 220 */
int  load_errors =  False                              /* line 221 */;
int  runtime_errors =  False                           /* line 222 */;/* line 223 */
void clone_string (s) {
                                                       /* line 224 */
    return ( s)                                        /* line 225 *//* line 226 *//* line 227 */}

void injector (eh,mevent) {
                                                       /* line 228 */
    eh.handler ( eh, mevent)                           /* line 229 *//* line 230 *//* line 231 */}
