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
void mkTemplate (name,template_data,instantiator) {
                                                       /* line 1 */
    templ =  Template ()                               /* line 2 */
    templ.name =  name                                 /* line 3 */
    templ.template_data =  template_data               /* line 4 */
    templ.instantiator =  instantiator                 /* line 5 */
    return ( templ)                                    /* line 6 */;;;/* line 7 *//* line 8 */}
                                                       /* line 9 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 10 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 11 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 12 *//* line 13 */
/*  ... by reading the little-net from an external file  *//* line 14 */
void lnet2internal_from_file (container_xml) {
                                                       /* line 15 */
    pathname = os.getenv('PBPWD', '<none>')            /* line 16 */
    filename =  os.path.basename ( container_xml)      /* line 17 */
    external
    try:
        fil = open(filename, "r")
        json_data = fil.read()
        routings = json.loads(json_data)
        fil.close ()
        return routings
    except FileNotFoundError:
        print (f"File not found: '{filename}'", file=sys.stderr)
        return None
    except json.JSONDecodeError as e:
        print (f"Error decoding JSON in path /{pathname}/: '{e}'", file=sys.stderr)
        return None
                                                       /* line 18 *//* line 19 *//* line 20 */}

/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 21 */
void lnet2internal_from_string (lnet) {
                                                       /* line 22 */
    external
    try:
        routings = json.loads(lnet)
        return routings
    except json.JSONDecodeError as e:
        print ("Error decoding JSON from string 'lnet': '{e}'")
        return None
                                                       /* line 23 *//* line 24 *//* line 25 */}

void delete_decls (d) {
                                                       /* line 26 */
                                                       /* line 27 *//* line 28 *//* line 29 */}

void make_component_registry () {
                                                       /* line 30 */
    return ( Component_Registry ()                     /* line 31 */)/* line 32 *//* line 33 */}

void register_component (reg,template) {

    return (abstracted_register_component ( reg, template, False))/* line 34 */}

void register_component_allow_overwriting (reg,template) {

    return (abstracted_register_component ( reg, template, True))/* line 35 *//* line 36 */}

void abstracted_register_component (reg,template,ok_to_overwrite) {
                                                       /* line 37 */
    name = mangle_name ( template.name)                /* line 38 */
    if  reg!= NULL and  name in  reg.templates and not  ok_to_overwrite:/* line 39 */
        load_error ( str( "Component /") +  str( template.name) +  "/ already declared"  )/* line 40 */
        return ( reg)                                  /* line 41 */
    else:                                              /* line 42 */
        reg.templates [name] =  template               /* line 43 */
        return ( reg)                                  /* line 44 */;/* line 45 *//* line 46 *//* line 47 */}

void get_component_instance (reg,full_name,owner) {
                                                       /* line 48 */
    /*  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  *//* line 49 */
    /*  ":?<string>" is a probe part that is tagged with <string>  *//* line 50 */
    /*  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  *//* line 51 */
    /*  ":<string>" else, it's just treated as a string part that produces <string> on its output  *//* line 52 */
    template_name = mangle_name ( full_name)           /* line 53 */
    if  ":" ==   full_name[0] :                        /* line 54 */
        instance_name = generate_instance_name ( owner, template_name)/* line 55 */
        instance = jit_instantiate ( reg, owner, instance_name, full_name)/* line 56 */
        return ( instance)                             /* line 57 */
    else:                                              /* line 58 */
        if  template_name in  reg.templates:           /* line 59 */
            template =  reg.templates [template_name]  /* line 60 */
            if ( template ==  NULL):                   /* line 61 */
                load_error ( str( "Registry Error (A): Can't find component /") +  str( template_name) +  "/"  )/* line 62 */
                return ( NULL)                         /* line 63 */
            else:                                      /* line 64 */
                instance_name = generate_instance_name ( owner, template_name)/* line 65 */
                instance =  template.instantiator ( reg, owner, instance_name, template.template_data, "")/* line 66 */
                return ( instance)                     /* line 67 *//* line 68 */
        else:                                          /* line 69 */
            load_error ( str( "Registry Error (B): Can't find component /") +  str( template_name) +  "/"  )/* line 70 */
            return ( NULL)                             /* line 71 *//* line 72 *//* line 73 *//* line 74 *//* line 75 */}

void generate_instance_name (owner,template_name) {
                                                       /* line 76 */
    owner_name =  ""                                   /* line 77 */
    instance_name =  template_name                     /* line 78 */
    if  NULL!= owner:                                  /* line 79 */
        owner_name =  owner.name                       /* line 80 */
        instance_name =  str( owner_name) +  str( "▹") +  template_name  /* line 81 */;;
    else:                                              /* line 82 */
        instance_name =  template_name;                /* line 83 *//* line 84 */
    return ( instance_name)                            /* line 85 *//* line 86 *//* line 87 */}

void mangle_name (s) {
                                                       /* line 88 */
    /*  trim name to remove code from Container component names _ deferred until later (or never) *//* line 89 */
    return ( s)                                        /* line 90 *//* line 91 */}
void create_down_connector (container,proto_conn,connectors,children_by_id) {
                                                       /* line 1 */
    /*  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, *//* line 2 */
    connector =  Connector ()                          /* line 3 */
    connector.direction =  "down"                      /* line 4 */
    connector.sender = mkSender ( container.name, container, proto_conn [ "source_port"])/* line 5 */
    target_proto =  proto_conn [ "target"]             /* line 6 */
    id_proto =  target_proto [ "id"]                   /* line 7 */
    target_component =  children_by_id [id_proto]      /* line 8 */
    if ( target_component ==  NULL):                   /* line 9 */
        load_error ( str( "internal error: .Down connection target internal error ") + ( proto_conn [ "target"]) [ "name"] )/* line 10 */
    else:                                              /* line 11 */
        connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn [ "target_port"], target_component.inq)/* line 12 */;/* line 13 */
    return ( connector)                                /* line 14 */;;/* line 15 *//* line 16 */}

