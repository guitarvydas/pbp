/* line 1 */
function load_error (s) {                              /* line 2 *//* line 3 */
    console.error ( s);                                /* line 4 */
                                                       /* line 5 */
    load_errors =  true;                               /* line 6 *//* line 7 *//* line 8 */
}

function runtime_error (s) {                           /* line 9 *//* line 10 */
    console.error ( s);                                /* line 11 */
    process.exit (1)                                   /* line 12 */
    runtime_errors =  true;                            /* line 13 *//* line 14 *//* line 15 */
}
                                                       /* line 16 */
function initialize_component_palette_from_files (diagram_source_files) {/* line 17 */
    let  reg = make_component_registry ();             /* line 18 */
    for (let diagram_source of  diagram_source_files) {/* line 19 */
      let all_containers_within_single_file = lnet2internal_from_file ( diagram_source)/* line 20 */;
      for (let container of  all_containers_within_single_file) {/* line 21 */
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 22 *//* line 23 */
      }                                                /* line 24 */
    }
    initialize_stock_components ( reg)                 /* line 25 */
    return  reg;                                       /* line 26 *//* line 27 *//* line 28 */
}

function initialize_component_palette_from_string (lnet) {/* line 29 */
    let  reg = make_component_registry ();             /* line 30 */
    let all_containers = lnet2internal_from_string ( lnet)/* line 31 */;
    for (let container of  all_containers) {           /* line 32 */
      register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 33 *//* line 34 */
    }
    initialize_stock_components ( reg)                 /* line 35 */
    return  reg;                                       /* line 36 *//* line 37 */
}

function initialize_from_files (diagram_names) {       /* line 38 */
    let arg =  null;                                   /* line 39 */
    let palette = initialize_component_palette_from_files ( diagram_names)/* line 40 */;
    return [ palette,[ diagram_names, arg]];           /* line 41 *//* line 42 *//* line 43 */
}

function initialize_from_string () {                   /* line 44 */
    let arg =  null;                                   /* line 45 */
    let palette = initialize_component_palette_from_string ();/* line 46 */
    return [ palette,[ null, arg]];                    /* line 47 *//* line 48 *//* line 49 */
}

function start (arg,part_name,palette,env) {           /* line 50 */
    let part = start_bare ( part_name, palette, env)   /* line 51 */;
    inject ( part, "", arg)                            /* line 52 */
    finalize ( part)                                   /* line 53 *//* line 54 *//* line 55 */
}

function start_bare (part_name,palette,env) {          /* line 56 */
    let diagram_names =  env [ 0];                     /* line 57 */
    /*  get entrypoint container */                    /* line 58 */
    let  part = get_component_instance ( palette, part_name, null)/* line 59 */;
    if ( null ==  part) {                              /* line 60 */
      load_error ( ( "Couldn;t find container with page name /".toString ()+  ( part_name.toString ()+  ( "/ in files ".toString ()+  (`${ diagram_names}`.toString ()+  " (check tab names, or disable compression?)".toString ()) .toString ()) .toString ()) .toString ()) )/* line 64 *//* line 65 */
    }
    return  part;                                      /* line 66 *//* line 67 *//* line 68 */
}

function inject (part,port,payload) {                  /* line 69 */
    if ((!  load_errors)) {                            /* line 70 */
      let  d =  new Datum ();                          /* line 71 */;
      d.v =  payload;                                  /* line 72 */
      d.clone =  function () {return obj_clone ( d)    /* line 73 */;};
      d.reclaim =  null;                               /* line 74 */
      let  mev = make_mevent ( port, d)                /* line 75 */;
      inject_mevent ( part, mev)                       /* line 76 */
    }
    else {                                             /* line 77 */
      process.exit (1)                                 /* line 78 *//* line 79 */
    }                                                  /* line 80 *//* line 81 */
}

function finalize (part) {                             /* line 82 */
    console.log (JSON.stringify ( part.outq.map(item => ({ [item.port]: item.datum.v })), null, 2));/* line 83 *//* line 84 *//* line 85 */
}

function new_datum_bang () {                           /* line 86 */
    let  d =  new Datum ();                            /* line 87 */;
    d.v =  "!";                                        /* line 88 */
    d.clone =  function () {return obj_clone ( d)      /* line 89 */;};
    d.reclaim =  null;                                 /* line 90 */
    return  d                                          /* line 91 *//* line 92 */;
}
