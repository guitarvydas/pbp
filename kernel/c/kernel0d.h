/*
 * kernel0d.h
 *
 * ANSI C (C89) port of kernel0d.py -- the 0D / Parts-Based-Programming
 * message-passing kernel (Eh components, Mevents, step/route engine,
 * component registry, and the stock built-in leaf components).
 *
 * ============================================================
 * READ THIS FIRST -- assumptions and deliberate simplifications
 * ============================================================
 *
 * 1. Payload type. Every Datum.v in the original file is, in practice,
 *    always a plain text string (file contents, shell output, string
 *    concatenation results, etc.). Datum.v is therefore modeled here
 *    as an owned, heap-allocated `char *`, not a generic/variant type.
 *
 * 2. Memory is intentionally leaked, matching the source. destroy_mevent,
 *    destroy_datum and destroy_port are literally `pass` in the Python
 *    ("during debug, dont destroy any mevent, since we want to trace
 *    mevents"). This matters for correctness, not just tidiness: some
 *    handlers (e.g. the deracer) stash a Mevent pointer for use across
 *    multiple steps, after step_children has already "destroyed" it.
 *    These functions are kept as no-ops here for the same reason.
 *
 * 3. clone_string / clone_port / obj_clone are identity functions in
 *    the original (Python strings are immutable, so "cloning" just
 *    returns the same reference) -- preserved as identity here too.
 *
 * 4. Containers with no direct ANSI C equivalent are hand-rolled:
 *      - MeventQueue   : singly linked FIFO (models a `deque` used
 *                        only with append()/popleft()).
 *      - EhDeque       : doubly-endable queue (models `visit_ordering`,
 *                        which needs both append() and appendleft()).
 *      - EhList        : growable array of Eh* (models `.children`).
 *      - ConnectorList : growable array of Connector (models
 *                        `.connections`).
 *      - ComponentRegistry : simple growable name->Template* table,
 *                        linear lookup (registries here hold at most a
 *                        few dozen entries, so a hash table would be
 *                        over-engineering).
 *      - IdMap         : growable int-id->Eh* table, linear lookup,
 *                        used only while wiring one diagram's children.
 *
 * 5. JSON is left as an abstract API (JsonValue is opaque), the way
 *    the previous step_children.c treated the kernel's queue
 *    primitives. Writing a full JSON parser is a separate project;
 *    wire json_parse_file/json_parse_string/json_object_get/etc.
 *    up to whatever JSON library you use (e.g. cJSON) -- only the
 *    handful of accessors declared below are needed.
 *
 * 6. live_update() (imported from a `repl` module in the original,
 *    presumably backed by a socket to a live REPL/UI) is declared
 *    `extern` and left for you to wire up. socket/hashlib/base64/struct
 *    /random are imported in kernel0d.py but never actually used in
 *    it, so nothing was carried over for them.
 *
 * 7. shell_out_handler needs to run a subprocess and capture stdout,
 *    stderr and an exit status separately. Plain ANSI C has no process
 *    API beyond system() (also part of C89), whose return value is
 *    implementation-defined -- there is no portable way, in strict
 *    ANSI C, to recover a real exit code the way POSIX's
 *    WIFEXITED/WEXITSTATUS would. The implementation here shells out
 *    via system() with manual stdin/stdout/stderr redirection to
 *    temp files (tmpnam() + fopen()), and treats a nonzero system()
 *    result as failure. tmpnam() has well-known TOCTOU issues; a real
 *    deployment on a POSIX system should switch to mkstemp()/popen()
 *    instead (no longer strict ANSI C, but far safer).
 *
 * 8. A handful of apparent bugs/dead code in the original are noted
 *    at their call sites (grep this file and kernel0d_*.c for "NOTE:"):
 *      - Template.container_data is an unused leftover; Template
 *        never actually declared a `template_data` field, though
 *        that's the one every caller reads/writes.
 *      - switch1star_reset_handler rebinds its local `inst` instead of
 *        writing back to eh.instance_data, so the reset silently did
 *        nothing; here it actually resets the state.
 *      - jit_instantiate reads name[1] (not arg[1]) to classify the
 *        JIT part; preserved as-is even though `name` may carry an
 *        owner prefix.
 *      - probe_handler's `tag` parameter, and shell_out_handler's use
 *        of a *value* rather than value.v in ensure_string_datum_handler
 *        (a str+Datum concatenation that would actually raise
 *        TypeError in Python) are both adjusted to the evident intent.
 *      - initialize_from_string() called initialize_component_palette_
 *        from_string() with no argument in the original, which cannot
 *        work at all (missing required parameter) -- given a `lnet`
 *        parameter here so it compiles and does something.
 *      - ensure_string_datum_handler calls mev.payload.kind(), but
 *        Datum never defines any such method/attribute anywhere in
 *        this file; declared as an extern hook (datum_kind) since it
 *        must be supplied by code outside kernel0d.py.
 *
 * Unicode symbols used as port/component names (e.g. the dropped-mevent
 * marker "\xE2\x9C\x97" for U+2717 "\xe2\x9c\x97") are written as raw
 * UTF-8 hex-escaped byte sequences rather than literal source
 * characters, so the file's meaning does not depend on your compiler
 * or editor's assumed source encoding.
 */

