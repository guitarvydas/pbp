/*
 * kernel0d_example_stubs_and_main.c
 *
 * NOT part of the kernel itself -- this is a minimal, throwaway
 * implementation of the extern hooks declared in kernel0d.h
 * (JsonValue/json_*, datum_kind, live_update), plus a tiny main()
 * that wires up a container and a leaf by hand and runs a mevent
 * through it. It exists purely so the rest of the port builds and
 * runs out of the box; replace the JSON stubs with real bindings to
 * your JSON library (e.g. cJSON) once you're ready to load actual
 * diagrams, and replace live_update with your real REPL hookup.
 *
 * Build everything together, e.g.:
 *   gcc -std=c89 -pedantic -Wall -Wextra -o kernel0d_demo \
 *       kernel0d_util.c kernel0d_core.c kernel0d_registry.c \
 *       kernel0d_builtins.c kernel0d_main.c \
 *       kernel0d_example_stubs_and_main.c
 */

#include "kernel0d.h"
#include <stdio.h>
#include <string.h>

struct JsonValue { int dummy; }; /* replace with your JSON library's value type */

JsonValue *json_parse_file(const char *path) { (void) path; return NULL; }
JsonValue *json_parse_string(const char *text) { (void) text; return NULL; }
void json_free(JsonValue *v) { (void) v; }
JsonValue *json_object_get(const JsonValue *obj, const char *key) { (void) obj; (void) key; return NULL; }
const char *json_get_string(const JsonValue *v) { (void) v; return ""; }
int json_get_int(const JsonValue *v) { (void) v; return 0; }
int json_array_length(const JsonValue *arr) { (void) arr; return 0; }
JsonValue *json_array_get(const JsonValue *arr, int index) { (void) arr; (void) index; return NULL; }

/* kernel0d.py never actually defines Datum.kind() either -- see note 8
   in kernel0d.h. Every payload in this port is a string, so "string"
   is always correct until you add real payload typing. */
const char *datum_kind(const Datum *d) { (void) d; return "string"; }

void live_update(const char *level, const char *message)
{
    fprintf(stderr, "[%s] %s\n", level, message);
}

int main(void)
{
    ComponentRegistry *reg = make_component_registry();
    Eh *container, *leaf;
    Connector conn;

    initialize_stock_components(reg);

    /* build a tiny network by hand: top -> "String Concat *" */
    container = make_container("top", NULL);
    leaf = get_component_instance(reg, "String Concat *", container);
    eh_list_append(&container->children, leaf);

    conn.direction = CONN_DOWN;
    conn.sender = mkSender("top", container, "");
    conn.receiver = mkReceiver(leaf->name, leaf, "", &leaf->inq);
    connector_list_append(&container->connections, conn);

    inject(container, "", "hello, ");
    inject(container, "", "world");
    finalize_part(container); /* prints the container's outq as JSON --
                                  empty here, since nothing is wired to
                                  "fini" or to route the accumulated
                                  string back out */

    printf("done\n");
    return 0;
}
