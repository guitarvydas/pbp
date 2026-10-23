#include "parse_d2j.h"

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
        reclaim_List (options);
        fold_result (4);
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
