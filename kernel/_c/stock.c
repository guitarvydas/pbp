/* line 1 */
void trash_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 2 */
    name_with_id = gensymbol ( "trash")                /* line 3 */
    return (make_leaf ( name_with_id, owner, NULL, "", trash_handler, NULL)/* line 4 */)/* line 5 *//* line 6 */}

void trash_handler (eh,mev) {
                                                       /* line 7 */
    /*  to appease dumped_on_floor checker */          /* line 8 */
                                                       /* line 9 *//* line 10 */}

typedef struct _TwoMevents {
                                                       /* line 11 */
    char* firstmev;                                    /* line 12 */
    char* secondmev;                                   /* line 13 *//* line 14 */
} TwoMevents;
TwoMevents fresh_TwoMevents () {
    TwoMevents *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->firstmev =  NULL;                            /* line 12 */
    self->secondmev =  NULL;                           /* line 13 *//* line 14 */
    return self;
}
                                                       /* line 15 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 16 */
typedef struct _Deracer_Instance_Data {
                                                       /* line 17 */
    char* state;                                       /* line 18 */
    char* buffer;                                      /* line 19 *//* line 20 */
} Deracer_Instance_Data;
Deracer_Instance_Data fresh_Deracer_Instance_Data () {
    Deracer_Instance_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->state =  NULL;                               /* line 18 */
    self->buffer =  NULL;                              /* line 19 *//* line 20 */
    return self;
}
                                                       /* line 21 */
void reclaim_Buffers_from_heap (inst) {
                                                       /* line 22 */
                                                       /* line 23 *//* line 24 *//* line 25 */}

void deracer_reset_handler (eh) {
                                                       /* line 26 */
    inst =  eh.instance_data                           /* line 27 */
    inst.state =  "idle"                               /* line 28 */
    inst.buffer =  TwoMevents ()                       /* line 29 *//* line 30 *//* line 31 */}

void deracer_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 32 */
    name_with_id = gensymbol ( "deracer")              /* line 33 */
    inst =  Deracer_Instance_Data ()                   /* line 34 */
    inst.state =  "idle"                               /* line 35 */
    inst.buffer =  TwoMevents ()                       /* line 36 */
    eh = make_leaf ( name_with_id, owner, inst, "", deracer_handler, deracer_reset_handler)/* line 37 */
    return ( eh)                                       /* line 38 *//* line 39 *//* line 40 */}

void send_firstmev_then_secondmev (eh,inst) {
                                                       /* line 41 */
    forward ( eh, "1", inst.buffer.firstmev)           /* line 42 */
    forward ( eh, "2", inst.buffer.secondmev)          /* line 43 */
    reclaim_Buffers_from_heap ( inst)                  /* line 44 *//* line 45 *//* line 46 */}

void deracer_handler (eh,mev) {
                                                       /* line 47 */
    inst =  eh.instance_data                           /* line 48 */
    if  inst.state ==  "idle":                         /* line 49 */
        if  "1" ==  mev.port:                          /* line 50 */
            inst.buffer.firstmev =  mev                /* line 51 */
            inst.state =  "waitingForSecondmev"        /* line 52 */
        elif  "2" ==  mev.port:                        /* line 53 */
            inst.buffer.secondmev =  mev               /* line 54 */
            inst.state =  "waitingForFirstmev"         /* line 55 */
        else:                                          /* line 56 */
            runtime_error ( str( "bad mev.port (case A) for deracer ") +  mev.port )/* line 57 *//* line 58 */
    elif  inst.state ==  "waitingForFirstmev":         /* line 59 */
        if  "1" ==  mev.port:                          /* line 60 */
            inst.buffer.firstmev =  mev                /* line 61 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 62 */
            inst.state =  "idle"                       /* line 63 */
        else:                                          /* line 64 */
            runtime_error ( str( "deracer: waiting for 1 but got [") +  str( mev.port) +  "] (case B)"  )/* line 65 *//* line 66 */
    elif  inst.state ==  "waitingForSecondmev":        /* line 67 */
        if  "2" ==  mev.port:                          /* line 68 */
            inst.buffer.secondmev =  mev               /* line 69 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 70 */
            inst.state =  "idle"                       /* line 71 */
        else:                                          /* line 72 */
            runtime_error ( str( "deracer: waiting for 2 but got [") +  str( mev.port) +  "] (case C)"  )/* line 73 *//* line 74 */
    else:                                              /* line 75 */
        runtime_error ( "bad state for deracer {eh.state}")/* line 76 *//* line 77 *//* line 78 *//* line 79 */}

