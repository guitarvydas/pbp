/*
 * kernel0d_core.c
 *
 * The heart of the kernel: mevent lifecycle, leaf/container
 * construction, and the step_children/route engine.
 */

#include "kernel0d.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int ticktime = 0;

/* ---- Datum ---- */

static Datum *datum_identity_clone(Datum *self)
{
    return self; /* see note 3 in kernel0d.h: not a real clone (yet),
                    matching obj_clone in the original */
}

Datum *datum_new(void)
{
    Datum *d = malloc(sizeof(Datum));
    d->v = NULL;
    d->clone = datum_identity_clone;
    d->reclaim = NULL;
    d->other = NULL;
    return d;
}

void *obj_clone(void *obj)
{
    return obj;
}

char *clone_string(const char *s)
{
    return (char *) s; /* identity, matching the original */
}

char *clone_port(const char *s)
{
    return clone_string(s);
}

char *mangle_name(const char *s)
{
    return (char *) s; /* identity in the original too */
}

/* ---- Mevent ---- */

Mevent *make_mevent(const char *port, Datum *datum)
{
    Mevent *m = malloc(sizeof(Mevent));
    m->port = clone_port(port);
    m->payload = datum->clone(datum);
    return m;
}

Mevent *mevent_clone(const Mevent *mev)
{
    Mevent *m = malloc(sizeof(Mevent));
    m->port = clone_port(mev->port);
    m->payload = mev->payload->clone(mev->payload);
    return m;
}

void destroy_mevent(Mevent *mev)
{
    (void) mev; /* no-op: see note 2 in kernel0d.h */
}

void destroy_datum(Datum *d)
{
    (void) d; /* no-op: see note 2 in kernel0d.h */
}

void destroy_port(char *port)
{
    (void) port; /* no-op: see note 2 in kernel0d.h */
}

char *format_mevent(const Mevent *m)
{
    /* NOTE: the original's literal here is
         "{%5C"" + port + "%5C":%5C"" + value + "%5C"}"
       which looks like mis-encoded escaped quotes (probably meant to
       be \" sequences that got garbled somewhere in this file's
       generation pipeline). Reconstructing the evident intent: a
       {"port":"value"} debug string. */
    if (m == NULL) {
        return xstrdup("{}");
    } else {
        const char *value = (m->payload != NULL && m->payload->v != NULL) ? m->payload->v : "";
        return str_join("{\"", m->port, "\":\"", value, "\"}", (char *) NULL);
    }
}

char *format_mevent_raw(const Mevent *m)
{
    if (m == NULL) {
        return xstrdup("");
    } else {
        return xstrdup(m->payload->v);
    }
}

/* ---- Sender / Receiver ---- */

Sender mkSender(const char *name, Eh *component, const char *port)
{
    Sender s;
    s.name = xstrdup(name);
    s.component = component;
    s.port = xstrdup(port);
    return s;
}

Receiver mkReceiver(const char *name, Eh *component, const char *port, MeventQueue *q)
{
    Receiver r;
    r.name = xstrdup(name);
    r.component = component;
    r.port = xstrdup(port);
    r.queue = q;
    return r;
}

/* ---- Eh construction ---- */

static Eh *eh_new(void)
{
    Eh *eh = malloc(sizeof(Eh));
    eh->name = xstrdup("");
    mevent_queue_init(&eh->inq);
    mevent_queue_init(&eh->outq);
    eh->owner = NULL;
    eh_list_init(&eh->children);
    eh_deque_init(&eh->visit_ordering);
    connector_list_init(&eh->connections);
    eh->handler = NULL;
    eh->reset_handler = NULL;
    eh->finject = NULL;
    eh->stop = NULL;
    eh->instance_data = NULL;
    eh->arg = xstrdup("");
    eh->state = EH_IDLE;
    eh->special = 0;
    eh->kind = NULL;
    return eh;
}

void injector(Eh *eh, Mevent *mevent)
{
    eh->handler(eh, mevent);
}

Eh *make_leaf(const char *name, Eh *owner, void *instance_data, const char *arg,
              HandlerFn handler, ResetHandlerFn reset_handler)
{
    Eh *eh = eh_new();
    const char *owner_name = (owner != NULL) ? owner->name : "";
    free(eh->name);
    eh->name = str_join(owner_name, "\xE2\x96\xB9" /* U+25B9 "\xe2\x96\xb9" */, name, (char *) NULL);
    eh->owner = owner;
    eh->handler = handler;
    eh->reset_handler = reset_handler;
    eh->finject = injector;
    eh->stop = leaf_reset;
    eh->instance_data = instance_data;
    free(eh->arg);
    eh->arg = xstrdup(arg);
    eh->state = EH_IDLE;
    return eh;
}

Eh *make_container(const char *name, Eh *owner)
{
    Eh *eh = eh_new();
    free(eh->name);
    eh->name = xstrdup(name);
    eh->owner = owner;
    eh->handler = container_handler;
    eh->finject = injector;
    eh->stop = container_reset_children;
    eh->state = EH_IDLE;
    eh->kind = "container";
    return eh;
}

void leaf_reset(Eh *part)
{
    mevent_queue_clear(&part->inq);
    mevent_queue_clear(&part->outq);
    if (part->reset_handler != NULL) {
        part->reset_handler(part);
    }
    part->state = EH_IDLE;
}

void container_reset_children(Eh *container)
{
    int i;
    for (i = 0; i < container->children.count; i++) {
        Eh *child = container->children.items[i];
        child->stop(child);
    }
    eh_deque_clear(&container->visit_ordering);
    mevent_queue_clear(&container->inq);
    mevent_queue_clear(&container->outq);
    container->state = EH_IDLE;
}

