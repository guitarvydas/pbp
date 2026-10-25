#include "pbp.h"

/*  Runtime data of a Part. */                         /* line 1 */
/*  Parts are either leaves (a user-supplied handler) or containers (a handler that routes mevents to children). *//* line 2 */
/*  `handler` is called as handler (eh, mev). */       /* line 3 */
/*  `instance_data` is private to the leaf; the handler finds it again each time it is invoked. *//* line 4 */
/*  `visit_queue` (containers only) holds the children that have been sent a mevent, once for each mevent. *//* line 5 */
/*  `tick` is a function (eh) that continues the processing of an active part. make_container sets it to container_tick; the programmer supplies it to make_leaf. *//* line 6 */
/*  `reset` is a function (eh) that puts the part back into a known state. make_container sets it to container_reset; the programmer supplies it to make_leaf. *//* line 7 */
/*  Eh_States :: enum { idle, active } */              /* line 8 */
Eh* fresh_Eh () {
    Eh *self;
    self = (Eh*)malloc(sizeof(Eh*));
    self->name =  counted("");                         /* line 10 */
    self->inq =  queue_fresh()                         /* line 11 */;
    self->outq =  queue_fresh()                        /* line 12 */;
    self->owner =  NULL;                               /* line 13 */
    self->children = list_fresh();                     /* line 14 */
    self->visit_queue =  queue_fresh()                 /* line 15 */;
    self->connections = list_fresh();                  /* line 16 */
    self->handler =  NULL;                             /* line 17 */
    self->tick =  NULL;                                /* line 18 */
    self->reset =  NULL;                               /* line 19 */
    self->instance_data =  NULL;                       /* line 20 */
    self->arg =  counted("");                          /* line 21 */
    self->state =  counted("idle");                    /* line 22 */
    self->vip =  FALSE;                                /* line 23 *//* line 24 */
    return self;
}