void low_level_read_text_file_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 80 */
    name_with_id = gensymbol ( "Low Level Read Text File")/* line 81 */
    return (make_leaf ( name_with_id, owner, NULL, "", low_level_read_text_file_handler, NULL)/* line 82 */)/* line 83 *//* line 84 */}

void low_level_read_text_file_handler (eh,mev) {
                                                       /* line 85 */
    fname =  mev.payload.v                             /* line 86 */
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
                                                       /* line 87 *//* line 88 *//* line 89 */}

void ensure_string_datum_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 90 */
    name_with_id = gensymbol ( "Ensure String Datum")  /* line 91 */
    return (make_leaf ( name_with_id, owner, NULL, "", ensure_string_datum_handler, NULL)/* line 92 */)/* line 93 *//* line 94 */}

void ensure_string_datum_handler (eh,mev) {
                                                       /* line 95 */
    if  "string" ==  mev.payload.kind ():              /* line 96 */
        forward ( eh, "", mev)                         /* line 97 */
    else:                                              /* line 98 */
        emev =  str( "*** ensure: type error (expected a string payload) but got ") +  mev.payload /* line 99 */
        send ( eh, "✗", emev, mev)                     /* line 100 *//* line 101 *//* line 102 *//* line 103 */}

typedef struct _Syncfilewrite_Data {
                                                       /* line 104 */
    char* filename;                                    /* line 105 *//* line 106 */
} Syncfilewrite_Data;
Syncfilewrite_Data fresh_Syncfilewrite_Data () {
    Syncfilewrite_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->filename =  "";                              /* line 105 *//* line 106 */
    return self;
}
                                                       /* line 107 */
void syncfilewrite_reset_handler (eh) {
                                                       /* line 108 */
    eh.instance_data =  Syncfilewrite_Data ()          /* line 109 *//* line 110 *//* line 111 */}

/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 112 */
void syncfilewrite_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 113 */
    name_with_id = gensymbol ( "syncfilewrite")        /* line 114 */
    inst =  Syncfilewrite_Data ()                      /* line 115 */
    return (make_leaf ( name_with_id, owner, inst, "", syncfilewrite_handler, syncfilewrite_reset_handler)/* line 116 */)/* line 117 *//* line 118 */}

void syncfilewrite_handler (eh,mev) {
                                                       /* line 119 */
    inst =  eh.instance_data                           /* line 120 */
    if  "filename" ==  mev.port:                       /* line 121 */
        inst.filename =  mev.payload.v                 /* line 122 */
    elif  "input" ==  mev.port:                        /* line 123 */
        contents =  mev.payload.v                      /* line 124 */
        f = open ( inst.filename, "w")                 /* line 125 */
        if  f!= NULL:                                  /* line 126 */
            f.write ( mev.payload.v)                   /* line 127 */
            f.close ()                                 /* line 128 */
            send ( eh, "done",new_datum_bang (), mev)  /* line 129 */
        else:                                          /* line 130 */
            send ( eh, "✗", str( "open error on file ") +  inst.filename , mev)/* line 131 *//* line 132 *//* line 133 *//* line 134 *//* line 135 */}

typedef struct _StringConcat_Instance_Data {
                                                       /* line 136 */
    char* buffer1;                                     /* line 137 */
    char* buffer2;                                     /* line 138 *//* line 139 */
} StringConcat_Instance_Data;
StringConcat_Instance_Data fresh_StringConcat_Instance_Data () {
    StringConcat_Instance_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->buffer1 =  NULL;                             /* line 137 */
    self->buffer2 =  NULL;                             /* line 138 *//* line 139 */
    return self;
}
                                                       /* line 140 */
void stringconcat_reset_handler (eh) {
                                                       /* line 141 */
    inst =  eh.instance_data                           /* line 142 */
    inst.buffer1 =  NULL                               /* line 143 */
    inst.buffer2 =  NULL                               /* line 144 *//* line 145 *//* line 146 */}

void stringconcat_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 147 */
    name_with_id = gensymbol ( "stringconcat")         /* line 148 */
    instp =  StringConcat_Instance_Data ()             /* line 149 */
    return (make_leaf ( name_with_id, owner, instp, "", stringconcat_handler, stringconcat_reset_handler)/* line 150 */)/* line 151 *//* line 152 */}

