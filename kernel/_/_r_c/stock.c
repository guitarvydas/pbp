void clone_string (s) {
                                                       /* line 1 */
    return ( s)                                        /* line 2 *//* line 3 *//* line 4 */}
                                                       /* line 5 */
void trash_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 6 */
    name_with_id = gensymbol ( "trash")                /* line 7 */
    return (make_leaf ( name_with_id, owner, NULL, "", trash_handler, NULL)/* line 8 */)/* line 9 *//* line 10 */}

void trash_handler (eh,mev) {
                                                       /* line 11 */
    /*  to appease dumped_on_floor checker */          /* line 12 */
                                                       /* line 13 *//* line 14 */}

typedef struct _TwoMevents {
                                                       /* line 15 */
    firstmev;                                          /* line 16 */
    secondmev;                                         /* line 17 *//* line 18 */
} TwoMevents;
TwoMevents fresh_TwoMevents () {
    TwoMevents *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->firstmev =  NULL;                            /* line 16 */
    self->secondmev =  NULL;                           /* line 17 *//* line 18 */
    return self;
}
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
typedef struct _Deracer_Instance_Data {
                                                       /* line 21 */
    state;                                             /* line 22 */
    buffer;                                            /* line 23 *//* line 24 */
} Deracer_Instance_Data;
Deracer_Instance_Data fresh_Deracer_Instance_Data () {
    Deracer_Instance_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->state =  NULL;                               /* line 22 */
    self->buffer =  NULL;                              /* line 23 *//* line 24 */
    return self;
}
                                                       /* line 25 */
void reclaim_Buffers_from_heap (inst) {
                                                       /* line 26 */
                                                       /* line 27 *//* line 28 *//* line 29 */}

void deracer_reset_handler (eh) {
                                                       /* line 30 */
    inst =  eh.instance_data                           /* line 31 */
    inst.state =  "idle"                               /* line 32 */
    inst.buffer =  TwoMevents ()                       /* line 33 */;;/* line 34 *//* line 35 */}

void deracer_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 36 */
    name_with_id = gensymbol ( "deracer")              /* line 37 */
    inst =  Deracer_Instance_Data ()                   /* line 38 */
    inst.state =  "idle"                               /* line 39 */
    inst.buffer =  TwoMevents ()                       /* line 40 */
    eh = make_leaf ( name_with_id, owner, inst, "", deracer_handler, deracer_reset_handler)/* line 41 */
    return ( eh)                                       /* line 42 */;;/* line 43 *//* line 44 */}

void send_firstmev_then_secondmev (eh,inst) {
                                                       /* line 45 */
    forward ( eh, "1", inst.buffer.firstmev)           /* line 46 */
    forward ( eh, "2", inst.buffer.secondmev)          /* line 47 */
    reclaim_Buffers_from_heap ( inst)                  /* line 48 *//* line 49 *//* line 50 */}

void deracer_handler (eh,mev) {
                                                       /* line 51 */
    inst =  eh.instance_data                           /* line 52 */
    if  inst.state ==  "idle":                         /* line 53 */
        if  "1" ==  mev.port:                          /* line 54 */
            inst.buffer.firstmev =  mev                /* line 55 */
            inst.state =  "waitingForSecondmev";       /* line 56 */;
        elif  "2" ==  mev.port:                        /* line 57 */
            inst.buffer.secondmev =  mev               /* line 58 */
            inst.state =  "waitingForFirstmev";        /* line 59 */;
        else:                                          /* line 60 */
            runtime_error ( str( "bad mev.port (case A) for deracer ") +  mev.port )/* line 61 *//* line 62 */
    elif  inst.state ==  "waitingForFirstmev":         /* line 63 */
        if  "1" ==  mev.port:                          /* line 64 */
            inst.buffer.firstmev =  mev                /* line 65 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 66 */
            inst.state =  "idle";                      /* line 67 */;
        else:                                          /* line 68 */
            runtime_error ( str( "deracer: waiting for 1 but got [") +  str( mev.port) +  "] (case B)"  )/* line 69 *//* line 70 */
    elif  inst.state ==  "waitingForSecondmev":        /* line 71 */
        if  "2" ==  mev.port:                          /* line 72 */
            inst.buffer.secondmev =  mev               /* line 73 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 74 */
            inst.state =  "idle";                      /* line 75 */;
        else:                                          /* line 76 */
            runtime_error ( str( "deracer: waiting for 2 but got [") +  str( mev.port) +  "] (case C)"  )/* line 77 *//* line 78 */
    else:                                              /* line 79 */
        runtime_error ( "bad state for deracer {eh.state}")/* line 80 *//* line 81 *//* line 82 *//* line 83 */}

