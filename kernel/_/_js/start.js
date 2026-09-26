let  load_errors =  false;                             /* line 1 */
let  runtime_errors =  false;                          /* line 2 */
let  ticktime =  0;                                    /* line 3 *//* line 4 */
function load_error (s) {                              /* line 5 *//* line 6 */
    console.error ( s);                                /* line 7 */
                                                       /* line 8 */
    load_errors =  true;                               /* line 9 *//* line 10 *//* line 11 */
}

function runtime_error (s) {                           /* line 12 *//* line 13 */
    console.error ( s);                                /* line 14 */
    process.exit (1)                                   /* line 15 */
    runtime_errors =  true;                            /* line 16 *//* line 17 *//* line 18 */
}
                                                       /* line 19 */
function initialize_component_palette_from_files (diagram_source_files) {/* line 20 */
    let  reg = make_component_registry ();             /* line 21 */
    for (let diagram_source of  diagram_source_files) {/* line 22 */
      let all_containers_within_single_file = lnet2internal_from_file ( diagram_source)/* line 23 */;
      for (let container of  all_containers_within_single_file) {/* line 24 */
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 25 *//* line 26 */
      }                                                /* line 27 */
    }
    initialize_stock_components ( reg)                 /* line 28 */
    return  reg;                                       /* line 29 *//* line 30 *//* line 31 */
}

function initialize_component_palette_from_string (lnet) {/* line 32 */
    let  reg = make_component_registry ();             /* line 33 */
    let all_containers = lnet2internal_from_string ( lnet)/* line 34 */;
    for (let container of  all_containers) {           /* line 35 */
      register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 36 *//* line 37 */
    }
    initialize_stock_components ( reg)                 /* line 38 */
    return  reg;                                       /* line 39 *//* line 40 */
}

function initialize_from_files (diagram_names) {       /* line 41 */
    let arg =  null;                                   /* line 42 */
    let palette = initialize_component_palette_from_files ( diagram_names)/* line 43 */;
    return [ palette,[ diagram_names, arg]];           /* line 44 *//* line 45 *//* line 46 */
}

function initialize_from_string () {                   /* line 47 */
    let arg =  null;                                   /* line 48 */
    let palette = initialize_component_palette_from_string ();/* line 49 */
    return [ palette,[ null, arg]];                    /* line 50 *//* line 51 *//* line 52 */
}

function start (arg,part_name,palette,env) {           /* line 53 */
    let part = start_bare ( part_name, palette, env)   /* line 54 */;
    inject ( part, "", arg)                            /* line 55 */
    finalize ( part)                                   /* line 56 *//* line 57 *//* line 58 */
}

function start_bare (part_name,palette,env) {          /* line 59 */
    let diagram_names =  env [ 0];                     /* line 60 */
    /*  get entrypoint container */                    /* line 61 */
    let  part = get_component_instance ( palette, part_name, null)/* line 62 */;
    if ( null ==  part) {                              /* line 63 */
      load_error ( ( "Couldn;t find container with page name /".toString ()+  ( part_name.toString ()+  ( "/ in files ".toString ()+  (`${ diagram_names}`.toString ()+  " (check tab names, or disable compression?)".toString ()) .toString ()) .toString ()) .toString ()) )/* line 67 *//* line 68 */
    }
    return  part;                                      /* line 69 *//* line 70 *//* line 71 */
}

function inject (part,port,payload) {                  /* line 72 */
    if ((!  load_errors)) {                            /* line 73 */
      let  d =  new Datum ();                          /* line 74 */;
      d.v =  payload;                                  /* line 75 */
      d.clone =  function () {return obj_clone ( d)    /* line 76 */;};
      d.reclaim =  null;                               /* line 77 */
      let  mev = make_mevent ( port, d)                /* line 78 */;
      inject_mevent ( part, mev)                       /* line 79 */
    }
    else {                                             /* line 80 */
      process.exit (1)                                 /* line 81 *//* line 82 */
    }                                                  /* line 83 *//* line 84 */
}

function finalize (part) {                             /* line 85 */
    console.log (JSON.stringify ( part.outq.map(item => ({ [item.port]: item.datum.v })), null, 2));/* line 86 *//* line 87 *//* line 88 */
}

function new_datum_bang () {                           /* line 89 */
    let  d =  new Datum ();                            /* line 90 */;
    d.v =  "!";                                        /* line 91 */
    d.clone =  function () {return obj_clone ( d)      /* line 92 */;};
    d.reclaim =  null;                                 /* line 93 */
    return  d                                          /* line 94 *//* line 95 */;
}