void create_across_connector (container,proto_conn,connectors,children_by_id) {
                                                       /* line 17 */
    connector =  Connector ()                          /* line 18 */
    connector.direction =  "across"                    /* line 19 */
    source_component =  children_by_id [(( proto_conn [ "source"]) [ "id"])]/* line 20 */
    target_component =  children_by_id [(( proto_conn [ "target"]) [ "id"])]/* line 21 */
    if  source_component ==  NULL:                     /* line 22 */
        load_error ( str( "internal error: .Across connection source not ok ") + ( proto_conn [ "source"]) [ "name"] )/* line 23 */
    else:                                              /* line 24 */
        connector.sender = mkSender ( source_component.name, source_component, proto_conn [ "source_port"])/* line 25 */
        if  target_component ==  NULL:                 /* line 26 */
            load_error ( str( "internal error: .Across connection target not ok ") + ( proto_conn [ "target"]) [ "name"] )/* line 27 */
        else:                                          /* line 28 */
            connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn [ "target_port"], target_component.inq)/* line 29 */;/* line 30 */;/* line 31 */
    return ( connector)                                /* line 32 */;/* line 33 *//* line 34 */}

void create_up_connector (container,proto_conn,connectors,children_by_id) {
                                                       /* line 35 */
    connector =  Connector ()                          /* line 36 */
    connector.direction =  "up"                        /* line 37 */
    source_component =  children_by_id [(( proto_conn [ "source"]) [ "id"])]/* line 38 */
    if  source_component ==  NULL:                     /* line 39 */
        load_error ( str( "internal error: .Up connection source not ok ") + ( proto_conn [ "source"]) [ "name"] )/* line 40 */
    else:                                              /* line 41 */
        connector.sender = mkSender ( source_component.name, source_component, proto_conn [ "source_port"])/* line 42 */
        connector.receiver = mkReceiver ( container.name, container, proto_conn [ "target_port"], container.outq)/* line 43 */;;/* line 44 */
    return ( connector)                                /* line 45 */;/* line 46 *//* line 47 */}

void create_through_connector (container,proto_conn,connectors,children_by_id) {
                                                       /* line 48 */
    connector =  Connector ()                          /* line 49 */
    connector.direction =  "through"                   /* line 50 */
    connector.sender = mkSender ( container.name, container, proto_conn [ "source_port"])/* line 51 */
    connector.receiver = mkReceiver ( container.name, container, proto_conn [ "target_port"], container.outq)/* line 52 */
    return ( connector)                                /* line 53 */;;;/* line 54 *//* line 55 */}
                                                       /* line 56 */
void container_instantiator (reg,owner,container_name,desc,arg) {
                                                       /* line 57 */
    static enumDown, enumUp, enumAcross, enumThrough   /* line 58 */
    container = make_container ( container_name, owner)/* line 59 */
    children = []                                      /* line 60 */
    children_by_id = {}
    /*  not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here *//* line 61 */
    /*  collect children */                            /* line 62 */
    for child_desc in  desc [ "children"]:             /* line 63 */
        child_instance = get_component_instance ( reg, child_desc [ "name"], container)/* line 64 */
        external   children.append ( child_instance)   /* line 65 */
        id =  child_desc [ "id"]                       /* line 66 */
        children_by_id [id] =  child_instance          /* line 67 *//* line 68 */;/* line 69 */
    container.children =  children                     /* line 70 *//* line 71 */
    connectors = []                                    /* line 72 */
    for proto_conn in  desc [ "connections"]:          /* line 73 */
        connector =  Connector ()                      /* line 74 */
        if  proto_conn [ "dir"] ==  enumDown:          /* line 75 */
            external   connectors.append (create_down_connector ( container, proto_conn, connectors, children_by_id)) /* line 76 */
        elif  proto_conn [ "dir"] ==  enumAcross:      /* line 77 */
            external   connectors.append (create_across_connector ( container, proto_conn, connectors, children_by_id)) /* line 78 */
        elif  proto_conn [ "dir"] ==  enumUp:          /* line 79 */
            external   connectors.append (create_up_connector ( container, proto_conn, connectors, children_by_id)) /* line 80 */
        elif  proto_conn [ "dir"] ==  enumThrough:     /* line 81 */
            external   connectors.append (create_through_connector ( container, proto_conn, connectors, children_by_id)) /* line 82 *//* line 83 *//* line 84 */
    container.connections =  connectors                /* line 85 */
    return ( container)                                /* line 86 */;;/* line 87 *//* line 88 */}

/*  The default handler for container components. */   /* line 89 */
void container_handler (container,mevent) {
                                                       /* line 90 */
    route ( container, container, mevent)
    /*  references to 'self' are replaced by the container during instantiation *//* line 91 */
    while any_child_ready ( container):                /* line 92 */
        step_children ( container, mevent)             /* line 93 *//* line 94 *//* line 95 */}

/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 96 */
void container_reset_children (container) {
                                                       /* line 97 */
    for child in  container.children:                  /* line 98 */
        child.stop ( child)                            /* line 99 *//* line 100 */
    external
    container.visit_ordering.clear ()                  /* line 101 */
    external
    container.routings.clear ()                        /* line 102 */
    external
    container.inq.clear ()                             /* line 103 */
    external
    container.outq.clear ()                            /* line 104 */
    container.state =  "idle";                         /* line 105 *//* line 106 *//* line 107 */}

/*  Frees the given container and associated data. */  /* line 108 */
void destroy_container (eh) {
                                                       /* line 109 */
                                                       /* line 110 *//* line 111 */}

/*  Checks if two senders match, by pointer equality and port name matching. *//* line 112 */
void sender_eq (s1,s2) {
                                                       /* line 113 */
    same_components = ( s1.component ==  s2.component) /* line 114 */
    same_ports = ( s1.port ==  s2.port)                /* line 115 */
    return ( same_components and  same_ports)          /* line 116 *//* line 117 *//* line 118 */}

