/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
void* jit_instantiate (void* reg,void* owner,void* name,void* arg) {
                                                       /* line 3 */
    name_with_id = gensymbol ( name)                   /* line 4 */
    inst = make_leaf ( name_with_id, owner, NULL, arg, handle_jit, NULL)/* line 5 */
    firstc =  (*name) [ 1]                             /* line 6 */
    if ( firstc!= "$"):                                /* line 7 */
        /*  probes get to go to the front of the line  *//* line 8 */
        (*inst).special =  True;                       /* line 9 *//* line 10 */
    return ( inst)                                     /* line 11 *//* line 12 *//* line 13 */}

void* handle_jit (void* eh,void* mev) {
                                                       /* line 14 */
    s =   (*eh).arg                                    /* line 15 */
    firstc =  (*s) [ 1]                                /* line 16 */
    if  firstc ==  "$":                                /* line 17 */
        shell_out_handler ( eh,    s[1:] [1:] [1:] , mev)/* line 18 */
    elif  firstc ==  "?":                              /* line 19 */
        probe_handler ( eh,  s[1:] , mev)              /* line 20 */
    else:                                              /* line 21 */
        /*  just a string, send it out  */             /* line 22 */
        send ( eh, "",  s[1:] , mev)                   /* line 23 *//* line 24 *//* line 25 *//* line 26 */}

void* probe_handler (void* eh,void* tag,void* mev) {
                                                       /* line 27 */
    static ticktime                                    /* line 28 */
    s =    (*mev).payload.v                            /* line 29 */
    external live_update ( "Info",  str( "  @") +  str(str ( ticktime)) +  str( "  ") +  str( "probe ") +  str(  (*eh).name) +  str( ": ") + str ( s)      )/* line 37 *//* line 38 *//* line 39 */}

void* shell_out_handler (void* eh,void* cmd,void* mev) {
                                                       /* line 40 */
    s =    (*mev).payload.v                            /* line 41 */
    ret =  NULL                                        /* line 42 */
    rc =  NULL                                         /* line 43 */
    stdout =  NULL                                     /* line 44 */
    stderr =  NULL                                     /* line 45 */
    command =  cmd                                     /* line 46 */
    pbpRoot = os.getenv('PBP', '<none>')               /* line 47 */
    if  pbpRoot!= "":                                  /* line 48 */
        command = re.sub ( "_/",  str( pbpRoot) +  "/" ,  command)/* line 51 */;/* line 52 */
    if ( ("PBPSHELLUT" in os.environ) ):               /* line 53 */
        external print ( str( "- --- shell-out: ") +  command , file=sys.stderr)/* line 54 */
        external                                       /* line 55 *//* line 56 */
    external
    try:
        with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', delete=False) as tmp:
            tmp.write( s)
            tmp_path = tmp.name
        try:
            with open(tmp_path, 'r') as stdin_file:
                ret = subprocess.run(
                shlex.split( command),
                stdin=stdin_file,
                text=True,
                capture_output=True
                )
        finally:
            os.unlink(tmp_path)
        rc = ret.returncode
        stdout = ret.stdout.strip()
        stderr = ret.stderr.strip()
    except Exception as e:
        rc = 1
        stdout = ''
        stderr = str(e)
                                                       /* line 57 */
    if  rc ==  0:                                      /* line 58 */
        send ( eh, "", str( stdout) +  stderr , mev)   /* line 59 */
    else:                                              /* line 60 */
        send ( eh, "✗", str( stdout) +  stderr , mev)  /* line 61 *//* line 62 *//* line 63 *//* line 64 */}
