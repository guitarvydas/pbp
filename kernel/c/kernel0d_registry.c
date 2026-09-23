/*
 * kernel0d_registry.c
 *
 * Template registry, JIT/AOT component lookup, and turning a parsed
 * "little network" (lnet) diagram into a live container of children
 * and connectors. JSON access goes through the abstract JsonValue API
 * declared in kernel0d.h (see note 5 there).
 */

#include "kernel0d.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Template *mkTemplate(const char *name, void *template_data, InstantiatorFn instantiator)
{
    Template *t = malloc(sizeof(Template));
    t->name = xstrdup(name);
    t->template_data = template_data;
    t->instantiator = instantiator;
    return t;
}

ComponentRegistry *abstracted_register_component(ComponentRegistry *reg, Template *tmpl, int ok_to_overwrite)
{
    char *name = mangle_name(tmpl->name);
    if (reg != NULL && component_registry_get(reg, name) != NULL && !ok_to_overwrite) {
        load_error(str_join("Component /", tmpl->name, "/ already declared", (char *) NULL));
        return reg;
    }
    component_registry_put(reg, name, tmpl);
    return reg;
}

ComponentRegistry *register_component(ComponentRegistry *reg, Template *tmpl)
{
    return abstracted_register_component(reg, tmpl, 0);
}

ComponentRegistry *register_component_allow_overwriting(ComponentRegistry *reg, Template *tmpl)
{
    return abstracted_register_component(reg, tmpl, 1);
}

char *generate_instance_name(const Eh *owner, const char *template_name)
{
    if (owner != NULL) {
        return str_join(owner->name, "\xE2\x96\xB9", template_name, (char *) NULL);
    } else {
        return xstrdup(template_name);
    }
}

Eh *get_component_instance(ComponentRegistry *reg, const char *full_name, Eh *owner)
{
    /* ":?<string>"  -- a probe part tagged with <string>
       ":$ <command>" -- a shell-out part that pipes to <command>
       ":<string>"    -- otherwise, a string part that emits <string> */
    char *template_name = mangle_name((char *) full_name);
    char *instance_name;
    Template *tmpl;
    Eh *instance;

    if (full_name[0] == ':') {
        instance_name = generate_instance_name(owner, template_name);
        instance = jit_instantiate(reg, owner, instance_name, full_name);
        free(instance_name);
        return instance;
    }

    tmpl = component_registry_get(reg, template_name);
    if (tmpl == NULL) {
        /* NOTE: the original has two near-identical branches here --
           "Registry Error (A)" for a key present but holding a NULL
           template, and "(B)" for the key missing outright. A NULL
           stored template never actually happens given how templates
           are always constructed via mkTemplate(), so both collapse
           to one NULL check here. */
        load_error(str_join("Registry Error (B): Can't find component /", template_name, "/", (char *) NULL));
        return NULL;
    }
    instance_name = generate_instance_name(owner, template_name);
    instance = tmpl->instantiator(reg, owner, instance_name, tmpl->template_data, "");
    free(instance_name);
    return instance;
}

/* ---- Connector construction from a parsed diagram ---- */

Connector create_down_connector(Eh *container, const JsonValue *proto_conn, ConnectorList *connectors, const IdMap *children_by_id)
{
    Connector connector;
    const JsonValue *target_proto;
    int id_proto;
    Eh *target_component;

    /* zeroed so an error path below (missing target) leaves a safely
       zeroed connector rather than reading uninitialized memory
       later, unlike Python's None */
    memset(&connector, 0, sizeof(connector));
    (void) connectors; /* unused in the original too */

    connector.direction = CONN_DOWN;
    connector.sender = mkSender(container->name, container, json_get_str_field(proto_conn, "source_port"));

    target_proto = json_object_get(proto_conn, "target");
    id_proto = json_get_int(json_object_get(target_proto, "id"));
    target_component = id_map_get(children_by_id, id_proto);

    if (target_component == NULL) {
        load_error(str_join("internal error: .Down connection target internal error ",
                             json_get_str_field(target_proto, "name"), (char *) NULL));
    } else {
        connector.receiver = mkReceiver(target_component->name, target_component,
                                         json_get_str_field(proto_conn, "target_port"),
                                         &target_component->inq);
    }
    return connector;
}

Connector create_across_connector(Eh *container, const JsonValue *proto_conn, ConnectorList *connectors, const IdMap *children_by_id)
{
    Connector connector;
    const JsonValue *source_proto, *target_proto;
    Eh *source_component, *target_component;

    memset(&connector, 0, sizeof(connector)); /* see create_down_connector */
    (void) container; /* unused in the original too */
    (void) connectors;

    connector.direction = CONN_ACROSS;
    source_proto = json_object_get(proto_conn, "source");
    target_proto = json_object_get(proto_conn, "target");
    source_component = id_map_get(children_by_id, json_get_int(json_object_get(source_proto, "id")));
    target_component = id_map_get(children_by_id, json_get_int(json_object_get(target_proto, "id")));

    if (source_component == NULL) {
        load_error(str_join("internal error: .Across connection source not ok ",
                             json_get_str_field(source_proto, "name"), (char *) NULL));
    } else {
        connector.sender = mkSender(source_component->name, source_component,
                                     json_get_str_field(proto_conn, "source_port"));
        if (target_component == NULL) {
            load_error(str_join("internal error: .Across connection target not ok ",
                                 json_get_str_field(target_proto, "name"), (char *) NULL));
        } else {
            connector.receiver = mkReceiver(target_component->name, target_component,
                                             json_get_str_field(proto_conn, "target_port"),
                                             &target_component->inq);
        }
    }
    return connector;
}

