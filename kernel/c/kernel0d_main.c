/*
 * kernel0d_main.c
 *
 * The top-level driver: building a component palette from one or more
 * diagram files (or an embedded diagram string), starting a named
 * part, injecting the initial mevent, and dumping its output queue as
 * JSON when the run settles.
 */

#include "kernel0d.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int load_errors_flag = 0;
int runtime_errors_flag = 0;
char *projectRoot = NULL; /* see kernel0d.h: set this from your driver
                              before using string_constant components */

void load_error(const char *s)
{
    fprintf(stderr, "%s\n", s);
    load_errors_flag = 1;
}

void runtime_error(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
    /* runtime_errors_flag = 1;  -- unreachable, exactly as in the
       original (the assignment after exit() there is dead code too) */
}

ComponentRegistry *initialize_component_palette_from_files(char **diagram_source_files, int count)
{
    ComponentRegistry *reg = make_component_registry();
    int i, j;
    for (i = 0; i < count; i++) {
        JsonValue *all_containers = lnet2internal_from_file(diagram_source_files[i]);
        int m = json_array_length(all_containers);
        for (j = 0; j < m; j++) {
            JsonValue *container = json_array_get(all_containers, j);
            const char *cname = json_get_str_field(container, "name");
            register_component(reg, mkTemplate(cname, container, container_instantiator));
        }
    }
    initialize_stock_components(reg);
    return reg;
}

ComponentRegistry *initialize_component_palette_from_string(const char *lnet)
{
    ComponentRegistry *reg = make_component_registry();
    JsonValue *all_containers = lnet2internal_from_string(lnet);
    int i, n = json_array_length(all_containers);
    for (i = 0; i < n; i++) {
        JsonValue *container = json_array_get(all_containers, i);
        const char *cname = json_get_str_field(container, "name");
        register_component(reg, mkTemplate(cname, container, container_instantiator));
    }
    initialize_stock_components(reg);
    return reg;
}

InitResult initialize_from_files(char **diagram_names, int count)
{
    InitResult r;
    r.arg = NULL;
    r.palette = initialize_component_palette_from_files(diagram_names, count);
    r.diagram_names = diagram_names;
    r.diagram_names_count = count;
    return r;
}

InitResult initialize_from_string(const char *lnet)
{
    /* NOTE: the original's initialize_from_string() calls
       initialize_component_palette_from_string() with no argument at
       all, which cannot work (that function requires `lnet`) -- given
       a `lnet` parameter here so this actually does something. */
    InitResult r;
    r.arg = NULL;
    r.palette = initialize_component_palette_from_string(lnet);
    r.diagram_names = NULL;
    r.diagram_names_count = 0;
    return r;
}

static char *join_names_for_message(char **names, int count)
{
    StrBuilder sb;
    int i;
    sb_init(&sb);
    sb_append(&sb, "[");
    for (i = 0; i < count; i++) {
        if (i > 0) sb_append(&sb, ", ");
        sb_append(&sb, names[i]);
    }
    sb_append(&sb, "]");
    return sb_to_cstr(&sb);
}

Eh *start_bare(const char *part_name, ComponentRegistry *palette, char **diagram_names, int diagram_names_count)
{
    Eh *part = get_component_instance(palette, part_name, NULL);
    if (part == NULL) {
        char *names_str = join_names_for_message(diagram_names, diagram_names_count);
        load_error(str_join("Couldn't find container with page name /", part_name,
                             "/ in files ", names_str,
                             " (check tab names, or disable compression?)", (char *) NULL));
        free(names_str);
    }
    return part;
}

void start(const char *arg, const char *part_name, ComponentRegistry *palette,
           char **diagram_names, int diagram_names_count)
{
    Eh *part = start_bare(part_name, palette, diagram_names, diagram_names_count);
    inject(part, "", arg);
    finalize_part(part);
}

void inject(Eh *part, const char *port, const char *payload)
{
    if (!load_errors_flag) {
        Datum *d = datum_new();
        Mevent *mev;
        d->v = xstrdup(payload);
        mev = make_mevent(port, d);
        inject_mevent(part, mev);
    } else {
        exit(1);
    }
}

char *deque_to_json(const MeventQueue *q)
{
    /* builds [ {"port":"value"}, ... ] with light indentation, close
       enough to json.dumps(..., indent=2) for this program's own
       debug output. */
    StrBuilder sb;
    MeventQueueNode *node;
    int first = 1;

    sb_init(&sb);
    sb_append(&sb, "[");
    for (node = q->head; node != NULL; node = node->next) {
        Mevent *mev = node->mev;
        const char *value = (mev->payload == NULL || mev->payload->v == NULL) ? "" : mev->payload->v;
        if (!first) sb_append(&sb, ",");
        first = 0;
        sb_append(&sb, "\n  {\n    \"");
        sb_append_escaped(&sb, mev->port);
        sb_append(&sb, "\": \"");
        sb_append_escaped(&sb, value);
        sb_append(&sb, "\"\n  }");
    }
    if (!first) sb_append(&sb, "\n");
    sb_append(&sb, "]");
    return sb_to_cstr(&sb);
}

void finalize_part(Eh *part)
{
    char *json = deque_to_json(&part->outq);
    printf("%s\n", json);
    free(json);
}

Datum *new_datum_bang(void)
{
    Datum *d = datum_new();
    d->v = xstrdup("!");
    return d;
}
