/*
 * kernel0d_util.c
 *
 * String helpers, file I/O helpers, and the small hand-rolled
 * container types (queue/deque/growable-array/lookup-table) that
 * stand in for Python's deque/list/dict in this port. See the big
 * comment at the top of kernel0d.h for the overall rationale.
 */

#include "kernel0d.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

/* ================================================================ */
/* Strings                                                            */
/* ================================================================ */

char *xstrdup(const char *s)
{
    size_t n;
    char *copy;
    if (s == NULL) {
        s = "";
    }
    n = strlen(s) + 1;
    copy = malloc(n);
    memcpy(copy, s, n);
    return copy;
}

/* Concatenates a NULL-terminated (const char *) NULL sentinel list of
   strings into one freshly malloc'd string. */
char *str_join(const char *first, ...)
{
    va_list args;
    const char *part;
    size_t total;
    char *result;
    char *cursor;

    total = (first != NULL) ? strlen(first) : 0;
    va_start(args, first);
    for (part = va_arg(args, const char *); part != NULL; part = va_arg(args, const char *)) {
        total += strlen(part);
    }
    va_end(args);

    result = malloc(total + 1);
    cursor = result;

    if (first != NULL) {
        size_t n = strlen(first);
        memcpy(cursor, first, n);
        cursor += n;
    }
    va_start(args, first);
    for (part = va_arg(args, const char *); part != NULL; part = va_arg(args, const char *)) {
        size_t n = strlen(part);
        memcpy(cursor, part, n);
        cursor += n;
    }
    va_end(args);

    *cursor = '\0';
    return result;
}

char *str_replace_all(const char *haystack, const char *needle, const char *replacement)
{
    size_t needle_len, replacement_len, haystack_len;
    size_t occurrences;
    const char *p;
    char *result;
    char *out;

    needle_len = strlen(needle);
    if (needle_len == 0) {
        return xstrdup(haystack);
    }

    occurrences = 0;
    for (p = haystack; (p = strstr(p, needle)) != NULL; p += needle_len) {
        occurrences++;
    }

    haystack_len = strlen(haystack);
    replacement_len = strlen(replacement);
    result = malloc(haystack_len - occurrences * needle_len + occurrences * replacement_len + 1);

    out = result;
    p = haystack;
    while (1) {
        const char *hit = strstr(p, needle);
        size_t chunk;
        if (hit == NULL) {
            chunk = strlen(p);
            memcpy(out, p, chunk);
            out += chunk;
            break;
        }
        chunk = (size_t) (hit - p);
        memcpy(out, p, chunk);
        out += chunk;
        memcpy(out, replacement, replacement_len);
        out += replacement_len;
        p = hit + needle_len;
    }
    *out = '\0';
    return result;
}

void str_strip_inplace(char *s)
{
    size_t start, end, len;
    if (s == NULL) return;
    len = strlen(s);
    start = 0;
    while (start < len && isspace((unsigned char) s[start])) start++;
    end = len;
    while (end > start && isspace((unsigned char) s[end - 1])) end--;
    if (start > 0) {
        memmove(s, s + start, end - start);
    }
    s[end - start] = '\0';
}

char *str_basename(const char *path)
{
    const char *slash;
    const char *backslash;
    const char *last;

    slash = strrchr(path, '/');
    backslash = strrchr(path, '\\');
    last = slash;
    if (backslash != NULL && (last == NULL || backslash > last)) {
        last = backslash;
    }
    return xstrdup(last != NULL ? last + 1 : path);
}

char *int_to_str(int n)
{
    char buf[32];
    sprintf(buf, "%d", n);
    return xstrdup(buf);
}