/*  Delivers the given mevent to the receiver of this connector. *//* line 119 *//* line 120 */
void deposit (parent,conn,mevent) {
                                                       /* line 121 */
    new_mevent = make_mevent ( conn.receiver.port, mevent.payload)/* line 122 */
    push_mevent ( parent, conn.receiver.component, conn.receiver.queue, new_mevent)/* line 123 *//* line 124 *//* line 125 */}

void force_tick (parent,eh) {
                                                       /* line 126 */
    tick_mev = make_mevent ( ".",new_datum_bang ())    /* line 127 */
    push_mevent ( parent, eh, eh.inq, tick_mev)        /* line 128 */
    return ( tick_mev)                                 /* line 129 *//* line 130 *//* line 131 */}

void push_mevent (parent,receiver,inq,m) {
                                                       /* line 132 */
    external  inq.append ( m)                          /* line 133 */
    if ( receiver.special):                            /* line 134 */
        external  parent.visit_ordering.appendleft ( receiver)/* line 135 */
    else:                                              /* line 136 */
        external  parent.visit_ordering.append ( receiver)/* line 137 *//* line 138 *//* line 139 *//* line 140 *//* line 141 */}

void is_self (child,container) {
                                                       /* line 142 */
    /*  in an earlier version “self“ was denoted as ϕ *//* line 143 */
    return ( child ==  container)                      /* line 144 *//* line 145 *//* line 146 */}

void step_child_once (child,mev) {
                                                       /* line 147 */
    if ( ("PBPSTEPPING" in os.environ) ):              /* line 148 */
        external print ( str( "-- stepping ❮") +  str( child.name) +  "❯"  , file=sys.stderr)/* line 149 */
        external                                       /* line 150 *//* line 151 */
    child.handler ( child, mev)                        /* line 152 *//* line 153 *//* line 154 */}

void step_children (container,causingMevent) {
                                                       /* line 155 */
    container.state =  "idle"                          /* line 156 *//* line 157 */
    /*  phase 1 - loop through children and process inputs or children that not "idle"  *//* line 158 */
    for child in  list ( container.visit_ordering):    /* line 159 */
        /*  child = container represents self, skip it *//* line 160 */
        if (not (is_self ( child, container))):        /* line 161 */
            if (not ((0==len( child.inq)))):           /* line 162 */
                mev =  child.inq.popleft ()            /* line 163 */
                step_child_once ( child, mev)          /* line 164 *//* line 165 */
                destroy_mevent ( mev)                  /* line 166 */
            else:                                      /* line 167 */
                if  child.state ==  "idle":            /* line 168 */
                                                       /* line 169 */
                else:                                  /* line 170 */
                    mev = force_tick ( container, child)/* line 171 */
                    step_child_once ( child, mev)      /* line 172 */
                    destroy_mevent ( mev)              /* line 173 *//* line 174 *//* line 175 *//* line 176 *//* line 177 */
    external
    container.visit_ordering.clear ()                  /* line 178 *//* line 179 */
    /*  phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections  *//* line 180 */
    for child in  container.children:                  /* line 181 */
        if  child.state ==  "active":                  /* line 182 */
            /*  if child remains active, then the container must remain active and must propagate “ticks“ to child *//* line 183 */
            container.state =  "active";               /* line 184 *//* line 185 *//* line 186 */
        while (not ((0==len( child.outq)))):           /* line 187 */
            mev =  child.outq.popleft ()               /* line 188 */
            route ( container, child, mev)             /* line 189 */
            destroy_mevent ( mev)                      /* line 190 *//* line 191 *//* line 192 */;/* line 193 *//* line 194 */}

void attempt_tick (parent,eh) {
                                                       /* line 195 */
    if  eh.state!= "idle":                             /* line 196 */
        force_tick ( parent, eh)                       /* line 197 *//* line 198 *//* line 199 *//* line 200 */}

void is_tick (mev) {
                                                       /* line 201 */
    return ( "." ==  mev.port)
    /*  assume that any mevent that is sent to port "." is a tick  *//* line 202 *//* line 203 *//* line 204 */}

/*  Routes a single mevent to all matching destinations, according to *//* line 205 */
/*  the container's connection network. */             /* line 206 *//* line 207 */
void route (container,from_component,mevent) {
                                                       /* line 208 */
    was_sent =  False
    /*  for checking that output went somewhere (at least during bootstrap) *//* line 209 */
    fromname =  ""                                     /* line 210 */
    static ticktime                                    /* line 211 */
    ticktime =  ticktime+ 1                            /* line 212 */
    if is_tick ( mevent):                              /* line 213 */
        for child in  container.children:              /* line 214 */
            attempt_tick ( container, child)           /* line 215 */
        was_sent =  True;                              /* line 216 */
    else:                                              /* line 217 */
        if (not (is_self ( from_component, container))):/* line 218 */
            fromname =  from_component.name;           /* line 219 *//* line 220 */
        from_sender = mkSender ( fromname, from_component, mevent.port)/* line 221 *//* line 222 */
        for connector in  container.connections:       /* line 223 */
            if sender_eq ( from_sender, connector.sender):/* line 224 */
                deposit ( container, connector, mevent)/* line 225 */
                was_sent =  True;                      /* line 226 *//* line 227 *//* line 228 *//* line 229 */
    if not ( was_sent):                                /* line 230 */
        external live_update ( "internal error",  str( container.name) +  str( ": mevent on port '") +  str( mevent.port) +  str( "' from ") +  str( fromname) +  " dropped on floor..."     )/* line 231 *//* line 232 */;/* line 233 *//* line 234 */}

void any_child_ready (container) {
                                                       /* line 235 */
    for child in  container.children:                  /* line 236 */
        if child_is_ready ( child):                    /* line 237 */
            return ( True)                             /* line 238 *//* line 239 *//* line 240 */
    return ( False)                                    /* line 241 *//* line 242 *//* line 243 */}