void low_level_read_text_file_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 84 */
    name_with_id = gensymbol ( "Low Level Read Text File")/* line 85 */
    return (make_leaf ( name_with_id, owner, NULL, "", low_level_read_text_file_handler, NULL)/* line 86 */)/* line 87 *//* line 88 */}

void low_level_read_text_file_handler (eh,mev) {
                                                       /* line 89 */
    fname =  mev.payload.v                             /* line 90 */
    external
    try:
        f = open (fname)
    except Exception as e:
        f = None
    if f != None:
        data = f.read ()
        if data!= None:
            send (eh, "", data, mev)
        else:
            send (eh, "✗", f"read error on file '{fname}'", mev)
        f.close ()
    else:
        send (eh, "✗", f"open error on file '{fname}'", mev)
                                                       /* line 91 *//* line 92 *//* line 93 */}

void ensure_string_datum_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 94 */
    name_with_id = gensymbol ( "Ensure String Datum")  /* line 95 */
    return (make_leaf ( name_with_id, owner, NULL, "", ensure_string_datum_handler, NULL)/* line 96 */)/* line 97 *//* line 98 */}

void ensure_string_datum_handler (eh,mev) {
                                                       /* line 99 */
    if  "string" ==  mev.payload.kind ():              /* line 100 */
        forward ( eh, "", mev)                         /* line 101 */
    else:                                              /* line 102 */
        emev =  str( "*** ensure: type error (expected a string payload) but got ") +  mev.payload /* line 103 */
        send ( eh, "✗", emev, mev)                     /* line 104 *//* line 105 *//* line 106 *//* line 107 */}

typedef struct _Syncfilewrite_Data {
                                                       /* line 108 */
    filename;                                          /* line 109 *//* line 110 */
} Syncfilewrite_Data;
Syncfilewrite_Data fresh_Syncfilewrite_Data () {
    Syncfilewrite_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->filename =  "";                              /* line 109 *//* line 110 */
    return self;
}
                                                       /* line 111 */
void syncfilewrite_reset_handler (eh) {
                                                       /* line 112 */
    eh.instance_data =  Syncfilewrite_Data ()          /* line 113 */;/* line 114 *//* line 115 */}

/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
void syncfilewrite_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 117 */
    name_with_id = gensymbol ( "syncfilewrite")        /* line 118 */
    inst =  Syncfilewrite_Data ()                      /* line 119 */
    return (make_leaf ( name_with_id, owner, inst, "", syncfilewrite_handler, syncfilewrite_reset_handler)/* line 120 */)/* line 121 *//* line 122 */}

void syncfilewrite_handler (eh,mev) {
                                                       /* line 123 */
    inst =  eh.instance_data                           /* line 124 */
    if  "filename" ==  mev.port:                       /* line 125 */
        inst.filename =  mev.payload.v;                /* line 126 */
    elif  "input" ==  mev.port:                        /* line 127 */
        contents =  mev.payload.v                      /* line 128 */
        f = open ( inst.filename, "w")                 /* line 129 */
        if  f!= NULL:                                  /* line 130 */
            f.write ( mev.payload.v)                   /* line 131 */
            f.close ()                                 /* line 132 */
            send ( eh, "done",new_datum_bang (), mev)  /* line 133 */
        else:                                          /* line 134 */
            send ( eh, "✗", str( "open error on file ") +  inst.filename , mev)/* line 135 *//* line 136 *//* line 137 *//* line 138 *//* line 139 */}

typedef struct _StringConcat_Instance_Data {
                                                       /* line 140 */
    buffer1;                                           /* line 141 */
    buffer2;                                           /* line 142 *//* line 143 */
} StringConcat_Instance_Data;
StringConcat_Instance_Data fresh_StringConcat_Instance_Data () {
    StringConcat_Instance_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->buffer1 =  NULL;                             /* line 141 */
    self->buffer2 =  NULL;                             /* line 142 *//* line 143 */
    return self;
}
                                                       /* line 144 */
void stringconcat_reset_handler (eh) {
                                                       /* line 145 */
    inst =  eh.instance_data                           /* line 146 */
    inst.buffer1 =  NULL                               /* line 147 */
    inst.buffer2 =  NULL;                              /* line 148 */;/* line 149 *//* line 150 */}

void stringconcat_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 151 */
    name_with_id = gensymbol ( "stringconcat")         /* line 152 */
    instp =  StringConcat_Instance_Data ()             /* line 153 */
    return (make_leaf ( name_with_id, owner, instp, "", stringconcat_handler, stringconcat_reset_handler)/* line 154 */)/* line 155 *//* line 156 */}