#ifndef KERNEL0D_H
#define KERNEL0D_H

#include <stdio.h>

/* ---------------------------------------------------------------- */
/* Forward declarations / opaque and function-pointer types          */
/* ---------------------------------------------------------------- */

typedef struct Eh Eh;
typedef struct Datum Datum;
typedef struct Mevent Mevent;
typedef struct ComponentRegistry ComponentRegistry;
typedef struct JsonValue JsonValue; /* opaque; see note 5 above */

typedef void  (*HandlerFn)(Eh *eh, Mevent *mev);
typedef void  (*ResetHandlerFn)(Eh *eh);
typedef void  (*InjectFn)(Eh *eh, Mevent *mev);
typedef void  (*StopFn)(Eh *eh);
typedef Datum *(*DatumCloneFn)(Datum *self);
typedef void  (*DatumReclaimFn)(Datum *self);
typedef Eh    *(*InstantiatorFn)(ComponentRegistry *reg, Eh *owner,
                                  const char *name, void *template_data,
                                  const char *arg);

/* ---------------------------------------------------------------- */
/* Datum / Mevent                                                     */
/* ---------------------------------------------------------------- */

struct Datum {
    char *v;                 /* see note 1: always a string here */
    DatumCloneFn clone;
    DatumReclaimFn reclaim;  /* always NULL in the original; reserved */
    void *other;             /* reserved for per-project use */
};

struct Mevent {
    char  *port;
    Datum *payload;
};

/* ---------------------------------------------------------------- */
/* Small hand-rolled containers                                       */
/* ---------------------------------------------------------------- */

typedef struct MeventQueueNode {
    Mevent *mev;
    struct MeventQueueNode *next;
} MeventQueueNode;

typedef struct MeventQueue {
    MeventQueueNode *head;
    MeventQueueNode *tail;
    int count;
} MeventQueue;

typedef struct EhDequeNode {
    Eh *eh;
    struct EhDequeNode *next;
} EhDequeNode;

typedef struct EhDeque {
    EhDequeNode *head;
    EhDequeNode *tail;
    int count;
} EhDeque;

typedef struct EhList {
    Eh **items;
    int count;
    int capacity;
} EhList;

/* ---------------------------------------------------------------- */
/* Routing: Sender / Receiver / Connector                             */
/* ---------------------------------------------------------------- */

typedef enum {
    CONN_DOWN = 0,
    CONN_ACROSS = 1,
    CONN_UP = 2,
    CONN_THROUGH = 3
} ConnDirection;

typedef struct Sender {
    char *name;
    Eh   *component;
    char *port;
} Sender;

typedef struct Receiver {
    char *name;
    Eh   *component;
    char *port;
    MeventQueue *queue;
} Receiver;

typedef struct Connector {
    ConnDirection direction;
    Sender   sender;
    Receiver receiver;
} Connector;

typedef struct ConnectorList {
    Connector *items;
    int count;
    int capacity;
} ConnectorList;

/* ---------------------------------------------------------------- */
/* Eh: the component record                                           */
/* ---------------------------------------------------------------- */

typedef enum { EH_IDLE, EH_ACTIVE } EhState;

