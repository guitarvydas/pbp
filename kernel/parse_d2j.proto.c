

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
} obj_cell;

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
char buff[???];
int buffx = 0;
int markx = 0;

void mark (void) {
  markx = buffx;
}

void undo (void) {
  buffx = markx;
}

void acceptc (void) {
  buffx += 1;
}

Bool eos(void) {
  /* TRUE if no more characters remain, else FALSE */
  if (buffx < len(buff)) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**************/
/* Result Stack */
/**************/
List* result_stack = NULL;

void push_kv_int (interned* key, int val) {
  result_stack = cons (key, NULL, result_stack);
}

void push_kv_string (interned* key, Str* s) {
  result_stack = cons (key, &reclaim_Str, result_stack);
}

void fold_result (int n) {
  /* fold all top n items on result stack into a single kv object on the result stack */
  /* a kv object is just a list, order doesn't matter */
  obj_cell* nobjlist = NULL;
  int i = n;
  while (i > 0) {
    /* return stack consists of pointers to kvs, collect them all (n of them) into an obj, remove them from the stack and
       put the new obj onto the stack */
    List_cell* r_cell = result_stack;
    kv* keyvalueptr = listcell.item.kvptr;
    obj_cell* nobj = fresh_obj_cell ();
    (*nobj).kv = keyvalueptr;
    if (r_cell != NULL) {
      result_stack = r_cell.next;
      r_cell.reclaim (r_cell);
    }
    i -= 1;
  }
}


/**************/
/* Parser     */
/**************/
Bool fail (void) {
  undo ();
  return FALSE;
}

Bool pass (void) {
  return TRUE;
}

Bool parse_string (char* s) {
  int sx = 0;
  mark ();
  while (!eos() && bx < len(s)) {
    if (buff[bx] != s[sx]) {
      return fail ();
    }
    acceptc ();
    sx += 1;
  }
  return pass ();
}

Bool parse_keyword (interned* key) {
  mark ();
  if (parse_string("\"")) {
    if (parse_string (key)) {
      if (parse_string("\"")) {
        return pass ();
      }
    }
  }
  return fail ();
}

Bool parse_any_string (void) {
  mark ();
  if (parse_string("\"")) {
    while (!parse_string("\"")) {
      acceptc ();
    }
    if (parse_string("\"")) {
      return pass ();
    }
  }
  return fail ();
}

// DI: cons (item, reclaimer for item, list or NULL);

Bool parse_down_direction (void) {
  mark ();
  if (parse_string ("{")) {
    List* options = fresh_List_cell ();
    options = cons (&parse_dirDown, NULL, options);
    options = cons (&parse_sourcePort, NULL, options);
    options = cons (&parse_targetPort, NULL, options);
    options = cons (&parse_target, NULL, options);
    if (parse_each_item (options)) {
      if (parse_string ("}")) {
        list_reclaim (options);
        fold_result (4);
        return pass ();
      }
    }
    list_reclaim (options);
  }
  return fail ();
}

Bool parse_across_direction (void) {
  mark ();
  if (parse_string ("{")) {
    List* options = fresh_List_cell ();
    options = cons (&parse_dirAcross, NULL, options);
    options = cons (&parse_source, NULL, options);
    options = cons (&parse_sourcePort, NULL, options);
    options = cons (&parse_target, NULL, options);
    options = cons (&parse_targetPort, NULL, options);
    if (parse_each_item (options)) {
      if (parse_string ("}")) {
        list_reclaim (options);
        fold_result (5);
        return pass ();
      }
    }
    list_reclaim (options);
  }
  return fail ();
}

Bool parse_up_direction (void) {
  mark ();
  if (parse_string ("{")) {
    List* options = fresh_List_cell ();
    options = cons (&parse_dirUp, NULL, options);
    options = cons (&parse_source, NULL, options);
    options = cons (&parse_sourcePort, NULL, options);
    options = cons (&parse_targetPort, NULL, options);
    if (parse_each_item (options)) {
      if (parse_string ("}")) {
        list_reclaim (options);
        fold_result (4);
        return pass ();
      }
    }
    list_reclaim (options);
  }
  return fail ();
}

Bool parse_through_direction (void) {
  mark ();
  if (parse_string ("{")) {
    List* options = fresh_List_cell ();
    options = cons (&parse_dirThrough, NULL, options);
    options = cons (&parse_sourcePort, NULL, options);
    options = cons (&parse_targetPort, NULL, options);
    if (parse_each_item (options)) {
      if (parse_string ("}")) {
        list_reclaim (options);
        fold_result (3);
        return pass ();
      }
    }
    list_reclaim (options);
  }
  return fail ();
}


Bool parse_dirDown (void) {
  mark ();
  {
    if (parse_string ("{")) {
      if (parse_string ("dir")) {
        if (parse_string (":")) {
          if (parse_int (0)) {
            push_kv_int ("dir", 0);
            return pass ();
          }
        }       
      }
    }
  }
  return fail ();
}

Bool parse_dirUp (void) {
  mark ();
  {
    if (parse_string ("{")) {
      if (parse_string ("dir")) {
        if (parse_string (":")) {
          if (parse_int (1)) {
            push_kv_int ("dir", 1);
            return pass ();
          }
        }       
      }
    }
  }
  return fail ();
}

Bool parse_dirUp (void) {
  mark ();
  {
    if (parse_string ("{")) {
      if (parse_string ("dir")) {
        if (parse_string (":")) {
          if (parse_int (2)) {
            push_kv_int ("dir", 2);
            return pass ();
          }
        }       
      }
    }
  }
  return fail ();
}

Bool parse_dirUp (void) {
  mark ();
  {
    if (parse_string ("{")) {
      if (parse_string ("dir")) {
        if (parse_string (":")) {
          if (parse_int (3)) {
            push_kv_int ("dir", 3);
            return pass ();
          }
        }       
      }
    }
  }
  return fail ();
}