void stringconcat_handler (eh,mev) {
                                                       /* line 153 */
    inst =  eh.instance_data                           /* line 154 */
    if  "1" ==  mev.port:                              /* line 155 */
        inst.buffer1 = clone_string ( mev.payload.v)   /* line 156 */
        maybe_stringconcat ( eh, inst, mev)            /* line 157 */
    elif  "2" ==  mev.port:                            /* line 158 */
        inst.buffer2 = clone_string ( mev.payload.v)   /* line 159 */
        maybe_stringconcat ( eh, inst, mev)            /* line 160 */
    elif  "reset" ==  mev.port:                        /* line 161 */
        inst.buffer1 =  NULL                           /* line 162 */
        inst.buffer2 =  NULL                           /* line 163 */
    else:                                              /* line 164 */
        runtime_error ( str( "bad mev.port for stringconcat: ") +  mev.port )/* line 165 *//* line 166 *//* line 167 *//* line 168 */}

void maybe_stringconcat (eh,inst,mev) {
                                                       /* line 169 */
    if  inst.buffer1!= NULL and  inst.buffer2!= NULL:  /* line 170 */
        concatenated_string =  ""                      /* line 171 */
        if  0 == len ( inst.buffer1):                  /* line 172 */
            concatenated_string =  inst.buffer2        /* line 173 */
        elif  0 == len ( inst.buffer2):                /* line 174 */
            concatenated_string =  inst.buffer1        /* line 175 */
        else:                                          /* line 176 */
            concatenated_string =  inst.buffer1+ inst.buffer2/* line 177 *//* line 178 */
        send ( eh, "", concatenated_string, mev)       /* line 179 */
        inst.buffer1 =  NULL                           /* line 180 */
        inst.buffer2 =  NULL                           /* line 181 *//* line 182 *//* line 183 *//* line 184 */}

/*  */                                                 /* line 185 *//* line 186 */
void string_constant_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 187 */
    static projectRoot                                 /* line 188 */
    name_with_id = gensymbol ( "strconst")             /* line 189 */
    s =  template_data                                 /* line 190 */
    if  projectRoot!= "":                              /* line 191 */
        s = re.sub ( "_00_",  projectRoot,  s)         /* line 192 *//* line 193 */
    return (make_leaf ( name_with_id, owner, s, "", string_constant_handler, NULL)/* line 194 */)/* line 195 *//* line 196 */}

void string_constant_handler (eh,mev) {
                                                       /* line 197 */
    s =  eh.instance_data                              /* line 198 */
    send ( eh, "", s, mev)                             /* line 199 *//* line 200 *//* line 201 */}

void fakepipename_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 202 */
    instance_name = gensymbol ( "fakepipe")            /* line 203 */
    return (make_leaf ( instance_name, owner, NULL, "", fakepipename_handler, NULL)/* line 204 */)/* line 205 *//* line 206 */}

int  rand =  0                                         /* line 207 */;/* line 208 */
void fakepipename_handler (eh,mev) {
                                                       /* line 209 */
    static rand                                        /* line 210 */
    rand =  rand+ 1
    /*  not very random, but good enough _ ;rand' must be unique within a single run *//* line 211 */
    send ( eh, "", str( "/tmp/fakepipe") +  rand , mev)/* line 212 *//* line 213 *//* line 214 */}
                                                       /* line 215 */
typedef struct _Switch1star_Instance_Data {
                                                       /* line 216 */
    char* state;                                       /* line 217 *//* line 218 */
} Switch1star_Instance_Data;
Switch1star_Instance_Data fresh_Switch1star_Instance_Data () {
    Switch1star_Instance_Data *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->state =  "1";                                /* line 217 *//* line 218 */
    return self;
}
                                                       /* line 219 */
void switch1star_reset_handler (eh) {
                                                       /* line 220 */
    inst =  eh.instance_data                           /* line 221 */
    inst =  Switch1star_Instance_Data ()               /* line 222 *//* line 223 *//* line 224 */}

void switch1star_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 225 */
    name_with_id = gensymbol ( "switch1*")             /* line 226 */
    instp =  Switch1star_Instance_Data ()              /* line 227 */
    return (make_leaf ( name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler)/* line 228 */)/* line 229 *//* line 230 */}