struct Eh {
    char   *name;
    MeventQueue inq;
    MeventQueue outq;
    Eh     *owner;
    EhList  children;
    EhDeque visit_ordering;
    ConnectorList connections;
    HandlerFn      handler;
    ResetHandlerFn reset_handler;   /* leaf-only; NULL for containers */
    InjectFn       finject;
    StopFn         stop;
    void   *instance_data;
    char   *arg;
    EhState state;
    int     special;
    const char *kind;               /* "container", or NULL for leaves */
};

/* ---------------------------------------------------------------- */
/* Component registry / templates                                     */
/* ---------------------------------------------------------------- */

typedef struct Template {
    char *name;
    void *template_data;   /* see note 8: replaces the unused
                               `.container` field of the original */
    InstantiatorFn instantiator;
} Template;

typedef struct TemplateEntry {
    char *key;
    Template *value;
} TemplateEntry;

struct ComponentRegistry {
    TemplateEntry *entries;
    int count;
    int capacity;
};

typedef struct IdMapEntry {
    int id;
    Eh *eh;
} IdMapEntry;

typedef struct IdMap {
    IdMapEntry *items;
    int count;
    int capacity;
} IdMap;

/* result of initialize_from_files / initialize_from_string, modeling
   the original's `[palette, [diagram_names, arg]]` return value */
typedef struct InitResult {
    ComponentRegistry *palette;
    char **diagram_names;
    int    diagram_names_count;
    void  *arg;
} InitResult;

/* ---------------------------------------------------------------- */
/* Extern hooks -- supply these from your JSON library / REPL layer   */
/* ---------------------------------------------------------------- */

extern JsonValue  *json_parse_file(const char *path);   /* NULL on error */
extern JsonValue  *json_parse_string(const char *text);  /* NULL on error */
extern void        json_free(JsonValue *v);
extern JsonValue  *json_object_get(const JsonValue *obj, const char *key);
extern const char *json_get_string(const JsonValue *v);
extern int         json_get_int(const JsonValue *v);
extern int         json_array_length(const JsonValue *arr);
extern JsonValue  *json_array_get(const JsonValue *arr, int index);

/* see note 8: not defined anywhere in kernel0d.py either */
extern const char *datum_kind(const Datum *d);

/* imported from `repl` in the original */
extern void live_update(const char *level, const char *message);

/* ---------------------------------------------------------------- */
/* Globals                                                             */
/* ---------------------------------------------------------------- */

extern int ticktime;
extern int load_errors_flag;
extern int runtime_errors_flag;
extern char *projectRoot;  /* referenced via `global projectRoot` in the
                               original but never assigned there either;
                               set it from your driver before using
                               string_constant components. */

/* ---------------------------------------------------------------- */
/* kernel0d_util.c                                                     */
/* ---------------------------------------------------------------- */

char  *xstrdup(const char *s);
char  *str_join(const char *first, ...); /* NULL-terminated (char *)NULL sentinel */
char  *str_replace_all(const char *haystack, const char *needle, const char *replacement);
void   str_strip_inplace(char *s);
char  *str_basename(const char *path);
char  *int_to_str(int n);
char  *read_entire_file(const char *path); /* NULL if it can't be opened/read */
int    write_entire_file(const char *path, const char *contents); /* 0 on failure */
const char *json_get_str_field(const JsonValue *obj, const char *key);

char  *subscripted_digit(int n);
char  *gensymbol(const char *s);

void   mevent_queue_init(MeventQueue *q);
void   mevent_queue_append(MeventQueue *q, Mevent *m);
Mevent *mevent_queue_dequeue(MeventQueue *q); /* pop from the front */
void   mevent_queue_clear(MeventQueue *q);

void   eh_deque_init(EhDeque *d);
void   eh_deque_push_back(EhDeque *d, Eh *eh);
void   eh_deque_push_front(EhDeque *d, Eh *eh);
void   eh_deque_clear(EhDeque *d);
Eh   **eh_deque_to_array(const EhDeque *d, int *out_count); /* caller frees */

void   eh_list_init(EhList *l);
void   eh_list_append(EhList *l, Eh *eh);

void   connector_list_init(ConnectorList *l);
void   connector_list_append(ConnectorList *l, Connector c);

void   id_map_init(IdMap *m);
void   id_map_put(IdMap *m, int id, Eh *eh);
Eh    *id_map_get(const IdMap *m, int id); /* NULL if absent */
void   id_map_free(IdMap *m);

ComponentRegistry *make_component_registry(void);
void    component_registry_put(ComponentRegistry *reg, const char *key, Template *tmpl);
Template *component_registry_get(const ComponentRegistry *reg, const char *key);