Connector create_up_connector(Eh *container, const JsonValue *proto_conn, ConnectorList *connectors, const IdMap *children_by_id)
{
    Connector connector;
    const JsonValue *source_proto;
    Eh *source_component;

    memset(&connector, 0, sizeof(connector)); /* see create_down_connector */
    (void) connectors;

    connector.direction = CONN_UP;
    source_proto = json_object_get(proto_conn, "source");
    source_component = id_map_get(children_by_id, json_get_int(json_object_get(source_proto, "id")));

    if (source_component == NULL) {
        load_error(str_join("internal error: .Up connection source not ok ",
                             json_get_str_field(source_proto, "name"), (char *) NULL));
    } else {
        connector.sender = mkSender(source_component->name, source_component,
                                     json_get_str_field(proto_conn, "source_port"));
        connector.receiver = mkReceiver(container->name, container,
                                         json_get_str_field(proto_conn, "target_port"),
                                         &container->outq);
    }
    return connector;
}

Connector create_through_connector(Eh *container, const JsonValue *proto_conn, ConnectorList *connectors, const IdMap *children_by_id)
{
    Connector connector;
    memset(&connector, 0, sizeof(connector)); /* so an error path below (missing
                                                   source/target) leaves a safely
                                                   zeroed connector rather than
                                                   reading uninitialized memory
                                                   later, unlike Python's None */
    (void) connectors;
    (void) children_by_id; /* unused in the original too */

    connector.direction = CONN_THROUGH;
    connector.sender = mkSender(container->name, container, json_get_str_field(proto_conn, "source_port"));
    connector.receiver = mkReceiver(container->name, container,
                                     json_get_str_field(proto_conn, "target_port"),
                                     &container->outq);
    return connector;
}

Eh *container_instantiator(ComponentRegistry *reg, Eh *owner, const char *container_name, void *desc_v, const char *arg)
{
    const JsonValue *desc = (const JsonValue *) desc_v;
    Eh *container;
    IdMap children_by_id;
    ConnectorList connectors;
    const JsonValue *children_arr, *connections_arr;
    int i, n;

    (void) arg;

    container = make_container(container_name, owner);
    id_map_init(&children_by_id);

    children_arr = json_object_get(desc, "children");
    n = json_array_length(children_arr);
    for (i = 0; i < n; i++) {
        const JsonValue *child_desc = json_array_get(children_arr, i);
        const char *child_name = json_get_str_field(child_desc, "name");
        Eh *child_instance = get_component_instance(reg, child_name, container);
        int id = json_get_int(json_object_get(child_desc, "id"));
        eh_list_append(&container->children, child_instance);
        id_map_put(&children_by_id, id, child_instance);
    }

    connector_list_init(&connectors);
    connections_arr = json_object_get(desc, "connections");
    n = json_array_length(connections_arr);
    for (i = 0; i < n; i++) {
        const JsonValue *proto_conn = json_array_get(connections_arr, i);
        int dir = json_get_int(json_object_get(proto_conn, "dir"));
        switch (dir) {
        case CONN_DOWN:
            connector_list_append(&connectors, create_down_connector(container, proto_conn, &connectors, &children_by_id));
            break;
        case CONN_ACROSS:
            connector_list_append(&connectors, create_across_connector(container, proto_conn, &connectors, &children_by_id));
            break;
        case CONN_UP:
            connector_list_append(&connectors, create_up_connector(container, proto_conn, &connectors, &children_by_id));
            break;
        case CONN_THROUGH:
            connector_list_append(&connectors, create_through_connector(container, proto_conn, &connectors, &children_by_id));
            break;
        default:
            break;
        }
    }
    container->connections = connectors;
    id_map_free(&children_by_id);
    return container;
}

/* ---- Loading a diagram's JSON from a file or an embedded string ---- */

JsonValue *lnet2internal_from_file(const char *container_xml)
{
    const char *pathname;
    char *filename;
    char *contents;
    JsonValue *routings;

    pathname = getenv("PBPWD");
    if (pathname == NULL) pathname = "<none>";
    filename = str_basename(container_xml);

    contents = read_entire_file(filename);
    if (contents == NULL) {
        fprintf(stderr, "File not found: '%s'\n", filename);
        free(filename);
        return NULL;
    }

    routings = json_parse_string(contents);
    free(contents);
    if (routings == NULL) {
        fprintf(stderr, "Error decoding JSON in path /%s/\n", pathname);
    }
    free(filename);
    return routings;
}

JsonValue *lnet2internal_from_string(const char *lnet)
{
    JsonValue *routings = json_parse_string(lnet);
    if (routings == NULL) {
        fprintf(stderr, "Error decoding JSON from string 'lnet'\n");
    }
    return routings;
}

void delete_decls(void *d)
{
    (void) d; /* no-op, matches the original */
}
