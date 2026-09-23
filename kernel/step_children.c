/*
 * step_children.c
 *
 * ANSI C (C89) translation of the "step_children" pseudo-code.
 *
 * Assumptions made about the surrounding kernel, since the original
 * pseudo-code only sketches the data model:
 *
 *   - "state" is one of IDLE or ACTIVE (mapped onto ContainerState).
 *   - Queue is an opaque type; the kernel already provides the
 *     primitives the pseudo-code marks with "#" (queue2list, empty,
 *     dequeue, resetQueue).
 *   - MEvent is an opaque type; only pointers to it are handled here.
 *   - container->children is a plain array of Container* of length
 *     num_children (phase 2's "for child in container.children").
 *     Phase 1 instead iterates the *visit order* list, which need
 *     not be the same sequence as container->children.
 *   - is_self, step_child_once, destroy_mevent, force_tick and route
 *     are domain functions defined elsewhere in the kernel.
 *
 * Adjust the extern declarations below to match your real headers
 * if the actual signatures differ.
 */

typedef enum {
    CONTAINER_IDLE,
    CONTAINER_ACTIVE
} ContainerState;

typedef struct Queue Queue;      /* opaque queue type */
typedef struct MEvent MEvent;    /* opaque message-event type */

typedef struct Container {
    ContainerState     state;
    Queue             *visit_ordering; /* queue of children, visiting order */
    struct Container **children;       /* array of child pointers */
    int                num_children;
    Queue             *inq;
    Queue             *outq;
} Container;

/* Kernel primitives (the "#"-prefixed operations in the pseudo-code) */
extern Container **queue2list(Queue *q, int *out_count);
extern int          q_empty(Queue *q);
extern MEvent      *q_dequeue(Queue *q);
extern void         reset_queue(Queue *q);

/* Domain functions used by the stepping algorithm */
extern int     is_self(Container *child, Container *container);
extern void    step_child_once(Container *child, MEvent *mev);
extern void    destroy_mevent(MEvent *mev);
extern MEvent *force_tick(Container *container, Container *child);
extern void    route(Container *container, Container *child, MEvent *mev);

void step_children(Container *container, MEvent *causingMevent)
{
    Container **list;
    int count;
    int i;

    /* causingMevent is not consulted by this phase of the algorithm;
       kept in the signature to match the original interface. */
    (void) causingMevent;

    container->state = CONTAINER_IDLE;

    /* phase 1 - loop through children and process inputs, or step
       children that are not "idle" */
    list = queue2list(container->visit_ordering, &count);
    for (i = 0; i < count; i++) {
        Container *child = list[i];

        /* child == container represents self; skip it */
        if (!is_self(child, container)) {
            if (!q_empty(child->inq)) {
                MEvent *mev = q_dequeue(child->inq);
                step_child_once(child, mev);
                destroy_mevent(mev);
            } else {
                if (child->state == CONTAINER_IDLE) {
                    /* pass */
                } else {
                    MEvent *mev = force_tick(container, child);
                    step_child_once(child, mev);
                    destroy_mevent(mev);
                }
            }
        }
    }
    reset_queue(container->visit_ordering);

    /* phase 2 - loop through children and route their outputs to the
       appropriate receiver queues based on .connections */
    for (i = 0; i < container->num_children; i++) {
        Container *child = container->children[i];

        if (child->state == CONTAINER_ACTIVE) {
            /* if child remains active, the container must remain
               active too, and must keep propagating "ticks" to it */
            container->state = CONTAINER_ACTIVE;
        }

        while (!q_empty(child->outq)) {
            MEvent *mev = q_dequeue(child->outq);
            route(container, child, mev);
            destroy_mevent(mev);
        }
    }
}