void child_is_ready (eh) {
                                                       /* line 244 */
    return ((not ((0==len( eh.outq)))) or (not ((0==len( eh.inq)))) or ( eh.state!= "idle") or (any_child_ready ( eh)))/* line 245 *//* line 246 *//* line 247 */}

void append_routing_descriptor (container,desc) {
                                                       /* line 248 */
    external  container.routings.append ( desc)        /* line 249 *//* line 250 *//* line 251 */}
                                                       /* line 252 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 253 */
/*  whose handler function is `container_handler`. */  /* line 254 */
void make_container (name,owner) {
                                                       /* line 255 */
    eh =  Eh ()                                        /* line 256 */
    eh.name =  name                                    /* line 257 */
    eh.owner =  owner                                  /* line 258 */
    eh.handler =  container_handler                    /* line 259 */
    eh.finject =  injector                             /* line 260 */
    eh.stop =  container_reset_children                /* line 261 */
    eh.state =  "idle"                                 /* line 262 */
    eh.kind =  "container"                             /* line 263 */
    return ( eh)                                       /* line 264 */;;;;;;;/* line 265 *//* line 266 */}

/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 267 */
/*  of the given component. */                         /* line 268 *//* line 269 */
void send (eh,port,obj,causingMevent) {
                                                       /* line 270 */
    d =  Datum ()                                      /* line 271 */
    d.v =  obj                                         /* line 272 */
    d.clone =  lambda : obj_clone ( d)                 /* line 273 */
    d.reclaim =  NULL                                  /* line 274 */
    mev = make_mevent ( port, d)                       /* line 275 */
    put_output ( eh, mev)                              /* line 276 */;;;/* line 277 *//* line 278 */}

void forward (eh,port,mev) {
                                                       /* line 279 */
    fwdmev = make_mevent ( port, mev.payload)          /* line 280 */
    put_output ( eh, fwdmev)                           /* line 281 *//* line 282 *//* line 283 */}

void inject_mevent (eh,mev) {
                                                       /* line 284 */
    eh.finject ( eh, mev)                              /* line 285 *//* line 286 *//* line 287 */}

void set_active (eh) {
                                                       /* line 288 */
    eh.state =  "active";                              /* line 289 *//* line 290 *//* line 291 */}

void set_idle (eh) {
                                                       /* line 292 */
    eh.state =  "idle";                                /* line 293 *//* line 294 *//* line 295 */}

void put_output (eh,mev) {
                                                       /* line 296 */
    external  eh.outq.append ( mev)                    /* line 297 *//* line 298 *//* line 299 */}

void obj_clone (obj) {
                                                       /* line 300 */
    return ( obj)                                      /* line 301 *//* line 302 */}
/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
void make_leaf (name,owner,instance_data,arg,handler,reset_handler) {
                                                       /* line 4 */
    eh =  Eh ()                                        /* line 5 */
    nm =  ""                                           /* line 6 */
    if  NULL!= owner:                                  /* line 7 */
        nm =  owner.name;                              /* line 8 *//* line 9 */
    eh.name =  str( nm) +  str( "▹") +  name           /* line 10 */
    eh.owner =  owner                                  /* line 11 */
    eh.handler =  handler                              /* line 12 */
    eh.reset_handler =  reset_handler                  /* line 13 */
    eh.finject =  injector                             /* line 14 */
    eh.stop =  leaf_reset                              /* line 15 */
    eh.instance_data =  instance_data                  /* line 16 */
    eh.arg =  arg                                      /* line 17 */
    eh.state =  "idle"                                 /* line 18 */
    eh.kind =  "leaf"                                  /* line 19 */
    return ( eh)                                       /* line 20 */;;;;;;;;;;/* line 21 *//* line 22 */}

/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 23 */
void leaf_reset (part) {
                                                       /* line 24 */
    external
    part.inq.clear ()                                  /* line 25 */
    external
    part.outq.clear ()                                 /* line 26 */
    if ( part.reset_handler!= NULL):                   /* line 27 */
        part.reset_handler ( part)                     /* line 28 *//* line 29 */
    part.state =  "idle";                              /* line 30 *//* line 31 */}
/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
void jit_instantiate (reg,owner,name,arg) {
                                                       /* line 3 */
    name_with_id = gensymbol ( name)                   /* line 4 */
    inst = make_leaf ( name_with_id, owner, NULL, arg, handle_jit, NULL)/* line 5 */
    firstc =  name [ 1]                                /* line 6 */
    if ( firstc!= "$"):                                /* line 7 */
        /*  probes get to go to the front of the line  *//* line 8 */
        inst.special =  True;                          /* line 9 *//* line 10 */
    return ( inst)                                     /* line 11 *//* line 12 *//* line 13 */}

void handle_jit (eh,mev) {
                                                       /* line 14 */
    s =  eh.arg                                        /* line 15 */
    firstc =  s [ 1]                                   /* line 16 */
    if  firstc ==  "$":                                /* line 17 */
        shell_out_handler ( eh,    s[1:] [1:] [1:] , mev)/* line 18 */
    elif  firstc ==  "?":                              /* line 19 */
        probe_handler ( eh,  s[1:] , mev)              /* line 20 */
    else:                                              /* line 21 */
        /*  just a string, send it out  */             /* line 22 */
        send ( eh, "",  s[1:] , mev)                   /* line 23 *//* line 24 *//* line 25 *//* line 26 */}

void probe_handler (eh,tag,mev) {
                                                       /* line 27 */
    s =  mev.payload.v                                 /* line 28 */
    external live_update ( "Info",  str( "  @") +  str(str ( ticktime)) +  str( "  ") +  str( "probe ") +  str( eh.name) +  str( ": ") + str ( s)      )/* line 36 *//* line 37 *//* line 38 */}

