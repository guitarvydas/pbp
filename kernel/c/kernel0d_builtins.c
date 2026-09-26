/*
 * kernel0d_builtins.c
 *
 * The stock leaf components (`initialize_stock_components`), plus the
 * JIT-part machinery (jit_instantiate/handle_jit/probe_handler/
 * shell_out_handler) that backs ":..."-style inline diagram parts.
 */

#include "kernel0d.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define X_MARK "\xE2\x9C\x97" /* U+2717 BALLOT X, the "dropped/error" port */

/* ================================================================ */
/* JIT parts                                                          */
/* ================================================================ */

Eh *jit_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, const char *arg)
{
    char *name_with_id = gensymbol(name);
    Eh *inst;
    char firstc;

    (void) reg;
    inst = make_leaf(name_with_id, owner, NULL, arg, handle_jit, NULL);
    free(name_with_id);

    /* NOTE: the original reads name[1], not arg[1], to classify the
       part. `name` may already carry an owner prefix (see
       generate_instance_name), in which case this index doesn't line
       up with the ':' + kind-char scheme described in handle_jit.
       Preserved as-is for fidelity to the source. */
    firstc = name[1];
    if (firstc != '$') {
        inst->special = 1; /* probes go to the front of the line */
    }
    return inst;
}

void handle_jit(Eh *eh, Mevent *mev)
{
    const char *s = eh->arg;
    char firstc = s[1];
    if (firstc == '$') {
        shell_out_handler(eh, s + 3, mev); /* Python's s[1:][1:][1:] == s+3 */
    } else if (firstc == '?') {
        probe_handler(eh, s + 1, mev);
    } else {
        send_mevent(eh, "", xstrdup(s + 1), mev);
    }
}

void probe_handler(Eh *eh, const char *tag, Mevent *mev)
{
    const char *s = mev->payload->v;
    char *ticktime_str = int_to_str(ticktime);
    (void) tag; /* the original's `tag` parameter is likewise unused
                   in probe_handler's body */
    live_update("Info", str_join("  @", ticktime_str, "  probe ", eh->name, ": ", s, (char *) NULL));
    free(ticktime_str);
}

void shell_out_handler(Eh *eh, const char *cmd, Mevent *mev)
{
    const char *s;
    const char *pbpRoot;
    char *command;
    char stdin_path[L_tmpnam];
    char stdout_path[L_tmpnam];
    char stderr_path[L_tmpnam];
    char *full_cmd;
    int rc;
    char *stdout_text;
    char *stderr_text;
    char *combined;

    s = mev->payload->v;
    command = xstrdup(cmd);

    pbpRoot = getenv("PBP");
    if (pbpRoot == NULL) pbpRoot = "<none>";
    if (pbpRoot[0] != '\0') {
        char *replacement = str_join(pbpRoot, "/", (char *) NULL);
        char *replaced = str_replace_all(command, "_/", replacement);
        free(replacement);
        free(command);
        command = replaced;
    }

    if (getenv("PBPSHELLUT") != NULL) {
        fprintf(stderr, "- --- shell-out: %s\n", command);
    }

    tmpnam(stdin_path);
    tmpnam(stdout_path);
    tmpnam(stderr_path);
    write_entire_file(stdin_path, s);

    /* see note 7 in kernel0d.h: system()'s return value cannot be
       decoded into a real exit status in strict ANSI C. */
    full_cmd = str_join(command, " < ", stdin_path, " > ", stdout_path, " 2> ", stderr_path, (char *) NULL);
    rc = system(full_cmd);
    free(full_cmd);
    free(command);

    stdout_text = read_entire_file(stdout_path);
    stderr_text = read_entire_file(stderr_path);
    if (stdout_text == NULL) stdout_text = xstrdup("");
    if (stderr_text == NULL) stderr_text = xstrdup("");
    str_strip_inplace(stdout_text);
    str_strip_inplace(stderr_text);

    remove(stdin_path);
    remove(stdout_path);
    remove(stderr_path);

    combined = str_join(stdout_text, stderr_text, (char *) NULL);
    free(stdout_text);
    free(stderr_text);

    if (rc == 0) {
        send_mevent(eh, "", combined, mev);
    } else {
        send_mevent(eh, X_MARK, combined, mev);
    }
}