void switch1star_handler (eh,mev) {
                                                       /* line 231 */
    inst =  eh.instance_data                           /* line 232 */
    whichOutput =  inst.state                          /* line 233 */
    if  "" ==  mev.port:                               /* line 234 */
        if  "1" ==  whichOutput:                       /* line 235 */
            forward ( eh, "1", mev)                    /* line 236 */
            inst.state =  "*"                          /* line 237 */
        elif  "*" ==  whichOutput:                     /* line 238 */
            forward ( eh, "*", mev)                    /* line 239 */
        else:                                          /* line 240 */
            send ( eh, "✗", "internal error bad state in switch1*", mev)/* line 241 *//* line 242 */
    elif  "reset" ==  mev.port:                        /* line 243 */
        inst.state =  "1"                              /* line 244 */
    else:                                              /* line 245 */
        send ( eh, "✗", "internal error bad mevent for switch1*", mev)/* line 246 *//* line 247 *//* line 248 *//* line 249 */}

typedef struct _StringAccumulator {
                                                       /* line 250 */
    char* s;                                           /* line 251 *//* line 252 */
} StringAccumulator;
StringAccumulator fresh_StringAccumulator () {
    StringAccumulator *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->s =  "";                                     /* line 251 *//* line 252 */
    return self;
}
                                                       /* line 253 */
void strcatstar_reset_handler (eh) {
                                                       /* line 254 */
    eh.instance_data =  StringAccumulator ()           /* line 255 *//* line 256 *//* line 257 */}

void strcatstar_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 258 */
    name_with_id = gensymbol ( "String Concat *")      /* line 259 */
    instp =  StringAccumulator ()                      /* line 260 */
    return (make_leaf ( name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler)/* line 261 */)/* line 262 *//* line 263 */}

void strcatstar_handler (eh,mev) {
                                                       /* line 264 */
    accum =  eh.instance_data                          /* line 265 */
    if  "" ==  mev.port:                               /* line 266 */
        accum.s =  str( accum.s) +  mev.payload.v      /* line 267 */
    elif  "fini" ==  mev.port:                         /* line 268 */
        send ( eh, "", accum.s, mev)                   /* line 269 */
    else:                                              /* line 270 */
        send ( eh, "✗", "internal error bad mevent for String Concat *", mev)/* line 271 *//* line 272 *//* line 273 *//* line 274 */}

void stop_instantiate (reg,owner,name,template_data,arg) {
                                                       /* line 275 */
    name_with_id = gensymbol ( "Stop")                 /* line 276 */
    inst =  NULL                                       /* line 277 */
    return (make_leaf ( name_with_id, owner, inst, "", stop_handler, NULL)/* line 278 */)/* line 279 *//* line 280 */}

void stop_handler (eh,mev) {
                                                       /* line 281 */
    inst =  eh.instance_data                           /* line 282 */
    parent =  eh.owner                                 /* line 283 */
    s =  str( "   !!! stopping: '") +  str( parent.name) +  "'"  /* line 284 */
    external print ( s, file=sys.stderr)               /* line 285 */
    external                                           /* line 286 */
    parent.stop ( parent)                              /* line 287 */
    send ( eh, "", mev.payload.v, mev)                 /* line 288 *//* line 289 *//* line 290 */}

/*  all of the the built_in leaves are listed here */  /* line 291 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 292 *//* line 293 */
void initialize_stock_components (reg) {
                                                       /* line 294 */
    register_component ( reg,mkTemplate ( "1then2", NULL, deracer_instantiate))/* line 295 */
    register_component ( reg,mkTemplate ( "1→2", NULL, deracer_instantiate))/* line 296 */
    register_component ( reg,mkTemplate ( "trash", NULL, trash_instantiate))/* line 297 */
    register_component ( reg,mkTemplate ( "🗑️", NULL, trash_instantiate))/* line 298 */
    register_component ( reg,mkTemplate ( "🚫", NULL, stop_instantiate))/* line 299 *//* line 300 *//* line 301 */
    register_component ( reg,mkTemplate ( "Read Text File", NULL, low_level_read_text_file_instantiate))/* line 302 */
    register_component ( reg,mkTemplate ( "Ensure String Datum", NULL, ensure_string_datum_instantiate))/* line 303 *//* line 304 */
    register_component ( reg,mkTemplate ( "syncfilewrite", NULL, syncfilewrite_instantiate))/* line 305 */
    register_component ( reg,mkTemplate ( "String Concat", NULL, stringconcat_instantiate))/* line 306 */
    register_component ( reg,mkTemplate ( "switch1*", NULL, switch1star_instantiate))/* line 307 */
    register_component ( reg,mkTemplate ( "String Concat *", NULL, strcatstar_instantiate))/* line 308 */
    /*  for fakepipe */                                /* line 309 */
    register_component ( reg,mkTemplate ( "fakepipename", NULL, fakepipename_instantiate))/* line 310 *//* line 311 *//* line 312 */}
