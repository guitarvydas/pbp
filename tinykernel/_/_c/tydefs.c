/* line 1 *//* line 2 */
int  counter =  0                                      /* line 3 */;
int  ticktime =  0                                     /* line 4 */;/* line 5 *//* line 6 */
typedef struct _Component_Registry {
                                                       /* line 7 */
    DICTP templates;                                   /* line 8 *//* line 9 */
} Component_Registry;
Component_Registry fresh_Component_Registry () {
    Component_Registry *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->templates = {};                              /* line 8 *//* line 9 */
    return self;
}
                                                       /* line 10 */
typedef struct _Template {
                                                       /* line 11 */
    STR name;                                          /* line 12 */
    CONTAINERP container;                              /* line 13 */
    FINST instantiator;                                /* line 14 *//* line 15 */
} Template;
Template fresh_Template () {
    Template *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  NULL;                                /* line 12 */
    self->container =  NULL;                           /* line 13 */
    self->instantiator =  NULL;                        /* line 14 *//* line 15 */
    return self;
}
                                                       /* line 16 */
void mkTemplate (name,template_data,instantiator) {
                                                       /* line 17 */
    templ =  Template ()                               /* line 18 */
    templ.name =  name                                 /* line 19 */
    templ.template_data =  template_data               /* line 20 */
    templ.instantiator =  instantiator                 /* line 21 */
    return ( templ)                                    /* line 22 */;;;/* line 23 *//* line 24 */}

void make_component_registry () {
                                                       /* line 25 */
    return ( Component_Registry ()                     /* line 26 */)/* line 27 *//* line 28 */}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 29 */
/*  and output queues of mevents. */                   /* line 30 */
/*  */                                                 /* line 31 */
/*  Components can either be a user_supplied function ("leaf“), or a “container“ *//* line 32 */
/*  that routes mevents to child components according to a list of connections *//* line 33 */
/*  that serve as a mevent routing table. */           /* line 34 */
/*  */                                                 /* line 35 */
/*  Child components themselves can be leaves or other containers. *//* line 36 */
/*  */                                                 /* line 37 */
/*  `handler` invokes the code that is attached to this component. *//* line 38 */
/*  */                                                 /* line 39 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 40 */
/*  function may want whenever it is invoked again. */ /* line 41 *//* line 42 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 43 *//* line 44 */
/*  Eh_States :: enum { idle, active } */              /* line 45 */
typedef struct _Eh {
                                                       /* line 46 */
    STR name;                                          /* line 47 */
    QP inq;
    QP outq;
    PARTP owner;                                       /* line 50 */
    LISTP children;                                    /* line 51 */
    QP visit_ordering;
    LISTP connections;                                 /* line 53 */
    QP routings;
    FHANDLER handler;                                  /* line 55 */
    FPROC reset_instance_data;                         /* line 56 */
    FINJECT finject;                                   /* line 57 */
    FPROC stop;                                        /* line 58 */
    STR instance_data;                                 /* line 59 *//*  arg needed for probe support  *//* line 60 */
    STR arg;                                           /* line 61 */
    STR state;                                         /* line 62 */
    FLAG special;                                      /* line 63 *//*  bootstrap debugging *//* line 64 */
    KINDENUM kind; /*  enum { container, leaf, } */    /* line 65 *//* line 66 */
} Eh;
Eh fresh_Eh () {
    Eh *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->name =  "";                                  /* line 47 */
    self->inq =  deque ([])                            /* line 48 */;
    self->outq =  deque ([])                           /* line 49 */;
    self->owner =  NULL;                               /* line 50 */
    self->children = [];                               /* line 51 */
    self->visit_ordering =  deque ([])                 /* line 52 */;
    self->connections = [];                            /* line 53 */
    self->routings =  deque ([])                       /* line 54 */;
    self->handler =  NULL;                             /* line 55 */
    self->reset_instance_data =  NULL;                 /* line 56 */
    self->finject =  NULL;                             /* line 57 */
    self->stop =  NULL;                                /* line 58 */
    self->instance_data =  NULL;                       /* line 59 *//*  arg needed for probe support  *//* line 60 */
    self->arg =  "";                                   /* line 61 */
    self->state =  "idle";                             /* line 62 */
    self->special =  False;                            /* line 63 *//*  bootstrap debugging *//* line 64 */
    self->kind =  NULL; /*  enum { container, leaf, } *//* line 65 *//* line 66 */
    return self;
}
                                                       /* line 67 */
int  load_errors =  False                              /* line 68 */;
int  runtime_errors =  False                           /* line 69 */;/* line 70 */
void clone_string (s) {
                                                       /* line 71 */
    return ( s)                                        /* line 72 *//* line 73 *//* line 74 */}

void injector (eh,mevent) {
                                                       /* line 75 */
    eh.handler ( eh, mevent)                           /* line 76 *//* line 77 *//* line 78 */}