/* ================================================================ */
/* trash                                                              */
/* ================================================================ */

Eh *trash_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("trash");
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    eh = make_leaf(name_with_id, owner, NULL, "", trash_handler, NULL);
    free(name_with_id);
    return eh;
}

void trash_handler(Eh *eh, Mevent *mev)
{
    (void) eh; (void) mev; /* deliberately swallows the mevent */
}

/* ================================================================ */
/* deracer                                                            */
/* ================================================================ */

void reclaim_buffers_from_heap(DeracerInstanceData *inst)
{
    (void) inst; /* no-op, matches the original */
}

void deracer_reset_handler(Eh *eh)
{
    DeracerInstanceData *inst = (DeracerInstanceData *) eh->instance_data;
    inst->state = DERACER_IDLE;
    inst->buffer.firstmev = NULL;
    inst->buffer.secondmev = NULL;
}

Eh *deracer_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("deracer");
    DeracerInstanceData *inst = malloc(sizeof(DeracerInstanceData));
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    inst->state = DERACER_IDLE;
    inst->buffer.firstmev = NULL;
    inst->buffer.secondmev = NULL;
    eh = make_leaf(name_with_id, owner, inst, "", deracer_handler, deracer_reset_handler);
    free(name_with_id);
    return eh;
}

void send_firstmev_then_secondmev(Eh *eh, DeracerInstanceData *inst)
{
    forward(eh, "1", inst->buffer.firstmev);
    forward(eh, "2", inst->buffer.secondmev);
    reclaim_buffers_from_heap(inst);
}

void deracer_handler(Eh *eh, Mevent *mev)
{
    DeracerInstanceData *inst = (DeracerInstanceData *) eh->instance_data;
    switch (inst->state) {
    case DERACER_IDLE:
        if (strcmp(mev->port, "1") == 0) {
            inst->buffer.firstmev = mev;
            inst->state = DERACER_WAITING_SECOND;
        } else if (strcmp(mev->port, "2") == 0) {
            inst->buffer.secondmev = mev;
            inst->state = DERACER_WAITING_FIRST;
        } else {
            runtime_error(str_join("bad mev.port (case A) for deracer ", mev->port, (char *) NULL));
        }
        break;
    case DERACER_WAITING_FIRST:
        if (strcmp(mev->port, "1") == 0) {
            inst->buffer.firstmev = mev;
            send_firstmev_then_secondmev(eh, inst);
            inst->state = DERACER_IDLE;
        } else {
            runtime_error(str_join("deracer: waiting for 1 but got [", mev->port, "] (case B)", (char *) NULL));
        }
        break;
    case DERACER_WAITING_SECOND:
        if (strcmp(mev->port, "2") == 0) {
            inst->buffer.secondmev = mev;
            send_firstmev_then_secondmev(eh, inst);
            inst->state = DERACER_IDLE;
        } else {
            runtime_error(str_join("deracer: waiting for 2 but got [", mev->port, "] (case C)", (char *) NULL));
        }
        break;
    default:
        runtime_error("bad state for deracer");
        break;
    }
}

/* ================================================================ */
/* Read Text File                                                     */
/* ================================================================ */

Eh *low_level_read_text_file_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("Low Level Read Text File");
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    eh = make_leaf(name_with_id, owner, NULL, "", low_level_read_text_file_handler, NULL);
    free(name_with_id);
    return eh;
}

