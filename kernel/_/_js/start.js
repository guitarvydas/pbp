let  load_errors =  true;                              /* line 1 */
let  runtime_errors =  true;                           /* line 2 *//* line 3 */
function load_error (s) {                              /* line 4 *//* line 5 */
    console.error ( s);                                /* line 6 */
                                                       /* line 7 */
    load_errors =  true;                               /* line 8 *//* line 9 *//* line 10 */
}

function runtime_error (s) {                           /* line 11 *//* line 12 */
    console.error ( s);                                /* line 13 */
    process.exit (1)                                   /* line 14 */
    runtime_errors =  true;                            /* line 15 *//* line 16 *//* line 17 */
}
                                                       /* line 18 */
function initialize_component_palette_from_files (diagram_source_files) {/* line 19 */
    let  reg = make_component_registry ();             /* line 20 */
    for (let diagram_source of  diagram_source_files) {/* line 21 */
      let all_containers_within_single_file = lnet2internal_from_file ( diagram_source)/* line 22 */;
      for (let container of  all_containers_within_single_file) {/* line 23 */
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 24 *//* line 25 */
      }                                                /* line 26 */
    }
    initialize_stock_components ( reg)                 /* line 27 */
    return  reg;                                       /* line 28 *//* line 29 *//* line 30 */
}

function initialize_component_palette_from_string (lnet) {/* line 31 */
    let  reg = make_component_registry ();             /* line 32 */
    let all_containers = lnet2internal_from_string ( lnet)/* line 33 */;
    for (let container of  all_containers) {           /* line 34 */
      register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 35 *//* line 36 */
    }
    initialize_stock_components ( reg)                 /* line 37 */
    return  reg;                                       /* line 38 *//* line 39 */
}

function initialize_from_files (diagram_names) {       /* line 40 */
    let arg =  null;                                   /* line 41 */
    let palette = initialize_component_palette_from_files ( diagram_names)/* line 42 */;
    return [ palette,[ diagram_names, arg]];           /* line 43 *//* line 44 *//* line 45 */
}

function initialize_from_string () {                   /* line 46 */
    let arg =  null;                                   /* line 47 */
    let palette = initialize_component_palette_from_string ();/* line 48 */
    return [ palette,[ null, arg]];                    /* line 49 *//* line 50 *//* line 51 */
}

function start (arg,part_name,palette,env) {           /* line 52 */
    let part = start_bare ( part_name, palette, env)   /* line 53 */;
    inject ( part, "", arg)                            /* line 54 */
    finalize ( part)                                   /* line 55 *//* line 56 *//* line 57 */
}

function start_bare (part_name,palette,env) {          /* line 58 */
    let diagram_names =  env [ 0];                     /* line 59 */
    /*  get entrypoint container */                    /* line 60 */
    let  part = get_component_instance ( palette, part_name, null)/* line 61 */;
    if ( null ==  part) {                              /* line 62 */
      load_error ( ( "Couldn;t find container with page name /".toString ()+  ( part_name.toString ()+  ( "/ in files ".toString ()+  (`${ diagram_names}`.toString ()+  " (check tab names, or disable compression?)".toString ()) .toString ()) .toString ()) .toString ()) )/* line 66 *//* line 67 */
    }
    return  part;                                      /* line 68 *//* line 69 *//* line 70 */
}

function inject (part,port,payload) {                  /* line 71 */
    if ((!  load_errors)) {                            /* line 72 */
      let  d =  new Datum ();                          /* line 73 */;
      d.v =  payload;                                  /* line 74 */
      d.clone =  function () {return obj_clone ( d)    /* line 75 */;};
      d.reclaim =  null;                               /* line 76 */
      let  mev = make_mevent ( port, d)                /* line 77 */;
      inject_mevent ( part, mev)                       /* line 78 */
    }
    else {                                             /* line 79 */
      process.exit (1)                                 /* line 80 *//* line 81 */
    }                                                  /* line 82 *//* line 83 */
}

function finalize (part) {                             /* line 84 */
    console.log (JSON.stringify ( part.outq.map(item => ({ [item.port]: item.datum.v })), null, 2));/* line 85 *//* line 86 *//* line 87 */
}

function new_datum_bang () {                           /* line 88 */
    let  d =  new Datum ();                            /* line 89 */;
    d.v =  "!";                                        /* line 90 */
    d.clone =  function () {return obj_clone ( d)      /* line 91 */;};
    d.reclaim =  null;                                 /* line 92 */
    return  d                                          /* line 93 *//* line 94 */;
}
