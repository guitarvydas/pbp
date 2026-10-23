typedef char InternedStr;

/**************/
/* List       */
/**************/

typedef void (*FreclaimListCell) (struct s_List_cell*);
typedef union {
  kv* kvptr;
  obj* objptr;
} KVorObj;

typedef struct s_List_cell {
  KVorObj item;
  FreclaimListCell reclaim;
  struct s_List_cell *next;
} List_cell;

/**************/
/* objects    */
/**************/
typedef void (*FreclaimObj) (struct s_obj_cell*);

typedef s_obj_cell {
  struct s_kv *kv;
  FreclaimObj reclaim;
  struct s_obj_cell *next;
} Object_cell;

/******************/
/* Key Value (kv) */
/******************/
typedef union {
    int i;
    Str s;
    Obj *o;
} IntOrStrOrObj;

typedef void (*FreclaimKV) (struct s_kv*);

typedef struct s_kv {
  InternedStr *key;
  IntOrStrOrObj *value;
  FreclaimKV reclaim;
} kv;



/****************/
/* input buffer */
/****************/
/**************/
/* Result Stack */
/**************/
extern List* result_stack;
extern void push_kv_int (interned* key, int val);
extern void push_kv_string (interned* key, Str* s);
extern void fold_result (int n);

void mark (void);
Bool fail (void);
Bool pass (void);
Bool parse_string (char*);
Bool parse_keyword (InternedStr*);
Bool parse_any_string (void);