void low_level_read_text_file_handler(Eh *eh, Mevent *mev)
{
    const char *fname = mev->payload->v;
    char *data = read_entire_file(fname);
    if (data != NULL) {
        send_mevent(eh, "", data, mev);
    } else {
        /* the original distinguishes an open error from a read error;
           read_entire_file collapses both into one NULL result, so
           both are reported the same way here. */
        send_mevent(eh, X_MARK, str_join("open/read error on file '", fname, "'", (char *) NULL), mev);
    }
}

/* ================================================================ */
/* Ensure String Datum                                                */
/* ================================================================ */

Eh *ensure_string_datum_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("Ensure String Datum");
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    eh = make_leaf(name_with_id, owner, NULL, "", ensure_string_datum_handler, NULL);
    free(name_with_id);
    return eh;
}

void ensure_string_datum_handler(Eh *eh, Mevent *mev)
{
    if (strcmp(datum_kind(mev->payload), "string") == 0) {
        forward(eh, "", mev);
    } else {
        /* NOTE: the original concatenates the Datum object itself
           here (`mev.payload`, not `mev.payload.v`), which would
           raise a TypeError in real Python. Using .v, the evident
           intent. */
        char *msg = str_join("*** ensure: type error (expected a string payload) but got ",
                              mev->payload->v, (char *) NULL);
        send_mevent(eh, X_MARK, msg, mev);
    }
}

/* ================================================================ */
/* syncfilewrite                                                      */
/* ================================================================ */

void syncfilewrite_reset_handler(Eh *eh)
{
    SyncfilewriteData *inst = malloc(sizeof(SyncfilewriteData));
    inst->filename = xstrdup("");
    eh->instance_data = inst;
}

Eh *syncfilewrite_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("syncfilewrite");
    SyncfilewriteData *inst = malloc(sizeof(SyncfilewriteData));
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    inst->filename = xstrdup("");
    eh = make_leaf(name_with_id, owner, inst, "", syncfilewrite_handler, syncfilewrite_reset_handler);
    free(name_with_id);
    return eh;
}

void syncfilewrite_handler(Eh *eh, Mevent *mev)
{
    SyncfilewriteData *inst = (SyncfilewriteData *) eh->instance_data;
    if (strcmp(mev->port, "filename") == 0) {
        free(inst->filename);
        inst->filename = xstrdup(mev->payload->v);
    } else if (strcmp(mev->port, "input") == 0) {
        FILE *f = fopen(inst->filename, "w");
        if (f != NULL) {
            fputs(mev->payload->v, f);
            fclose(f);
            send_mevent(eh, "done", xstrdup("!"), mev);
        } else {
            send_mevent(eh, X_MARK, str_join("open error on file ", inst->filename, (char *) NULL), mev);
        }
    }
}

/* ================================================================ */
/* String Concat                                                      */
/* ================================================================ */

void stringconcat_reset_handler(Eh *eh)
{
    StringConcatInstanceData *inst = (StringConcatInstanceData *) eh->instance_data;
    inst->buffer1 = NULL; inst->buffer1_set = 0;
    inst->buffer2 = NULL; inst->buffer2_set = 0;
}

Eh *stringconcat_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("stringconcat");
    StringConcatInstanceData *instp = malloc(sizeof(StringConcatInstanceData));
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    instp->buffer1 = NULL; instp->buffer1_set = 0;
    instp->buffer2 = NULL; instp->buffer2_set = 0;
    eh = make_leaf(name_with_id, owner, instp, "", stringconcat_handler, stringconcat_reset_handler);
    free(name_with_id);
    return eh;
}

void stringconcat_handler(Eh *eh, Mevent *mev)
{
    StringConcatInstanceData *inst = (StringConcatInstanceData *) eh->instance_data;
    if (strcmp(mev->port, "1") == 0) {
        inst->buffer1 = clone_string(mev->payload->v);
        inst->buffer1_set = 1;
        maybe_stringconcat(eh, inst, mev);
    } else if (strcmp(mev->port, "2") == 0) {
        inst->buffer2 = clone_string(mev->payload->v);
        inst->buffer2_set = 1;
        maybe_stringconcat(eh, inst, mev);
    } else if (strcmp(mev->port, "reset") == 0) {
        inst->buffer1 = NULL; inst->buffer1_set = 0;
        inst->buffer2 = NULL; inst->buffer2_set = 0;
    } else {
        runtime_error(str_join("bad mev.port for stringconcat: ", mev->port, (char *) NULL));
    }
}

