/* line 1 *//* line 2 */
int  counter =  0                                      /* line 3 */;
int  ticktime =  0                                     /* line 4 */;/* line 5 */
int  digits = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"]/* line 12 */;/* line 13 *//* line 14 */
void gensymbol (s) {
                                                       /* line 15 */
    static counter                                     /* line 16 */
    name_with_id =  str( s) + subscripted_digit ( counter) /* line 17 */
    counter =  counter+ 1                              /* line 18 */
    return ( name_with_id)                             /* line 19 *//* line 20 *//* line 21 */}

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
    self->v =  nil;                                    /* line 32 */
    self->clone =  nil;                                /* line 33 */
    self->reclaim =  nil;                              /* line 34 */
    self->other =  nil; /*  reserved for use on per-project basis  *//* line 35 *//* line 36 */
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
    self->port =  nil;                                 /* line 44 */
    self->payload =  nil;                              /* line 45 *//* line 46 */
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
    return ( m)                                        /* line 59 *//* line 60 *//* line 61 */}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 62 */
void mevent_clone (mev) {
                                                       /* line 63 */
    m =  Mevent ()                                     /* line 64 */
    m.port = clone_port ( mev.port)                    /* line 65 */
    m.payload =  mev.payload.clone ()                  /* line 66 */
    return ( m)                                        /* line 67 *//* line 68 *//* line 69 */}

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
    if  m ==  nil:                                     /* line 86 */
        return ( "{}")                                 /* line 87 */
    else:                                              /* line 88 */
        return ( str( "{%5C”") +  str( m.port) +  str( "%5C”:%5C”") +  str( m.payload.v) +  "%5C”}"    /* line 89 */)/* line 90 *//* line 91 */}