void stringconcat_handler (eh,mev) {
                                                       /* line 157 */
    inst =  eh.instance_data                           /* line 158 */
    if  "1" ==  mev.port:                              /* line 159 */
        inst.buffer1 = clone_string ( mev.payload.v)   /* line 160 */
        maybe_stringconcat ( eh, inst, mev)            /* line 161 */;
    elif  "2" ==  mev.port:                            /* line 162 */
        inst.buffer2 = clone_string ( mev.payload.v)   /* line 163 */
        maybe_stringconcat ( eh, inst, mev)            /* line 164 */;
    elif  "reset" ==  mev.port:                        /* line 165 */
        inst.buffer1 =  NULL                           /* line 166 */
        inst.buffer2 =  NULL;                          /* line 167 */;
    else:                                              /* line 168 */
        runtime_error ( str( "bad mev.port for stringconcat: ") +  mev.port )/* line 169 *//* line 170 *//* line 171 *//* line 172 */}

void maybe_stringconcat (eh,inst,mev) {
                                                       /* line 173 */
    if  inst.buffer1!= NULL and  inst.buffer2!= NULL:  /* line 174 */
        concatenated_string =  ""                      /* line 175 */
        if  0 == len ( inst.buffer1):                  /* line 176 */
            concatenated_string =  inst.buffer2;       /* line 177 */
        elif  0 == len ( inst.buffer2):                /* line 178 */
            concatenated_string =  inst.buffer1;       /* line 179 */
        else:                                          /* line 180 */
            concatenated_string =  inst.buffer1+ inst.buffer2;/* line 181 *//* line 182 */
        send ( eh, "", concatenated_string, mev)       /* line 183 */
        inst.buffer1 =  NULL                           /* line 184 */
        inst.buffer2 =  NULL;                          /* line 185 */;/* line 186 *//* line 187 *//* line 188 */}

/*  */                                                 /* line 189 *//* line 190 */
void string_constant_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 191 */
    static projectRoot                                 /* line 192 */
    name_with_id = gensymbol ( "strconst")             /* line 193 */
    s =  template_data                                 /* line 194 */
    if  projectRoot!= "":                              /* line 195 */
        s = re.sub ( "_00_",  projectRoot,  s)         /* line 196 */;/* line 197 */
    return (make_leaf ( name_with_id, owner, s, "", string_constant_handler, NULL)/* line 198 */)/* line 199 *//* line 200 */}

void string_constant_handler (eh,mev) {
                                                       /* line 201 */
    s =  eh.instance_data                              /* line 202 */
    send ( eh, "", s, mev)                             /* line 203 *//* line 204 *//* line 205 */}

void fakepipename_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 206 */
    instance_name = gensymbol ( "fakepipe")            /* line 207 */
    return (make_leaf ( instance_name, owner, NULL, "", fakepipename_handler, NULL)/* line 208 */)/* line 209 *//* line 210 */}

int  rand =  0                                         /* line 211 */;/* line 212 */
void fakepipename_handler (eh,mev) {
                                                       /* line 213 */
    static rand                                        /* line 214 */
    rand =  rand+ 1
    /*  not very random, but good enough _ ;rand' must be unique within a single run *//* line 215 */
    send ( eh, "", str( "/tmp/fakepipe") +  rand , mev)/* line 216 */;/* line 217 *//* line 218 */}
                                                       /* line 219 */
typedef struct _Switch1star_Instance_Data {
                                                       /* line 220 */
    state;                                             /* line 221 *//* line 222 */
} Switch1star_Instance_Data;
Switch1star_Instance_Data fresh_Switch1star_Instance_Data () {
    Switch1star_Instance_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->state =  "1";                                /* line 221 *//* line 222 */
    return self;
}
                                                       /* line 223 */
void switch1star_reset_handler (eh) {
                                                       /* line 224 */
    inst =  eh.instance_data                           /* line 225 */
    inst =  Switch1star_Instance_Data ()               /* line 226 */;/* line 227 *//* line 228 */}

void switch1star_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 229 */
    name_with_id = gensymbol ( "switch1*")             /* line 230 */
    instp =  Switch1star_Instance_Data ()              /* line 231 */
    return (make_leaf ( name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler)/* line 232 */)/* line 233 *//* line 234 */}