void maybe_stringconcat(Eh *eh, StringConcatInstanceData *inst, Mevent *mev)
{
    if (inst->buffer1_set && inst->buffer2_set) {
        char *concatenated;
        if (strlen(inst->buffer1) == 0) {
            concatenated = xstrdup(inst->buffer2);
        } else if (strlen(inst->buffer2) == 0) {
            concatenated = xstrdup(inst->buffer1);
        } else {
            concatenated = str_join(inst->buffer1, inst->buffer2, (char *) NULL);
        }
        send_mevent(eh, "", concatenated, mev);
        inst->buffer1 = NULL; inst->buffer1_set = 0;
        inst->buffer2 = NULL; inst->buffer2_set = 0;
    }
}

/* ================================================================ */
/* string_constant (not wired into initialize_stock_components in    */
/* the original either -- it takes its constant text via             */
/* template_data, so it must be registered per-instance elsewhere)   */
/* ================================================================ */

Eh *string_constant_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("strconst");
    char *s = xstrdup((const char *) template_data);
    Eh *eh;
    (void) reg; (void) name; (void) arg;
    if (projectRoot != NULL && projectRoot[0] != '\0') {
        char *replaced = str_replace_all(s, "_00_", projectRoot);
        free(s);
        s = replaced;
    }
    eh = make_leaf(name_with_id, owner, s, "", string_constant_handler, NULL);
    free(name_with_id);
    return eh;
}

void string_constant_handler(Eh *eh, Mevent *mev)
{
    char *s = (char *) eh->instance_data;
    send_mevent(eh, "", xstrdup(s), mev);
}

/* ================================================================ */
/* fakepipename                                                       */
/* ================================================================ */

static int fakepipe_counter = 0; /* Python's global `rand`, renamed to
                                     avoid colliding with C's rand() */

Eh *fakepipename_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *instance_name = gensymbol("fakepipe");
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    eh = make_leaf(instance_name, owner, NULL, "", fakepipename_handler, NULL);
    free(instance_name);
    return eh;
}

void fakepipename_handler(Eh *eh, Mevent *mev)
{
    char *num_str;
    fakepipe_counter++; /* not very random, but good enough: unique within a run */
    num_str = int_to_str(fakepipe_counter);
    send_mevent(eh, "", str_join("/tmp/fakepipe", num_str, (char *) NULL), mev);
    free(num_str);
}

/* ================================================================ */
/* switch1*                                                           */
/* ================================================================ */

void switch1star_reset_handler(Eh *eh)
{
    /* NOTE: the original rebinds its local `inst` to a fresh
       Switch1star_Instance_Data() without ever writing it back to
       eh.instance_data, so the Python reset handler is actually a
       no-op bug. Implemented here as the evident intent: reset the
       state to "1". */
    Switch1starInstanceData *inst = (Switch1starInstanceData *) eh->instance_data;
    inst->state = SWITCH1_STATE_1;
}

Eh *switch1star_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("switch1*");
    Switch1starInstanceData *instp = malloc(sizeof(Switch1starInstanceData));
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    instp->state = SWITCH1_STATE_1;
    eh = make_leaf(name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler);
    free(name_with_id);
    return eh;
}