void shell_out_handler (eh,cmd,mev) {
                                                       /* line 39 */
    s =  mev.payload.v                                 /* line 40 */
    ret =  NULL                                        /* line 41 */
    rc =  NULL                                         /* line 42 */
    stdout =  NULL                                     /* line 43 */
    stderr =  NULL                                     /* line 44 */
    command =  cmd                                     /* line 45 */
    pbpRoot = os.getenv('PBP', '<none>')               /* line 46 */
    if  pbpRoot!= "":                                  /* line 47 */
        command = re.sub ( "_/",  str( pbpRoot) +  "/" ,  command)/* line 50 */;/* line 51 */
    if ( ("PBPSHELLUT" in os.environ) ):               /* line 52 */
        external print ( str( "- --- shell-out: ") +  command , file=sys.stderr)/* line 53 */
        external                                       /* line 54 *//* line 55 */
    external
    try:
        with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', delete=False) as tmp:
            tmp.write( s)
            tmp_path = tmp.name
        try:
            with open(tmp_path, 'r') as stdin_file:
                ret = subprocess.run(
                shlex.split( command),
                stdin=stdin_file,
                text=True,
                capture_output=True
                )
        finally:
            os.unlink(tmp_path)
        rc = ret.returncode
        stdout = ret.stdout.strip()
        stderr = ret.stderr.strip()
    except Exception as e:
        rc = 1
        stdout = ''
        stderr = str(e)
                                                       /* line 56 */
    if  rc ==  0:                                      /* line 57 */
        send ( eh, "", str( stdout) +  stderr , mev)   /* line 58 */
    else:                                              /* line 59 */
        send ( eh, "✗", str( stdout) +  stderr , mev)  /* line 60 *//* line 61 *//* line 62 *//* line 63 */}
/* line 1 */
void trash_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 2 */
    name_with_id = gensymbol ( "trash")                /* line 3 */
    return (make_leaf ( name_with_id, owner, NULL, "", trash_handler, NULL)/* line 4 */)/* line 5 *//* line 6 */}

void trash_handler (eh,mev) {
                                                       /* line 7 */
    /*  to appease dumped_on_floor checker */          /* line 8 */
                                                       /* line 9 *//* line 10 */}

typedef struct _TwoMevents {
                                                       /* line 11 */
    char* firstmev;                                    /* line 12 */
    char* secondmev;                                   /* line 13 *//* line 14 */
} TwoMevents;
TwoMevents fresh_TwoMevents () {
    TwoMevents *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->firstmev =  NULL;                            /* line 12 */
    self->secondmev =  NULL;                           /* line 13 *//* line 14 */
    return self;
}
                                                       /* line 15 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 16 */
typedef struct _Deracer_Instance_Data {
                                                       /* line 17 */
    char* state;                                       /* line 18 */
    char* buffer;                                      /* line 19 *//* line 20 */
} Deracer_Instance_Data;
Deracer_Instance_Data fresh_Deracer_Instance_Data () {
    Deracer_Instance_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->state =  NULL;                               /* line 18 */
    self->buffer =  NULL;                              /* line 19 *//* line 20 */
    return self;
}
                                                       /* line 21 */
void reclaim_Buffers_from_heap (inst) {
                                                       /* line 22 */
                                                       /* line 23 *//* line 24 *//* line 25 */}

void deracer_reset_handler (eh) {
                                                       /* line 26 */
    inst =  eh.instance_data                           /* line 27 */
    inst.state =  "idle"                               /* line 28 */
    inst.buffer =  TwoMevents ()                       /* line 29 */;;/* line 30 *//* line 31 */}

void deracer_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 32 */
    name_with_id = gensymbol ( "deracer")              /* line 33 */
    inst =  Deracer_Instance_Data ()                   /* line 34 */
    inst.state =  "idle"                               /* line 35 */
    inst.buffer =  TwoMevents ()                       /* line 36 */
    eh = make_leaf ( name_with_id, owner, inst, "", deracer_handler, deracer_reset_handler)/* line 37 */
    return ( eh)                                       /* line 38 */;;/* line 39 *//* line 40 */}

void send_firstmev_then_secondmev (eh,inst) {
                                                       /* line 41 */
    forward ( eh, "1", inst.buffer.firstmev)           /* line 42 */
    forward ( eh, "2", inst.buffer.secondmev)          /* line 43 */
    reclaim_Buffers_from_heap ( inst)                  /* line 44 *//* line 45 *//* line 46 */}

void deracer_handler (eh,mev) {
                                                       /* line 47 */
    inst =  eh.instance_data                           /* line 48 */
    if  inst.state ==  "idle":                         /* line 49 */
        if  "1" ==  mev.port:                          /* line 50 */
            inst.buffer.firstmev =  mev                /* line 51 */
            inst.state =  "waitingForSecondmev";       /* line 52 */;
        elif  "2" ==  mev.port:                        /* line 53 */
            inst.buffer.secondmev =  mev               /* line 54 */
            inst.state =  "waitingForFirstmev";        /* line 55 */;
        else:                                          /* line 56 */
            runtime_error ( str( "bad mev.port (case A) for deracer ") +  mev.port )/* line 57 *//* line 58 */
    elif  inst.state ==  "waitingForFirstmev":         /* line 59 */
        if  "1" ==  mev.port:                          /* line 60 */
            inst.buffer.firstmev =  mev                /* line 61 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 62 */
            inst.state =  "idle";                      /* line 63 */;
        else:                                          /* line 64 */
            runtime_error ( str( "deracer: waiting for 1 but got [") +  str( mev.port) +  "] (case B)"  )/* line 65 *//* line 66 */
    elif  inst.state ==  "waitingForSecondmev":        /* line 67 */
        if  "2" ==  mev.port:                          /* line 68 */
            inst.buffer.secondmev =  mev               /* line 69 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 70 */
            inst.state =  "idle";                      /* line 71 */;
        else:                                          /* line 72 */
            runtime_error ( str( "deracer: waiting for 2 but got [") +  str( mev.port) +  "] (case C)"  )/* line 73 *//* line 74 */
    else:                                              /* line 75 */
        runtime_error ( "bad state for deracer {eh.state}")/* line 76 *//* line 77 *//* line 78 *//* line 79 */}

