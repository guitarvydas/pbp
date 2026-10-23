#include "parse_d2j.h"

/**************/
/* List       */
/**************/

/**************/
/* objects    */
/**************/
/******************/
/* Key Value (kv) */
/******************/


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
// DI: cons (item, reclaimer for item, list or NULL);

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
  
  /* initialization (0.1) */
  Object_cell* nobj = NULL;

  while (result_stack != NULL) {
    /* fold loop step 1 */
    /* create a new (empty) object */
    Object_cell* temp_obj = fresh_Object_cell ();
    temp_obj->reclaim = &reclaim_object;
    temp_obj->next = NULL;
    
    /* fold loop step 2 */
    /* link the .kv field of *temp_obj to the .item field of result_stack */
    temp_obj->kv = result_stack.item.kvptr;

    /* fold loop step 3 */
    /* bump result_stack to the next cell
       but hang onto the removed List cell using variable "temp" */
    List_cell* temp = result_stack;
    result_stack = result_stack->next;

    /* fold loop step 4 */
    /* NULL out the .item and .next fields of *temp */
    temp->item = NULL;
    temp->next = NULL;

    /* fold loop step 5 */
    /* reclaim the list cell (using Freclaim_List_cell), then NULL out temp
       (the kv is not reclaimed because .kv is NULL) */
    temp->reclaim (temp->item);
    reclaim_List_cell (temp);
    temp = NULL;

    /* fold loop step 6 */
    /* push the new temp_obj onto the front of result nobj */
    temp_obj->next = nobj;
    nobj = temp_obj;
    temp_obj = NULL;

  } /* end fold loop */

  /* fold finalization 1 */
  /* result_stack is empty */

  /* fold finalization 2 */
  /* create fresh list cell assigned on result_stack */
  result_stack = fresh_List_cell ();
  result_stack->next = NULL;
  result_stack->reclaim = &reclaim_List_cell;
  temp_obj->reclaim = &reclaim_Object_cell;

  /* link top list .item to nobj */
  result_stack->item.objptr = nobj;
  nobj = NULL;
  
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