void switch1star_handler(Eh *eh, Mevent *mev)
{
    Switch1starInstanceData *inst = (Switch1starInstanceData *) eh->instance_data;
    if (strcmp(mev->port, "") == 0) {
        if (inst->state == SWITCH1_STATE_1) {
            forward(eh, "1", mev);
            inst->state = SWITCH1_STATE_STAR;
        } else if (inst->state == SWITCH1_STATE_STAR) {
            forward(eh, "*", mev);
        } else {
            send_mevent(eh, X_MARK, xstrdup("internal error bad state in switch1*"), mev);
        }
    } else if (strcmp(mev->port, "reset") == 0) {
        inst->state = SWITCH1_STATE_1;
    } else {
        send_mevent(eh, X_MARK, xstrdup("internal error bad mevent for switch1*"), mev);
    }
}

/* ================================================================ */
/* String Concat *                                                    */
/* ================================================================ */

void strcatstar_reset_handler(Eh *eh)
{
    StringAccumulator *acc = (StringAccumulator *) eh->instance_data;
    free(acc->s);
    acc->s = xstrdup("");
}

Eh *strcatstar_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("String Concat *");
    StringAccumulator *instp = malloc(sizeof(StringAccumulator));
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    instp->s = xstrdup("");
    eh = make_leaf(name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler);
    free(name_with_id);
    return eh;
}

void strcatstar_handler(Eh *eh, Mevent *mev)
{
    StringAccumulator *accum = (StringAccumulator *) eh->instance_data;
    if (strcmp(mev->port, "") == 0) {
        char *joined = str_join(accum->s, mev->payload->v, (char *) NULL);
        free(accum->s);
        accum->s = joined;
    } else if (strcmp(mev->port, "fini") == 0) {
        send_mevent(eh, "", xstrdup(accum->s), mev);
    } else {
        send_mevent(eh, X_MARK, xstrdup("internal error bad mevent for String Concat *"), mev);
    }
}

/* ================================================================ */
/* Stop                                                                */
/* ================================================================ */

Eh *stop_instantiate(ComponentRegistry *reg, Eh *owner, const char *name, void *template_data, const char *arg)
{
    char *name_with_id = gensymbol("Stop");
    Eh *eh;
    (void) reg; (void) name; (void) template_data; (void) arg;
    eh = make_leaf(name_with_id, owner, NULL, "", stop_handler, NULL);
    free(name_with_id);
    return eh;
}

void stop_handler(Eh *eh, Mevent *mev)
{
    Eh *parent = eh->owner;
    fprintf(stderr, "   !!! stopping: '%s'\n", parent->name);
    parent->stop(parent);
    send_mevent(eh, "", xstrdup(mev->payload->v), mev);
}

/* ================================================================ */
/* Registration table for all of the above                            */
/* ================================================================ */

void initialize_stock_components(ComponentRegistry *reg)
{
    register_component(reg, mkTemplate("1then2", NULL, deracer_instantiate));
    register_component(reg, mkTemplate("1\xE2\x86\x92" "2" /* "1", U+2192, "2" -- split so the
                                                                trailing '2' isn't read as more
                                                                hex digits of the \x escape */,
                                        NULL, deracer_instantiate));
    register_component(reg, mkTemplate("trash", NULL, trash_instantiate));
    register_component(reg, mkTemplate("\xF0\x9F\x97\x91\xEF\xB8\x8F" /* wastebasket emoji */, NULL, trash_instantiate));
    register_component(reg, mkTemplate("\xF0\x9F\x9A\xAB" /* no-entry emoji */, NULL, stop_instantiate));
    register_component(reg, mkTemplate("Read Text File", NULL, low_level_read_text_file_instantiate));
    register_component(reg, mkTemplate("Ensure String Datum", NULL, ensure_string_datum_instantiate));
    register_component(reg, mkTemplate("syncfilewrite", NULL, syncfilewrite_instantiate));
    register_component(reg, mkTemplate("String Concat", NULL, stringconcat_instantiate));
    register_component(reg, mkTemplate("switch1*", NULL, switch1star_instantiate));
    register_component(reg, mkTemplate("String Concat *", NULL, strcatstar_instantiate));
    /* for fakepipe */
    register_component(reg, mkTemplate("fakepipename", NULL, fakepipename_instantiate));
}