typedef struct StrBuilder {
    char *buf;
    int   len;
    int   capacity;
} StrBuilder;

void sb_init(StrBuilder *sb);
void sb_append(StrBuilder *sb, const char *text);
void sb_append_escaped(StrBuilder *sb, const char *text); /* minimal JSON string escaping */
char *sb_to_cstr(StrBuilder *sb); /* transfers ownership of the buffer */

/* ---------------------------------------------------------------- */
/* kernel0d_core.c                                                     */
/* ---------------------------------------------------------------- */

Datum  *datum_new(void);
Mevent *make_mevent(const char *port, Datum *datum);
Mevent *mevent_clone(const Mevent *mev);
void    destroy_mevent(Mevent *mev);   /* no-op: see note 2 */
void    destroy_datum(Datum *d);       /* no-op: see note 2 */
void    destroy_port(char *port);      /* no-op: see note 2 */
char   *format_mevent(const Mevent *m);
char   *format_mevent_raw(const Mevent *m);

char   *clone_string(const char *s);   /* identity: see note 3 */
char   *clone_port(const char *s);     /* identity: see note 3 */
void   *obj_clone(void *obj);          /* identity: see note 3 */
char   *mangle_name(const char *s);    /* identity in the original */

Sender   mkSender(const char *name, Eh *component, const char *port);
Receiver mkReceiver(const char *name, Eh *component, const char *port, MeventQueue *q);

Eh   *make_leaf(const char *name, Eh *owner, void *instance_data, const char *arg,
                 HandlerFn handler, ResetHandlerFn reset_handler);
Eh   *make_container(const char *name, Eh *owner);
void  leaf_reset(Eh *part);
void  container_reset_children(Eh *container);
void  destroy_container(Eh *eh); /* no-op, matches the original */

void  injector(Eh *eh, Mevent *mevent);
int   is_self(const Eh *child, const Eh *container);
void  step_child_once(Eh *child, Mevent *mev);
void  step_children(Eh *container, Mevent *causingMevent);
Mevent *force_tick(Eh *parent, Eh *eh);
void  push_mevent(Eh *parent, Eh *receiver, MeventQueue *inq, Mevent *m);
int   is_tick(const Mevent *mev);
void  attempt_tick(Eh *parent, Eh *eh);
void  route(Eh *container, Eh *from_component, Mevent *mevent);
void  deposit(Eh *parent, const Connector *conn, Mevent *mevent);
int   sender_eq(const Sender *s1, const Sender *s2);
int   any_child_ready(const Eh *container);
int   child_is_ready(const Eh *eh);
void  container_handler(Eh *container, Mevent *mevent);

void  send_mevent(Eh *eh, const char *port, char *obj, Mevent *causingMevent); /* Python's `send` (renamed: C's send() is a socket call) */
void  forward(Eh *eh, const char *port, Mevent *mev);
void  inject_mevent(Eh *eh, Mevent *mev);
void  set_active(Eh *eh);
void  set_idle(Eh *eh);
void  put_output(Eh *eh, Mevent *mev);

/* ---------------------------------------------------------------- */
/* kernel0d_registry.c                                                 */
/* ---------------------------------------------------------------- */

Template *mkTemplate(const char *name, void *template_data, InstantiatorFn instantiator);
ComponentRegistry *register_component(ComponentRegistry *reg, Template *tmpl);
ComponentRegistry *register_component_allow_overwriting(ComponentRegistry *reg, Template *tmpl);
ComponentRegistry *abstracted_register_component(ComponentRegistry *reg, Template *tmpl, int ok_to_overwrite);
Eh *get_component_instance(ComponentRegistry *reg, const char *full_name, Eh *owner);
char *generate_instance_name(const Eh *owner, const char *template_name);

Connector create_down_connector(Eh *container, const JsonValue *proto_conn, ConnectorList *connectors, const IdMap *children_by_id);
Connector create_across_connector(Eh *container, const JsonValue *proto_conn, ConnectorList *connectors, const IdMap *children_by_id);
Connector create_up_connector(Eh *container, const JsonValue *proto_conn, ConnectorList *connectors, const IdMap *children_by_id);
Connector create_through_connector(Eh *container, const JsonValue *proto_conn, ConnectorList *connectors, const IdMap *children_by_id);
Eh *container_instantiator(ComponentRegistry *reg, Eh *owner, const char *container_name, void *desc, const char *arg);