void low_level_read_text_file_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 80 */
    name_with_id = gensymbol ( "Low Level Read Text File")/* line 81 */
    return (make_leaf ( name_with_id, owner, NULL, "", low_level_read_text_file_handler, NULL)/* line 82 */)/* line 83 *//* line 84 */}

void low_level_read_text_file_handler (eh,mev) {
                                                       /* line 85 */
    fname =  mev.payload.v                             /* line 86 */
    external
    try:
        f = open (fname)
    except Exception as e:
        f = None
    if f != None:
        data = f.read ()
        if data!= None:
            send (eh, "", data, mev)
        else:
            send (eh, "✗", f"read error on file '{fname}'", mev)
        f.close ()
    else:
        send (eh, "✗", f"open error on file '{fname}'", mev)
                                                       /* line 87 *//* line 88 *//* line 89 */}

void ensure_string_datum_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 90 */
    name_with_id = gensymbol ( "Ensure String Datum")  /* line 91 */
    return (make_leaf ( name_with_id, owner, NULL, "", ensure_string_datum_handler, NULL)/* line 92 */)/* line 93 *//* line 94 */}

void ensure_string_datum_handler (eh,mev) {
                                                       /* line 95 */
    if  "string" ==  mev.payload.kind ():              /* line 96 */
        forward ( eh, "", mev)                         /* line 97 */
    else:                                              /* line 98 */
        emev =  str( "*** ensure: type error (expected a string payload) but got ") +  mev.payload /* line 99 */
        send ( eh, "✗", emev, mev)                     /* line 100 *//* line 101 *//* line 102 *//* line 103 */}

typedef struct _Syncfilewrite_Data {
                                                       /* line 104 */
    char* filename;                                    /* line 105 *//* line 106 */
} Syncfilewrite_Data;
Syncfilewrite_Data fresh_Syncfilewrite_Data () {
    Syncfilewrite_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->filename =  "";                              /* line 105 *//* line 106 */
    return self;
}
                                                       /* line 107 */
void syncfilewrite_reset_handler (eh) {
                                                       /* line 108 */
    eh.instance_data =  Syncfilewrite_Data ()          /* line 109 */;/* line 110 *//* line 111 */}

/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 112 */
void syncfilewrite_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 113 */
    name_with_id = gensymbol ( "syncfilewrite")        /* line 114 */
    inst =  Syncfilewrite_Data ()                      /* line 115 */
    return (make_leaf ( name_with_id, owner, inst, "", syncfilewrite_handler, syncfilewrite_reset_handler)/* line 116 */)/* line 117 *//* line 118 */}

void syncfilewrite_handler (eh,mev) {
                                                       /* line 119 */
    inst =  eh.instance_data                           /* line 120 */
    if  "filename" ==  mev.port:                       /* line 121 */
        inst.filename =  mev.payload.v;                /* line 122 */
    elif  "input" ==  mev.port:                        /* line 123 */
        contents =  mev.payload.v                      /* line 124 */
        f = open ( inst.filename, "w")                 /* line 125 */
        if  f!= NULL:                                  /* line 126 */
            f.write ( mev.payload.v)                   /* line 127 */
            f.close ()                                 /* line 128 */
            send ( eh, "done",new_datum_bang (), mev)  /* line 129 */
        else:                                          /* line 130 */
            send ( eh, "✗", str( "open error on file ") +  inst.filename , mev)/* line 131 *//* line 132 *//* line 133 *//* line 134 *//* line 135 */}

typedef struct _StringConcat_Instance_Data {
                                                       /* line 136 */
    char* buffer1;                                     /* line 137 */
    char* buffer2;                                     /* line 138 *//* line 139 */
} StringConcat_Instance_Data;
StringConcat_Instance_Data fresh_StringConcat_Instance_Data () {
    StringConcat_Instance_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->buffer1 =  NULL;                             /* line 137 */
    self->buffer2 =  NULL;                             /* line 138 *//* line 139 */
    return self;
}
                                                       /* line 140 */
void stringconcat_reset_handler (eh) {
                                                       /* line 141 */
    inst =  eh.instance_data                           /* line 142 */
    inst.buffer1 =  NULL                               /* line 143 */
    inst.buffer2 =  NULL;                              /* line 144 */;/* line 145 *//* line 146 */}

void stringconcat_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 147 */
    name_with_id = gensymbol ( "stringconcat")         /* line 148 */
    instp =  StringConcat_Instance_Data ()             /* line 149 */
    return (make_leaf ( name_with_id, owner, instp, "", stringconcat_handler, stringconcat_reset_handler)/* line 150 */)/* line 151 *//* line 152 */}

void stringconcat_handler (eh,mev) {
                                                       /* line 153 */
    inst =  eh.instance_data                           /* line 154 */
    if  "1" ==  mev.port:                              /* line 155 */
        inst.buffer1 = clone_string ( mev.payload.v)   /* line 156 */
        maybe_stringconcat ( eh, inst, mev)            /* line 157 */;
    elif  "2" ==  mev.port:                            /* line 158 */
        inst.buffer2 = clone_string ( mev.payload.v)   /* line 159 */
        maybe_stringconcat ( eh, inst, mev)            /* line 160 */;
    elif  "reset" ==  mev.port:                        /* line 161 */
        inst.buffer1 =  NULL                           /* line 162 */
        inst.buffer2 =  NULL;                          /* line 163 */;
    else:                                              /* line 164 */
        runtime_error ( str( "bad mev.port for stringconcat: ") +  mev.port )/* line 165 *//* line 166 *//* line 167 *//* line 168 */}