char *read_entire_file(const char *path)
{
    FILE *f;
    long size;
    char *buf;
    size_t got;

    f = fopen(path, "rb");
    if (f == NULL) {
        return NULL;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    size = ftell(f);
    if (size < 0 || fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return NULL;
    }
    buf = malloc((size_t) size + 1);
    got = fread(buf, 1, (size_t) size, f);
    fclose(f);
    buf[got] = '\0';
    return buf;
}

int write_entire_file(const char *path, const char *contents)
{
    FILE *f = fopen(path, "wb");
    size_t len;
    size_t written;
    if (f == NULL) {
        return 0;
    }
    len = strlen(contents);
    written = fwrite(contents, 1, len, f);
    fclose(f);
    return written == len;
}

const char *json_get_str_field(const JsonValue *obj, const char *key)
{
    return json_get_string(json_object_get(obj, key));
}

/* ---- subscripted_digit / gensymbol ---- */

static const char *SUBSCRIPT_DIGIT[10] = {
    "\xE2\x82\x80", "\xE2\x82\x81", "\xE2\x82\x82", "\xE2\x82\x83", "\xE2\x82\x84",
    "\xE2\x82\x85", "\xE2\x82\x86", "\xE2\x82\x87", "\xE2\x82\x88", "\xE2\x82\x89"
}; /* UTF-8 for U+2080..U+2089 */

char *subscripted_digit(int n)
{
    /* the original hardcodes a 30-entry table of literal subscript
       strings for 0..29, where two-digit entries (e.g. "10") are just
       the two single-digit subscript glyphs next to each other.
       Composing the glyphs digit-by-digit produces byte-identical
       output without retyping 30 UTF-8 literals. */
    if (n >= 0 && n <= 29) {
        if (n < 10) {
            return xstrdup(SUBSCRIPT_DIGIT[n]);
        } else {
            return str_join(SUBSCRIPT_DIGIT[n / 10], SUBSCRIPT_DIGIT[n % 10], (char *) NULL);
        }
    } else {
        char *num_str = int_to_str(n);
        char *result = str_join("\xE2\x82\x8A" /* U+208A SUBSCRIPT PLUS */, num_str, (char *) NULL);
        free(num_str);
        return result;
    }
}

static int gensymbol_counter = 0;

char *gensymbol(const char *s)
{
    char *digit_str = subscripted_digit(gensymbol_counter);
    char *name_with_id = str_join(s, digit_str, (char *) NULL);
    free(digit_str);
    gensymbol_counter++;
    return name_with_id;
}

/* ================================================================ */
/* MeventQueue: singly linked FIFO (append at tail, dequeue at head)  */
/* ================================================================ */

void mevent_queue_init(MeventQueue *q)
{
    q->head = NULL;
    q->tail = NULL;
    q->count = 0;
}

void mevent_queue_append(MeventQueue *q, Mevent *m)
{
    MeventQueueNode *node = malloc(sizeof(MeventQueueNode));
    node->mev = m;
    node->next = NULL;
    if (q->tail == NULL) {
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->count++;
}

Mevent *mevent_queue_dequeue(MeventQueue *q)
{
    MeventQueueNode *node;
    Mevent *mev;
    if (q->head == NULL) {
        return NULL;
    }
    node = q->head;
    mev = node->mev;
    q->head = node->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    free(node);
    q->count--;
    return mev;
}

void mevent_queue_clear(MeventQueue *q)
{
    MeventQueueNode *node = q->head;
    while (node != NULL) {
        MeventQueueNode *next = node->next;
        free(node);
        node = next;
    }
    q->head = NULL;
    q->tail = NULL;
    q->count = 0;
}

/* ================================================================ */
/* EhDeque: doubly-endable queue of Eh* (visit_ordering)              */
/* ================================================================ */

void eh_deque_init(EhDeque *d)
{
    d->head = NULL;
    d->tail = NULL;
    d->count = 0;
}

void eh_deque_push_back(EhDeque *d, Eh *eh)
{
    EhDequeNode *node = malloc(sizeof(EhDequeNode));
    node->eh = eh;
    node->next = NULL;
    if (d->tail == NULL) {
        d->head = node;
        d->tail = node;
    } else {
        d->tail->next = node;
        d->tail = node;
    }
    d->count++;
}

void eh_deque_push_front(EhDeque *d, Eh *eh)
{
    EhDequeNode *node = malloc(sizeof(EhDequeNode));
    node->eh = eh;
    node->next = d->head;
    d->head = node;
    if (d->tail == NULL) {
        d->tail = node;
    }
    d->count++;
}

void eh_deque_clear(EhDeque *d)
{
    EhDequeNode *node = d->head;
    while (node != NULL) {
        EhDequeNode *next = node->next;
        free(node);
        node = next;
    }
    d->head = NULL;
    d->tail = NULL;
    d->count = 0;
}

Eh **eh_deque_to_array(const EhDeque *d, int *out_count)
{
    Eh **arr;
    EhDequeNode *node;
    int i;

    arr = malloc((d->count > 0 ? (size_t) d->count : 1) * sizeof(Eh *));
    i = 0;
    for (node = d->head; node != NULL; node = node->next) {
        arr[i++] = node->eh;
    }
    *out_count = d->count;
    return arr;
}

/* ================================================================ */
/* EhList: growable array of Eh* (.children)                          */
/* ================================================================ */

void eh_list_init(EhList *l)
{
    l->items = NULL;
    l->count = 0;
    l->capacity = 0;
}

void eh_list_append(EhList *l, Eh *eh)
{
    if (l->count == l->capacity) {
        l->capacity = (l->capacity == 0) ? 4 : l->capacity * 2;
        l->items = realloc(l->items, (size_t) l->capacity * sizeof(Eh *));
    }
    l->items[l->count++] = eh;
}

/* ================================================================ */
/* ConnectorList: growable array of Connector (.connections)          */
/* ================================================================ */

void connector_list_init(ConnectorList *l)
{
    l->items = NULL;
    l->count = 0;
    l->capacity = 0;
}

void connector_list_append(ConnectorList *l, Connector c)
{
    if (l->count == l->capacity) {
        l->capacity = (l->capacity == 0) ? 4 : l->capacity * 2;
        l->items = realloc(l->items, (size_t) l->capacity * sizeof(Connector));
    }
    l->items[l->count++] = c;
}

/* ================================================================ */
/* IdMap: growable int-id -> Eh* table, linear lookup                 */
/* ================================================================ */

void id_map_init(IdMap *m)
{
    m->items = NULL;
    m->count = 0;
    m->capacity = 0;
}

void id_map_put(IdMap *m, int id, Eh *eh)
{
    if (m->count == m->capacity) {
        m->capacity = (m->capacity == 0) ? 4 : m->capacity * 2;
        m->items = realloc(m->items, (size_t) m->capacity * sizeof(IdMapEntry));
    }
    m->items[m->count].id = id;
    m->items[m->count].eh = eh;
    m->count++;
}

Eh *id_map_get(const IdMap *m, int id)
{
    int i;
    for (i = 0; i < m->count; i++) {
        if (m->items[i].id == id) {
            return m->items[i].eh;
        }
    }
    return NULL;
}

void id_map_free(IdMap *m)
{
    free(m->items);
    m->items = NULL;
    m->count = 0;
    m->capacity = 0;
}

/* ================================================================ */
/* ComponentRegistry: growable name -> Template* table                */
/* ================================================================ */

ComponentRegistry *make_component_registry(void)
{
    ComponentRegistry *reg = malloc(sizeof(ComponentRegistry));
    reg->entries = NULL;
    reg->count = 0;
    reg->capacity = 0;
    return reg;
}

void component_registry_put(ComponentRegistry *reg, const char *key, Template *tmpl)
{
    int i;
    for (i = 0; i < reg->count; i++) {
        if (strcmp(reg->entries[i].key, key) == 0) {
            reg->entries[i].value = tmpl;
            return;
        }
    }
    if (reg->count == reg->capacity) {
        reg->capacity = (reg->capacity == 0) ? 8 : reg->capacity * 2;
        reg->entries = realloc(reg->entries, (size_t) reg->capacity * sizeof(TemplateEntry));
    }
    reg->entries[reg->count].key = xstrdup(key);
    reg->entries[reg->count].value = tmpl;
    reg->count++;
}

Template *component_registry_get(const ComponentRegistry *reg, const char *key)
{
    int i;
    if (reg == NULL) {
        return NULL;
    }
    for (i = 0; i < reg->count; i++) {
        if (strcmp(reg->entries[i].key, key) == 0) {
            return reg->entries[i].value;
        }
    }
    return NULL;
}

/* ================================================================ */
/* StrBuilder: tiny growable string buffer, used by deque_to_json     */
/* ================================================================ */

void sb_init(StrBuilder *sb)
{
    sb->capacity = 64;
    sb->buf = malloc((size_t) sb->capacity);
    sb->buf[0] = '\0';
    sb->len = 0;
}

static void sb_ensure(StrBuilder *sb, int extra)
{
    if (sb->len + extra + 1 > sb->capacity) {
        while (sb->len + extra + 1 > sb->capacity) {
            sb->capacity *= 2;
        }
        sb->buf = realloc(sb->buf, (size_t) sb->capacity);
    }
}

void sb_append(StrBuilder *sb, const char *text)
{
    int n = (int) strlen(text);
    sb_ensure(sb, n);
    memcpy(sb->buf + sb->len, text, (size_t) n + 1);
    sb->len += n;
}

void sb_append_escaped(StrBuilder *sb, const char *text)
{
    const unsigned char *p;
    for (p = (const unsigned char *) text; *p != '\0'; p++) {
        switch (*p) {
        case '"':  sb_append(sb, "\\\""); break;
        case '\\': sb_append(sb, "\\\\"); break;
        case '\n': sb_append(sb, "\\n");  break;
        case '\r': sb_append(sb, "\\r");  break;
        case '\t': sb_append(sb, "\\t");  break;
        default: {
            char one[2];
            one[0] = (char) *p;
            one[1] = '\0';
            sb_append(sb, one);
            break;
        }
        }
    }
}

char *sb_to_cstr(StrBuilder *sb)
{
    char *result = sb->buf;
    sb->buf = NULL;
    sb->len = 0;
    sb->capacity = 0;
    return result;
}