void switch1star_handler (eh,mev) {
                                                       /* line 235 */
    inst =  eh.instance_data                           /* line 236 */
    whichOutput =  inst.state                          /* line 237 */
    if  "" ==  mev.port:                               /* line 238 */
        if  "1" ==  whichOutput:                       /* line 239 */
            forward ( eh, "1", mev)                    /* line 240 */
            inst.state =  "*";                         /* line 241 */
        elif  "*" ==  whichOutput:                     /* line 242 */
            forward ( eh, "*", mev)                    /* line 243 */
        else:                                          /* line 244 */
            send ( eh, "✗", "internal error bad state in switch1*", mev)/* line 245 *//* line 246 */
    elif  "reset" ==  mev.port:                        /* line 247 */
        inst.state =  "1";                             /* line 248 */
    else:                                              /* line 249 */
        send ( eh, "✗", "internal error bad mevent for switch1*", mev)/* line 250 *//* line 251 *//* line 252 *//* line 253 */}

typedef struct _StringAccumulator {
                                                       /* line 254 */
    s;                                                 /* line 255 *//* line 256 */
} StringAccumulator;
StringAccumulator fresh_StringAccumulator () {
    StringAccumulator *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->s =  "";                                     /* line 255 *//* line 256 */
    return self;
}
                                                       /* line 257 */
void strcatstar_reset_handler (eh) {
                                                       /* line 258 */
    eh.instance_data =  StringAccumulator ()           /* line 259 */;/* line 260 *//* line 261 */}

void strcatstar_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 262 */
    name_with_id = gensymbol ( "String Concat *")      /* line 263 */
    instp =  StringAccumulator ()                      /* line 264 */
    return (make_leaf ( name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler)/* line 265 */)/* line 266 *//* line 267 */}

void strcatstar_handler (eh,mev) {
                                                       /* line 268 */
    accum =  eh.instance_data                          /* line 269 */
    if  "" ==  mev.port:                               /* line 270 */
        accum.s =  str( accum.s) +  mev.payload.v      /* line 271 */;
    elif  "fini" ==  mev.port:                         /* line 272 */
        send ( eh, "", accum.s, mev)                   /* line 273 */
    else:                                              /* line 274 */
        send ( eh, "✗", "internal error bad mevent for String Concat *", mev)/* line 275 *//* line 276 *//* line 277 *//* line 278 */}

void stop_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 279 */
    name_with_id = gensymbol ( "Stop")                 /* line 280 */
    inst =  NULL                                       /* line 281 */
    return (make_leaf ( name_with_id, owner, inst, "", stop_handler, NULL)/* line 282 */)/* line 283 *//* line 284 */}

void stop_handler (eh,mev) {
                                                       /* line 285 */
    inst =  eh.instance_data                           /* line 286 */
    parent =  eh.owner                                 /* line 287 */
    s =  str( "   !!! stopping: '") +  str( parent.name) +  "'"  /* line 288 */
    external print ( s, file=sys.stderr)               /* line 289 */
    external                                           /* line 290 */
    parent.stop ( parent)                              /* line 291 */
    send ( eh, "", mev.payload.v, mev)                 /* line 292 *//* line 293 *//* line 294 */}

/*  all of the the built_in leaves are listed here */  /* line 295 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 296 *//* line 297 */
void initialize_stock_components (reg) {
                                                       /* line 298 */
    register_component ( reg,mkTemplate ( "1then2", NULL, deracer_instantiate))/* line 299 */
    register_component ( reg,mkTemplate ( "1→2", NULL, deracer_instantiate))/* line 300 */
    register_component ( reg,mkTemplate ( "trash", NULL, trash_instantiate))/* line 301 */
    register_component ( reg,mkTemplate ( "🗑️", NULL, trash_instantiate))/* line 302 */
    register_component ( reg,mkTemplate ( "🚫", NULL, stop_instantiate))/* line 303 *//* line 304 *//* line 305 */
    register_component ( reg,mkTemplate ( "Read Text File", NULL, low_level_read_text_file_instantiate))/* line 306 */
    register_component ( reg,mkTemplate ( "Ensure String Datum", NULL, ensure_string_datum_instantiate))/* line 307 *//* line 308 */
    register_component ( reg,mkTemplate ( "syncfilewrite", NULL, syncfilewrite_instantiate))/* line 309 */
    register_component ( reg,mkTemplate ( "String Concat", NULL, stringconcat_instantiate))/* line 310 */
    register_component ( reg,mkTemplate ( "switch1*", NULL, switch1star_instantiate))/* line 311 */
    register_component ( reg,mkTemplate ( "String Concat *", NULL, strcatstar_instantiate))/* line 312 */
    /*  for fakepipe */                                /* line 313 */
    register_component ( reg,mkTemplate ( "fakepipename", NULL, fakepipename_instantiate))/* line 314 *//* line 315 *//* line 316 */}