void maybe_stringconcat (eh,inst,mev) {
                                                       /* line 169 */
    if  inst.buffer1!= NULL and  inst.buffer2!= NULL:  /* line 170 */
        concatenated_string =  ""                      /* line 171 */
        if  0 == len ( inst.buffer1):                  /* line 172 */
            concatenated_string =  inst.buffer2;       /* line 173 */
        elif  0 == len ( inst.buffer2):                /* line 174 */
            concatenated_string =  inst.buffer1;       /* line 175 */
        else:                                          /* line 176 */
            concatenated_string =  inst.buffer1+ inst.buffer2;/* line 177 *//* line 178 */
        send ( eh, "", concatenated_string, mev)       /* line 179 */
        inst.buffer1 =  NULL                           /* line 180 */
        inst.buffer2 =  NULL;                          /* line 181 */;/* line 182 *//* line 183 *//* line 184 */}

/*  */                                                 /* line 185 *//* line 186 */
void string_constant_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 187 */
    static projectRoot                                 /* line 188 */
    name_with_id = gensymbol ( "strconst")             /* line 189 */
    s =  template_data                                 /* line 190 */
    if  projectRoot!= "":                              /* line 191 */
        s = re.sub ( "_00_",  projectRoot,  s)         /* line 192 */;/* line 193 */
    return (make_leaf ( name_with_id, owner, s, "", string_constant_handler, NULL)/* line 194 */)/* line 195 *//* line 196 */}

void string_constant_handler (eh,mev) {
                                                       /* line 197 */
    s =  eh.instance_data                              /* line 198 */
    send ( eh, "", s, mev)                             /* line 199 *//* line 200 *//* line 201 */}

void fakepipename_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 202 */
    instance_name = gensymbol ( "fakepipe")            /* line 203 */
    return (make_leaf ( instance_name, owner, NULL, "", fakepipename_handler, NULL)/* line 204 */)/* line 205 *//* line 206 */}

int  rand =  0                                         /* line 207 */;/* line 208 */
void fakepipename_handler (eh,mev) {
                                                       /* line 209 */
    static rand                                        /* line 210 */
    rand =  rand+ 1
    /*  not very random, but good enough _ ;rand' must be unique within a single run *//* line 211 */
    send ( eh, "", str( "/tmp/fakepipe") +  rand , mev)/* line 212 */;/* line 213 *//* line 214 */}
                                                       /* line 215 */
typedef struct _Switch1star_Instance_Data {
                                                       /* line 216 */
    char* state;                                       /* line 217 *//* line 218 */
} Switch1star_Instance_Data;
Switch1star_Instance_Data fresh_Switch1star_Instance_Data () {
    Switch1star_Instance_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->state =  "1";                                /* line 217 *//* line 218 */
    return self;
}
                                                       /* line 219 */
void switch1star_reset_handler (eh) {
                                                       /* line 220 */
    inst =  eh.instance_data                           /* line 221 */
    inst =  Switch1star_Instance_Data ()               /* line 222 */;/* line 223 *//* line 224 */}

void switch1star_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 225 */
    name_with_id = gensymbol ( "switch1*")             /* line 226 */
    instp =  Switch1star_Instance_Data ()              /* line 227 */
    return (make_leaf ( name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler)/* line 228 */)/* line 229 *//* line 230 */}

void switch1star_handler (eh,mev) {
                                                       /* line 231 */
    inst =  eh.instance_data                           /* line 232 */
    whichOutput =  inst.state                          /* line 233 */
    if  "" ==  mev.port:                               /* line 234 */
        if  "1" ==  whichOutput:                       /* line 235 */
            forward ( eh, "1", mev)                    /* line 236 */
            inst.state =  "*";                         /* line 237 */
        elif  "*" ==  whichOutput:                     /* line 238 */
            forward ( eh, "*", mev)                    /* line 239 */
        else:                                          /* line 240 */
            send ( eh, "✗", "internal error bad state in switch1*", mev)/* line 241 *//* line 242 */
    elif  "reset" ==  mev.port:                        /* line 243 */
        inst.state =  "1";                             /* line 244 */
    else:                                              /* line 245 */
        send ( eh, "✗", "internal error bad mevent for switch1*", mev)/* line 246 *//* line 247 *//* line 248 *//* line 249 */}

typedef struct _StringAccumulator {
                                                       /* line 250 */
    char* s;                                           /* line 251 *//* line 252 */
} StringAccumulator;
StringAccumulator fresh_StringAccumulator () {
    StringAccumulator *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->s =  "";                                     /* line 251 *//* line 252 */
    return self;
}
                                                       /* line 253 */
void strcatstar_reset_handler (eh) {
                                                       /* line 254 */
    eh.instance_data =  StringAccumulator ()           /* line 255 */;/* line 256 *//* line 257 */}

void strcatstar_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 258 */
    name_with_id = gensymbol ( "String Concat *")      /* line 259 */
    instp =  StringAccumulator ()                      /* line 260 */
    return (make_leaf ( name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler)/* line 261 */)/* line 262 *//* line 263 */}

void strcatstar_handler (eh,mev) {
                                                       /* line 264 */
    accum =  eh.instance_data                          /* line 265 */
    if  "" ==  mev.port:                               /* line 266 */
        accum.s =  str( accum.s) +  mev.payload.v      /* line 267 */;
    elif  "fini" ==  mev.port:                         /* line 268 */
        send ( eh, "", accum.s, mev)                   /* line 269 */
    else:                                              /* line 270 */
        send ( eh, "✗", "internal error bad mevent for String Concat *", mev)/* line 271 *//* line 272 *//* line 273 *//* line 274 */}

void stop_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 275 */
    name_with_id = gensymbol ( "Stop")                 /* line 276 */
    inst =  NULL                                       /* line 277 */
    return (make_leaf ( name_with_id, owner, inst, "", stop_handler, NULL)/* line 278 */)/* line 279 *//* line 280 */}

void stop_handler (eh,mev) {
                                                       /* line 281 */
    inst =  eh.instance_data                           /* line 282 */
    parent =  eh.owner                                 /* line 283 */
    s =  str( "   !!! stopping: '") +  str( parent.name) +  "'"  /* line 284 */
    external print ( s, file=sys.stderr)               /* line 285 */
    external                                           /* line 286 */
    parent.stop ( parent)                              /* line 287 */
    send ( eh, "", mev.payload.v, mev)                 /* line 288 *//* line 289 *//* line 290 */}