void destroy_container(Eh *eh)
{
    (void) eh; /* no-op, matches the original */
}

/* ---- Sending / forwarding ---- */

void send_mevent(Eh *eh, const char *port, char *obj, Mevent *causingMevent)
{
    Datum *d;
    Mevent *mev;
    (void) causingMevent; /* unused, matching the original's `send` */
    d = datum_new();
    d->v = obj;
    mev = make_mevent(port, d);
    put_output(eh, mev);
}

void forward(Eh *eh, const char *port, Mevent *mev)
{
    Mevent *fwdmev = make_mevent(port, mev->payload);
    put_output(eh, fwdmev);
}

void inject_mevent(Eh *eh, Mevent *mev)
{
    eh->finject(eh, mev);
}

void set_active(Eh *eh) { eh->state = EH_ACTIVE; }
void set_idle(Eh *eh)   { eh->state = EH_IDLE; }

void put_output(Eh *eh, Mevent *mev)
{
    mevent_queue_append(&eh->outq, mev);
}

/* ---- Container default handler ---- */

void container_handler(Eh *container, Mevent *mevent)
{
    route(container, container, mevent);
    while (any_child_ready(container)) {
        step_children(container, mevent);
    }
}

/* ---- The step/route engine ---- */

int is_self(const Eh *child, const Eh *container)
{
    return child == container;
}

void step_child_once(Eh *child, Mevent *mev)
{
    if (getenv("PBPSTEPPING") != NULL) {
        fprintf(stderr, "-- stepping <%s>\n", child->name);
        /* the original brackets the name with "\xe2\x9d\xae"/"\xe2\x9d\xaf"
           (U+276E/U+276F); using plain <> here for portability */
    }
    child->handler(child, mev);
}

void step_children(Eh *container, Mevent *causingMevent)
{
    Eh **list;
    int count, i;

    (void) causingMevent;

    container->state = EH_IDLE;

    /* phase 1 - loop through children and process inputs, or step
       children that are not "idle" */
    list = eh_deque_to_array(&container->visit_ordering, &count);
    for (i = 0; i < count; i++) {
        Eh *child = list[i];
        if (!is_self(child, container)) {
            if (child->inq.count != 0) {
                Mevent *mev = mevent_queue_dequeue(&child->inq);
                step_child_once(child, mev);
                destroy_mevent(mev);
            } else if (child->state != EH_IDLE) {
                Mevent *mev = force_tick(container, child);
                step_child_once(child, mev);
                destroy_mevent(mev);
            }
        }
    }
    free(list);
    eh_deque_clear(&container->visit_ordering);

    /* phase 2 - loop through children and route their outputs to the
       appropriate receiver queues based on .connections */
    for (i = 0; i < container->children.count; i++) {
        Eh *child = container->children.items[i];
        if (child->state == EH_ACTIVE) {
            container->state = EH_ACTIVE;
        }
        while (child->outq.count != 0) {
            Mevent *mev = mevent_queue_dequeue(&child->outq);
            route(container, child, mev);
            destroy_mevent(mev);
        }
    }
}

Mevent *force_tick(Eh *parent, Eh *eh)
{
    Mevent *tick_mev = make_mevent(".", new_datum_bang());
    push_mevent(parent, eh, &eh->inq, tick_mev);
    return tick_mev;
}

void push_mevent(Eh *parent, Eh *receiver, MeventQueue *inq, Mevent *m)
{
    mevent_queue_append(inq, m);
    if (receiver->special) {
        eh_deque_push_front(&parent->visit_ordering, receiver);
    } else {
        eh_deque_push_back(&parent->visit_ordering, receiver);
    }
}

int is_tick(const Mevent *mev)
{
    return strcmp(mev->port, ".") == 0;
}

void attempt_tick(Eh *parent, Eh *eh)
{
    if (eh->state != EH_IDLE) {
        force_tick(parent, eh);
    }
}

int sender_eq(const Sender *s1, const Sender *s2)
{
    int same_components = (s1->component == s2->component);
    int same_ports = (strcmp(s1->port, s2->port) == 0);
    return same_components && same_ports;
}

void deposit(Eh *parent, const Connector *conn, Mevent *mevent)
{
    Mevent *new_mevent = make_mevent(conn->receiver.port, mevent->payload);
    push_mevent(parent, conn->receiver.component, conn->receiver.queue, new_mevent);
}

void route(Eh *container, Eh *from_component, Mevent *mevent)
{
    int was_sent = 0;
    const char *fromname = "";
    int i;

    ticktime++;

    if (is_tick(mevent)) {
        for (i = 0; i < container->children.count; i++) {
            attempt_tick(container, container->children.items[i]);
        }
        was_sent = 1;
    } else {
        Sender from_sender;
        if (!is_self(from_component, container)) {
            fromname = from_component->name;
        }
        from_sender = mkSender(fromname, from_component, mevent->port);
        for (i = 0; i < container->connections.count; i++) {
            Connector *connector = &container->connections.items[i];
            if (sender_eq(&from_sender, &connector->sender)) {
                deposit(container, connector, mevent);
                was_sent = 1;
            }
        }
    }
    if (!was_sent) {
        live_update("internal error",
                     str_join(container->name, ": mevent on port '", mevent->port,
                              "' from ", fromname, " dropped on floor...", (char *) NULL));
    }
}

int any_child_ready(const Eh *container)
{
    int i;
    for (i = 0; i < container->children.count; i++) {
        if (child_is_ready(container->children.items[i])) {
            return 1;
        }
    }
    return 0;
}

int child_is_ready(const Eh *eh)
{
    return (eh->outq.count != 0) || (eh->inq.count != 0) ||
           (eh->state != EH_IDLE) || any_child_ready(eh);
}