void format_mevent_raw (m) {
                                                       /* line 92 */
    if  m ==  nil:                                     /* line 93 */
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
typedef struct _Template {
                                                       /* line 110 */
    char* name;                                        /* line 111 */
    char* container;                                   /* line 112 */
    char* instantiator;                                /* line 113 *//* line 114 */
} Template;
Template fresh_Template () {
    Template *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  nil;                                 /* line 111 */
    self->container =  nil;                            /* line 112 */
    self->instantiator =  nil;                         /* line 113 *//* line 114 */
    return self;
}
                                                       /* line 115 */
/*  Routing connection for a container component. The `direction` field has *//* line 116 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 117 */
/*  purposes, or for reading by other tools. */        /* line 118 *//* line 119 */
typedef struct _Connector {
                                                       /* line 120 */
    char* direction; /*  down, across, up, through */  /* line 121 */
    char* sender;                                      /* line 122 */
    char* receiver;                                    /* line 123 *//* line 124 */
} Connector;
Connector fresh_Connector () {
    Connector *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->direction =  nil; /*  down, across, up, through *//* line 121 */
    self->sender =  nil;                               /* line 122 */
    self->receiver =  nil;                             /* line 123 *//* line 124 */
    return self;
}
                                                       /* line 125 */
/*  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, *//* line 126 */
/*  based on component ID (pointer) and port name. */  /* line 127 *//* line 128 */
typedef struct _Sender {
                                                       /* line 129 */
    char* name;                                        /* line 130 */
    char* component;                                   /* line 131 */
    char* port;                                        /* line 132 *//* line 133 */
} Sender;
Sender fresh_Sender () {
    Sender *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  nil;                                 /* line 130 */
    self->component =  nil;                            /* line 131 */
    self->port =  nil;                                 /* line 132 *//* line 133 */
    return self;
}
                                                       /* line 134 *//* line 135 *//* line 136 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 137 */
/*  to incoming mevents to this queue. */              /* line 138 *//* line 139 */
typedef struct _Receiver {
                                                       /* line 140 */
    char* name;                                        /* line 141 */
    char* queue;                                       /* line 142 */
    char* port;                                        /* line 143 */
    char* component;                                   /* line 144 *//* line 145 */
} Receiver;
Receiver fresh_Receiver () {
    Receiver *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  nil;                                 /* line 141 */
    self->queue =  nil;                                /* line 142 */
    self->port =  nil;                                 /* line 143 */
    self->component =  nil;                            /* line 144 *//* line 145 */
    return self;
}
                                                       /* line 146 */
void mkSender (name,component,port) {
                                                       /* line 147 */
    s =  Sender ()                                     /* line 148 */
    s.name =  name                                     /* line 149 */
    s.component =  component                           /* line 150 */
    s.port =  port                                     /* line 151 */
    return ( s)                                        /* line 152 *//* line 153 *//* line 154 */}

void mkReceiver (name,component,port,q) {
                                                       /* line 155 */
    r =  Receiver ()                                   /* line 156 */
    r.name =  name                                     /* line 157 */
    r.component =  component                           /* line 158 */
    r.port =  port                                     /* line 159 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 160 */
    r.queue =  q                                       /* line 161 */
    return ( r)                                        /* line 162 *//* line 163 *//* line 164 */}
                                                       /* line 165 */
typedef struct _Component_Registry {
                                                       /* line 166 */
    char* templates;                                   /* line 167 *//* line 168 */
} Component_Registry;
Component_Registry fresh_Component_Registry () {
    Component_Registry *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->templates = {};                              /* line 167 *//* line 168 */
    return self;
}
                                                       /* line 169 */
typedef struct _Template {
                                                       /* line 170 */
    char* name;                                        /* line 171 */
    char* container;                                   /* line 172 */
    char* instantiator;                                /* line 173 *//* line 174 */
} Template;
Template fresh_Template () {
    Template *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  nil;                                 /* line 171 */
    self->container =  nil;                            /* line 172 */
    self->instantiator =  nil;                         /* line 173 *//* line 174 */
    return self;
}
                                                       /* line 175 */
void mkTemplate (name,template_data,instantiator) {
                                                       /* line 176 */
    templ =  Template ()                               /* line 177 */
    templ.name =  name                                 /* line 178 */
    templ.template_data =  template_data               /* line 179 */
    templ.instantiator =  instantiator                 /* line 180 */
    return ( templ)                                    /* line 181 *//* line 182 *//* line 183 */}

void make_component_registry () {
                                                       /* line 184 */
    return ( Component_Registry ()                     /* line 185 */)/* line 186 *//* line 187 */}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 188 */
/*  and output queues of mevents. */                   /* line 189 */
/*  */                                                 /* line 190 */
/*  Components can either be a user_supplied function (“leaf“), or a “container“ *//* line 191 */
/*  that routes mevents to child components according to a list of connections *//* line 192 */
/*  that serve as a mevent routing table. */           /* line 193 */
/*  */                                                 /* line 194 */
/*  Child components themselves can be leaves or other containers. *//* line 195 */
/*  */                                                 /* line 196 */
/*  `handler` invokes the code that is attached to this component. *//* line 197 */
/*  */                                                 /* line 198 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 199 */
/*  function may want whenever it is invoked again. */ /* line 200 *//* line 201 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 202 *//* line 203 */
/*  Eh_States :: enum { idle, active } */              /* line 204 */
typedef struct _Eh {
                                                       /* line 205 */
    char* name;                                        /* line 206 */
    char* inq;
    char* outq;
    char* owner;                                       /* line 209 */
    char* children;                                    /* line 210 */
    char* visit_ordering;
    char* connections;                                 /* line 212 */
    char* routings;
    char* handler;                                     /* line 214 */
    char* reset_instance_data;                         /* line 215 */
    char* finject;                                     /* line 216 */
    char* stop;                                        /* line 217 */
    char* instance_data;                               /* line 218 *//*  arg needed for probe support  *//* line 219 */
    char* arg;                                         /* line 220 */
    char* state;                                       /* line 221 */
    char* special;                                     /* line 222 *//*  bootstrap debugging *//* line 223 */
    char* kind; /*  enum { container, leaf, } */       /* line 224 *//* line 225 */
} Eh;
Eh fresh_Eh () {
    Eh *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  "";                                  /* line 206 */
    self->inq =  deque ([])                            /* line 207 */;
    self->outq =  deque ([])                           /* line 208 */;
    self->owner =  nil;                                /* line 209 */
    self->children = [];                               /* line 210 */
    self->visit_ordering =  deque ([])                 /* line 211 */;
    self->connections = [];                            /* line 212 */
    self->routings =  deque ([])                       /* line 213 */;
    self->handler =  nil;                              /* line 214 */
    self->reset_instance_data =  nil;                  /* line 215 */
    self->finject =  nil;                              /* line 216 */
    self->stop =  nil;                                 /* line 217 */
    self->instance_data =  nil;                        /* line 218 *//*  arg needed for probe support  *//* line 219 */
    self->arg =  "";                                   /* line 220 */
    self->state =  "idle";                             /* line 221 */
    self->special =  False;                            /* line 222 *//*  bootstrap debugging *//* line 223 */
    self->kind =  nil; /*  enum { container, leaf, } *//* line 224 *//* line 225 */
    return self;
}
                                                       /* line 226 */
int  load_errors =  False                              /* line 227 */;
int  runtime_errors =  False                           /* line 228 */;/* line 229 */
void clone_string (s) {
                                                       /* line 230 */
    return ( s)                                        /* line 231 *//* line 232 *//* line 233 */}

void injector (eh,mevent) {
                                                       /* line 234 */
    eh.handler ( eh, mevent)                           /* line 235 *//* line 236 *//* line 237 */}