/*  all of the the built_in leaves are listed here */  /* line 291 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 292 *//* line 293 */
void initialize_stock_components (reg) {
                                                       /* line 294 */
    register_component ( reg,mkTemplate ( "1then2", NULL, deracer_instantiate))/* line 295 */
    register_component ( reg,mkTemplate ( "1→2", NULL, deracer_instantiate))/* line 296 */
    register_component ( reg,mkTemplate ( "trash", NULL, trash_instantiate))/* line 297 */
    register_component ( reg,mkTemplate ( "🗑️", NULL, trash_instantiate))/* line 298 */
    register_component ( reg,mkTemplate ( "🚫", NULL, stop_instantiate))/* line 299 *//* line 300 *//* line 301 */
    register_component ( reg,mkTemplate ( "Read Text File", NULL, low_level_read_text_file_instantiate))/* line 302 */
    register_component ( reg,mkTemplate ( "Ensure String Datum", NULL, ensure_string_datum_instantiate))/* line 303 *//* line 304 */
    register_component ( reg,mkTemplate ( "syncfilewrite", NULL, syncfilewrite_instantiate))/* line 305 */
    register_component ( reg,mkTemplate ( "String Concat", NULL, stringconcat_instantiate))/* line 306 */
    register_component ( reg,mkTemplate ( "switch1*", NULL, switch1star_instantiate))/* line 307 */
    register_component ( reg,mkTemplate ( "String Concat *", NULL, strcatstar_instantiate))/* line 308 */
    /*  for fakepipe */                                /* line 309 */
    register_component ( reg,mkTemplate ( "fakepipename", NULL, fakepipename_instantiate))/* line 310 *//* line 311 *//* line 312 */}
/* line 1 */
void load_error (s) {
                                                       /* line 2 */
    static load_errors                                 /* line 3 */
    external print ( s, file=sys.stderr)               /* line 4 */
    external                                           /* line 5 */
    load_errors =  True;                               /* line 6 *//* line 7 *//* line 8 */}

void runtime_error (s) {
                                                       /* line 9 */
    static runtime_errors                              /* line 10 */
    external print ( s, file=sys.stderr)               /* line 11 */
    external exit (1)                                  /* line 12 */
    runtime_errors =  True;                            /* line 13 *//* line 14 *//* line 15 */}
                                                       /* line 16 */
void initialize_component_palette_from_files (diagram_source_files) {
                                                       /* line 17 */
    reg = make_component_registry ()                   /* line 18 */
    for diagram_source in  diagram_source_files:       /* line 19 */
        all_containers_within_single_file = lnet2internal_from_file ( diagram_source)/* line 20 */
        for container in  all_containers_within_single_file:/* line 21 */
            register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 22 *//* line 23 *//* line 24 */
    initialize_stock_components ( reg)                 /* line 25 */
    return ( reg)                                      /* line 26 *//* line 27 *//* line 28 */}

void initialize_component_palette_from_string (lnet) {
                                                       /* line 29 */
    reg = make_component_registry ()                   /* line 30 */
    all_containers = lnet2internal_from_string ( lnet) /* line 31 */
    for container in  all_containers:                  /* line 32 */
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 33 *//* line 34 */
    initialize_stock_components ( reg)                 /* line 35 */
    return ( reg)                                      /* line 36 *//* line 37 */}

void initialize_from_files (diagram_names) {
                                                       /* line 38 */
    arg =  NULL                                        /* line 39 */
    palette = initialize_component_palette_from_files ( diagram_names)/* line 40 */
    return [ palette,[ diagram_names, arg]]            /* line 41 *//* line 42 *//* line 43 */}

void initialize_from_string () {
                                                       /* line 44 */
    arg =  NULL                                        /* line 45 */
    palette = initialize_component_palette_from_string ()/* line 46 */
    return [ palette,[ NULL, arg]]                     /* line 47 *//* line 48 *//* line 49 */}

void start (arg,part_name,palette,env) {
                                                       /* line 50 */
    part = start_bare ( part_name, palette, env)       /* line 51 */
    inject ( part, "", arg)                            /* line 52 */
    finalize ( part)                                   /* line 53 *//* line 54 *//* line 55 */}

void start_bare (part_name,palette,env) {
                                                       /* line 56 */
    diagram_names =  env [ 0]                          /* line 57 */
    /*  get entrypoint container */                    /* line 58 */
    part = get_component_instance ( palette, part_name, NULL)/* line 59 */
    if  NULL ==  part:                                 /* line 60 */
        load_error ( str( "Couldn;t find container with page name /") +  str( part_name) +  str( "/ in files ") +  str(str ( diagram_names)) +  " (check tab names, or disable compression?)"    )/* line 64 *//* line 65 */
    return ( part)                                     /* line 66 *//* line 67 *//* line 68 */}

void inject (part,port,payload) {
                                                       /* line 69 */
    if not  load_errors:                               /* line 70 */
        d =  Datum ()                                  /* line 71 */
        d.v =  payload                                 /* line 72 */
        d.clone =  lambda : obj_clone ( d)             /* line 73 */
        d.reclaim =  NULL                              /* line 74 */
        mev = make_mevent ( port, d)                   /* line 75 */
        inject_mevent ( part, mev)                     /* line 76 */;;;
    else:                                              /* line 77 */
        external exit (1)                              /* line 78 *//* line 79 *//* line 80 *//* line 81 */}

void finalize (part) {
                                                       /* line 82 */
    external print (deque_to_json ( part.outq))        /* line 83 *//* line 84 *//* line 85 */}

void new_datum_bang () {
                                                       /* line 86 */
    d =  Datum ()                                      /* line 87 */
    d.v =  "!"                                         /* line 88 */
    d.clone =  lambda : obj_clone ( d)                 /* line 89 */
    d.reclaim =  NULL                                  /* line 90 */
    return ( d)                                        /* line 91 *//* line 92 */;;;}
