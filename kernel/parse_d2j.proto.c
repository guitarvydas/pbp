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

Bool parse_keyword (char* key) {
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

// cons (item, reclaimer for item, list or NULL);

Bool parse_down_direction (void) {
  mark ();
  if (parse_string ("{")) {
    List* options = fresh_List ();
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








Bool parse_down_dir (void) {
  Bool done [4] = {FALSE, FALSE, FALSE, FALSE];
  if (!parse_string ("{")) { goto fin; }
  if (!done[0]) {
    if (!parse_dirDown ()) { goto fin; }
    done[0] = TRUE;
    if (all_done (done)) { goto fin; }
  } else if (!done[1]) {
    if (!parse_sourcePort ()) { goto fin; }
    done[1] = TRUE;
    if (all_done (done)) { goto fin; }
  } else if (!done[2]) {
    if (!parse_targetPort ()) { goto fin; }
    done[2] = TRUE;
    if (all_done (done)) { goto fin; }
  } else if (!done[3]) {
    if (!parse_target ()) { goto fin; }
    done[3] = TRUE;
    if (all_done (done)) { goto fin; }
  } else {
    return FALSE;
  }
    fin:
    if (done[0] && done[1] && done[2] && done[3]) {
      if (!parse_string ("}")) { 
	return FALSE;
      } else {
	return TRUE;
      }
    } else {
      return FALSE;
    }
}