JsonValue *lnet2internal_from_file(const char *container_xml);
JsonValue *lnet2internal_from_string(const char *lnet);
void delete_decls(void *d); /* no-op, matches the original */

/* ---------------------------------------------------------------- */
/* kernel0d_builtins.c                                                 */
/* ---------------------------------------------------------------- */

Eh   *jit_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, const char *arg);
void  handle_jit(Eh *eh, Mevent *mev);
void  probe_handler(Eh *eh, const char *tag, Mevent *mev);
void  shell_out_handler(Eh *eh, const char *cmd, Mevent *mev);

Eh *trash_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void trash_handler(Eh *eh, Mevent *mev);

typedef struct TwoMevents {
    Mevent *firstmev;
    Mevent *secondmev;
} TwoMevents;

typedef enum { DERACER_IDLE, DERACER_WAITING_FIRST, DERACER_WAITING_SECOND } DeracerState;

typedef struct DeracerInstanceData {
    DeracerState state;
    TwoMevents buffer;
} DeracerInstanceData;

void reclaim_buffers_from_heap(DeracerInstanceData *inst); /* no-op, matches the original */
void deracer_reset_handler(Eh *eh);
Eh  *deracer_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void send_firstmev_then_secondmev(Eh *eh, DeracerInstanceData *inst);
void deracer_handler(Eh *eh, Mevent *mev);

Eh *low_level_read_text_file_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void low_level_read_text_file_handler(Eh *eh, Mevent *mev);

Eh *ensure_string_datum_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void ensure_string_datum_handler(Eh *eh, Mevent *mev);

typedef struct SyncfilewriteData {
    char *filename;
} SyncfilewriteData;

void syncfilewrite_reset_handler(Eh *eh);
Eh  *syncfilewrite_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void syncfilewrite_handler(Eh *eh, Mevent *mev);

typedef struct StringConcatInstanceData {
    char *buffer1; int buffer1_set;
    char *buffer2; int buffer2_set;
} StringConcatInstanceData;

void stringconcat_reset_handler(Eh *eh);
Eh  *stringconcat_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void stringconcat_handler(Eh *eh, Mevent *mev);
void maybe_stringconcat(Eh *eh, StringConcatInstanceData *inst, Mevent *mev);

Eh *string_constant_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void string_constant_handler(Eh *eh, Mevent *mev);

Eh *fakepipename_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void fakepipename_handler(Eh *eh, Mevent *mev);

typedef enum { SWITCH1_STATE_1, SWITCH1_STATE_STAR } Switch1starState;
typedef struct Switch1starInstanceData {
    Switch1starState state;
} Switch1starInstanceData;

void switch1star_reset_handler(Eh *eh);
Eh  *switch1star_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void switch1star_handler(Eh *eh, Mevent *mev);

typedef struct StringAccumulator {
    char *s;
} StringAccumulator;

void strcatstar_reset_handler(Eh *eh);
Eh  *strcatstar_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void strcatstar_handler(Eh *eh, Mevent *mev);

Eh *stop_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg);
void stop_handler(Eh *eh, Mevent *mev);

void initialize_stock_components(ComponentRegistry *reg);

/* ---------------------------------------------------------------- */
/* kernel0d_main.c                                                     */
/* ---------------------------------------------------------------- */

void load_error(const char *s);
void runtime_error(const char *s);

ComponentRegistry *initialize_component_palette_from_files(char **diagram_source_files, int count);
ComponentRegistry *initialize_component_palette_from_string(const char *lnet);
InitResult initialize_from_files(char **diagram_names, int count);
InitResult initialize_from_string(const char *lnet); /* see note 8 */

void start(const char *arg, const char *part_name, ComponentRegistry *palette,
           char **diagram_names, int diagram_names_count);
Eh  *start_bare(const char *part_name, ComponentRegistry *palette,
                 char **diagram_names, int diagram_names_count);
void inject(Eh *part, const char *port, const char *payload);
void finalize_part(Eh *part); /* Python's `finalize` (avoids clashing with C++ / some libc `finalize`) */
Datum *new_datum_bang(void);
char *deque_to_json(const MeventQueue *d);

#endif /* KERNEL0D_H */
