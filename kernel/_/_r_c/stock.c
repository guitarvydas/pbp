#include "pbp.h"

Str* clone_string (Str* s) {
                                                       /* line 1 */
    return ( s)                                        /* line 2 *//* line 3 *//* line 4 */}
                                                       /* line 5 */
Leaf* trash_instantiate (Component_Registry* reg,Container* owner,Str* name,Template* template_data,Str* arg) {
                                                       /* line 6 */
    Str* name_with_id = gensymbol ( counted("trash"))  /* line 7 */
    return (make_leaf ( name_with_id, owner, NULL, counted(""), trash_handler, NULL)/* line 8 */)/* line 9 *//* line 10 */}

void trash_handler (Part* eh,Mevent* mev) {
                                                       /* line 11 */
    /*  to appease dumped_on_floor checker */          /* line 12 */
                                                       /* line 13 *//* line 14 */}

TwoMevents* fresh_TwoMevents () {
    TwoMevents *self;
    self = (TwoMevents*)malloc(sizeof(TwoMevents*));
    self->firstmev =  NULL;                            /* line 16 */
    self->secondmev =  NULL;                           /* line 17 *//* line 18 */
    return self;
}
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
Deracer_Instance_Data* fresh_Deracer_Instance_Data () {
    Deracer_Instance_Data *self;
    self = (Deracer_Instance_Data*)malloc(sizeof(Deracer_Instance_Data*));
    self->state =  NULL;                               /* line 22 */
    self->buffer =  NULL;                              /* line 23 *//* line 24 */
    return self;
}
                                                       /* line 25 */
void reclaim_Buffers_from_heap (Leaf* inst) {
                                                       /* line 26 */
                                                       /* line 27 *//* line 28 *//* line 29 */}

void deracer_reset_handler (Leaf* eh) {
                                                       /* line 30 */
    Deracer_Instance_Data*  inst =   (*eh).instance_data;/* line 31 */
    (*inst).state =  counted("idle");                  /* line 32 */
    (*inst).buffer =  fresh_TwoMevents ()              /* line 33 */;;;/* line 34 *//* line 35 */}

Leaf* deracer_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 36 */
    Str* name_with_id = gensymbol ( counted("deracer"))/* line 37 */
    Deracer_Instance_Data*  inst =  fresh_Deracer_Instance_Data ()/* line 38 */;
    (*inst).state =  counted("idle");                  /* line 39 */
    (*inst).buffer =  fresh_TwoMevents ()              /* line 40 */;
    Leaf* eh = make_leaf ( name_with_id, owner, inst, counted(""), deracer_handler, deracer_reset_handler)/* line 41 */
    return ( eh)                                       /* line 42 */;;/* line 43 *//* line 44 */}

void send_firstmev_then_secondmev (Leaf* eh,Deracer_Instance_Data* inst) {
                                                       /* line 45 */
    forward ( eh, counted("1"),   (*inst).buffer.firstmev)/* line 46 */
    forward ( eh, counted("2"),   (*inst).buffer.secondmev)/* line 47 */
    reclaim_Buffers_from_heap ( inst)                  /* line 48 *//* line 49 *//* line 50 */}

void deracer_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 51 */
    Deracer_Instance_Data*  inst =   (*eh).instance_data;/* line 52 */
    if   (*inst).state ==  counted("idle"):            /* line 53 */
        if  counted("1") ==   (*mev).port:             /* line 54 */
            (*inst).buffer.firstmev =  mev;            /* line 55 */
            (*inst).state =  counted("waitingForSecondmev");;/* line 56 */;
        elif  counted("2") ==   (*mev).port:           /* line 57 */
            (*inst).buffer.secondmev =  mev;           /* line 58 */
            (*inst).state =  counted("waitingForFirstmev");;/* line 59 */;
        else:                                          /* line 60 */
            runtime_error ( str( counted("bad_mev.port_(case_A)_for_deracer_")) +   (*mev).port )/* line 61 *//* line 62 */
    elif   (*inst).state ==  counted("waitingForFirstmev"):/* line 63 */
        if  counted("1") ==   (*mev).port:             /* line 64 */
            (*inst).buffer.firstmev =  mev;            /* line 65 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 66 */
            (*inst).state =  counted("idle");;         /* line 67 */;
        else:                                          /* line 68 */
            runtime_error ( str( counted("deracer:_waiting_for_1_but_got_[")) +  str(  (*mev).port) +  counted("]_(case_B)")  )/* line 69 *//* line 70 */
    elif   (*inst).state ==  counted("waitingForSecondmev"):/* line 71 */
        if  counted("2") ==   (*mev).port:             /* line 72 */
            (*inst).buffer.secondmev =  mev;           /* line 73 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 74 */
            (*inst).state =  counted("idle");;         /* line 75 */;
        else:                                          /* line 76 */
            runtime_error ( str( counted("deracer:_waiting_for_2_but_got_[")) +  str(  (*mev).port) +  counted("]_(case_C)")  )/* line 77 *//* line 78 */
    else:                                              /* line 79 */
        runtime_error ( counted("bad_state_for_deracer_{eh.state}"))/* line 80 *//* line 81 *//* line 82 *//* line 83 */}

Leaf* low_level_read_text_file_instantiate (Component_Registry* reg,Container* owner,Str* name,Template* template_data,Str* arg) {
                                                       /* line 84 */
    Str* name_with_id = gensymbol ( counted("Low_Level_Read_Text_File"))/* line 85 */
    return (make_leaf ( name_with_id, owner, NULL, counted(""), low_level_read_text_file_handler, NULL)/* line 86 */)/* line 87 *//* line 88 */}

void low_level_read_text_file_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 89 */
    Payload* fname =    (*mev).payload.v               /* line 90 */

    try:
        f = open (fname)
    except Exception as e:
        f = None
    if f != None:
        data = f.read ()
        if data!= None:
            send (eh, counted(""), data, mev)
        else:
            send (eh, counted("✗"), f"read error on file '{fname}'", mev)
        f.close ()
    else:
        send (eh, counted("✗"), f"open error on file '{fname}'", mev)
                                                       /* line 91 *//* line 92 *//* line 93 */}

Leaf* ensure_string_datum_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 94 */
    Str* name_with_id = gensymbol ( counted("Ensure_String_Datum"))/* line 95 */
    return (make_leaf ( name_with_id, owner, NULL, counted(""), ensure_string_datum_handler, NULL)/* line 96 */)/* line 97 *//* line 98 */}

void ensure_string_datum_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 99 */
    if  counted("string") ==    (*mev).payload.kind ():/* line 100 */
        forward ( eh, counted(""), mev)                /* line 101 */
    else:                                              /* line 102 */
        Str* emev =  str( counted("***_ensure:_type_error_(expected_a_string_payload)_but_got_")) +   (*mev).payload /* line 103 */
        send ( eh, counted("✗"), emev, mev)            /* line 104 *//* line 105 *//* line 106 *//* line 107 */}

Syncfilewrite_Data* fresh_Syncfilewrite_Data () {
    Syncfilewrite_Data *self;
    self = (Syncfilewrite_Data*)malloc(sizeof(Syncfilewrite_Data*));
    self->filename =  counted("");                     /* line 109 *//* line 110 */
    return self;
}
                                                       /* line 111 */
void syncfilewrite_reset_handler (Leaf* eh) {
                                                       /* line 112 */
    (*eh).instance_data =  fresh_Syncfilewrite_Data () /* line 113 */;;/* line 114 *//* line 115 */}

/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
Leaf* syncfilewrite_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 117 */
    Str* name_with_id = gensymbol ( counted("syncfilewrite"))/* line 118 */
    SyncFilewrite_Data* inst =  fresh_Syncfilewrite_Data ()/* line 119 */
    return (make_leaf ( name_with_id, owner, inst, counted(""), syncfilewrite_handler, syncfilewrite_reset_handler)/* line 120 */)/* line 121 *//* line 122 */}

void syncfilewrite_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 123 */
    Synfilewrite_Data*  inst =   (*eh).instance_data;  /* line 124 */
    if  counted("filename") ==   (*mev).port:          /* line 125 */
        (*inst).filename =    (*mev).payload.v;;       /* line 126 */
    elif  counted("input") ==   (*mev).port:           /* line 127 */
        Payload* contents =    (*mev).payload.v        /* line 128 */
        FileDescriptor*  f = open (  (*inst).filename, counted("w"))/* line 129 */;
        if  f!= NULL:                                  /* line 130 */
            (*f).write (   (*mev).payload.v)           /* line 131 */
            (*f).close ()                              /* line 132 */
            send ( eh, counted("done"),new_datum_bang (), mev)/* line 133 */
        else:                                          /* line 134 */
            send ( eh, counted("✗"), str( counted("open_error_on_file_")) +   (*inst).filename , mev)/* line 135 *//* line 136 *//* line 137 *//* line 138 *//* line 139 */}

StringConcat_Instance_Data* fresh_StringConcat_Instance_Data () {
    StringConcat_Instance_Data *self;
    self = (StringConcat_Instance_Data*)malloc(sizeof(StringConcat_Instance_Data*));
    self->buffer1 =  NULL;                             /* line 141 */
    self->buffer2 =  NULL;                             /* line 142 *//* line 143 */
    return self;
}
                                                       /* line 144 */
void stringconcat_reset_handler (Leaf* eh) {
                                                       /* line 145 */
    StringConcat_Instance_Data*  inst =   (*eh).instance_data;/* line 146 */
    (*inst).buffer1 =  NULL;                           /* line 147 */
    (*inst).buffer2 =  NULL;;                          /* line 148 */;/* line 149 *//* line 150 */}

Leaf* stringconcat_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 151 */
    Str* name_with_id = gensymbol ( counted("stringconcat"))/* line 152 */
    StringConcat_Instance_Data* instp =  fresh_StringConcat_Instance_Data ()/* line 153 */
    return (make_leaf ( name_with_id, owner, instp, counted(""), stringconcat_handler, stringconcat_reset_handler)/* line 154 */)/* line 155 *//* line 156 */}

void stringconcat_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 157 */
    StringConcat_Instance_Data*  inst =   (*eh).instance_data;/* line 158 */
    if  counted("1") ==   (*mev).port:                 /* line 159 */
        (*inst).buffer1 = clone_string (   (*mev).payload.v)/* line 160 */;
        maybe_stringconcat ( eh, inst, mev)            /* line 161 */;
    elif  counted("2") ==   (*mev).port:               /* line 162 */
        (*inst).buffer2 = clone_string (   (*mev).payload.v)/* line 163 */;
        maybe_stringconcat ( eh, inst, mev)            /* line 164 */;
    elif  counted("reset") ==   (*mev).port:           /* line 165 */
        (*inst).buffer1 =  NULL;                       /* line 166 */
        (*inst).buffer2 =  NULL;;                      /* line 167 */;
    else:                                              /* line 168 */
        runtime_error ( str( counted("bad_mev.port_for_stringconcat:_")) +   (*mev).port )/* line 169 *//* line 170 *//* line 171 *//* line 172 */}

void maybe_stringconcat (Leaf* eh,StringConcat_Instance_Dat* inst,Mevent* mev) {
                                                       /* line 173 */
    if   (*inst).buffer1!= NULL and   (*inst).buffer2!= NULL:/* line 174 */
        Str*  concatenated_string =  counted("");      /* line 175 */
        if  0 == len (  (*inst).buffer1):              /* line 176 */
            concatenated_string =   (*inst).buffer2;;  /* line 177 */
        elif  0 == len (  (*inst).buffer2):            /* line 178 */
            concatenated_string =   (*inst).buffer1;;  /* line 179 */
        else:                                          /* line 180 */
            concatenated_string =   (*inst).buffer1+  (*inst).buffer2;;/* line 181 *//* line 182 */
        send ( eh, counted(""), concatenated_string, mev)/* line 183 */
        (*inst).buffer1 =  NULL;                       /* line 184 */
        (*inst).buffer2 =  NULL;;                      /* line 185 */;/* line 186 *//* line 187 *//* line 188 */}

/*  */                                                 /* line 189 *//* line 190 */
Str* =  counted(".")                                   /* line 191 */;/* line 192 */
Leaf* string_constant_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 193 */
    static projectRoot                                 /* line 194 */
    Str* name_with_id = gensymbol ( counted("strconst"))/* line 195 */
    Str*  s =  template_data;                          /* line 196 */
    if  projectRoot!= counted(""):                     /* line 197 */
        s = re.sub ( counted("_00_"),  projectRoot,  s)/* line 198 */;;/* line 199 */
    return (make_leaf ( name_with_id, owner, s, counted(""), string_constant_handler, NULL)/* line 200 */)/* line 201 *//* line 202 */}

void string_constant_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 203 */
    Str* s =   (*eh).instance_data                     /* line 204 */
    send ( eh, counted(""), s, mev)                    /* line 205 *//* line 206 *//* line 207 */}

Leaf* fakepipename_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 208 */
    Str* instance_name = gensymbol ( counted("fakepipe"))/* line 209 */
    return (make_leaf ( instance_name, owner, NULL, counted(""), fakepipename_handler, NULL)/* line 210 */)/* line 211 *//* line 212 */}

Number =  0                                            /* line 213 */;/* line 214 */
void fakepipename_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 215 */
    static rand                                        /* line 216 */
    rand =  rand+ 1;
    /*  not very random, but good enough _ ;rand' must be unique within a single run *//* line 217 */
    send ( eh, counted(""), str( counted("/tmp/fakepipe")) +  rand , mev)/* line 218 */;/* line 219 *//* line 220 */}
                                                       /* line 221 */
Switch1star_Instance_Data* fresh_Switch1star_Instance_Data () {
    Switch1star_Instance_Data *self;
    self = (Switch1star_Instance_Data*)malloc(sizeof(Switch1star_Instance_Data*));
    self->state =  counted("1");                       /* line 223 *//* line 224 */
    return self;
}
                                                       /* line 225 */
void switch1star_reset_handler (Leaf* eh) {
                                                       /* line 226 */
    Switch1star_Instance_Data*  inst =   (*eh).instance_data;/* line 227 */
    inst =  fresh_Switch1star_Instance_Data ()         /* line 228 */;;/* line 229 *//* line 230 */}

Leaf* switch1star_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 231 */
    Str* name_with_id = gensymbol ( counted("switch1*"))/* line 232 */
    Switch1star_Instance_Data* instp =  fresh_Switch1star_Instance_Data ()/* line 233 */
    return (make_leaf ( name_with_id, owner, instp, counted(""), switch1star_handler, switch1star_reset_handler)/* line 234 */)/* line 235 *//* line 236 */}

void switch1star_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 237 */
    Switch1star_Instance_Data*  inst =   (*eh).instance_data;/* line 238 */
    Str* whichOutput =   (*inst).state                 /* line 239 */
    if  counted("") ==   (*mev).port:                  /* line 240 */
        if  counted("1") ==  whichOutput:              /* line 241 */
            forward ( eh, counted("1"), mev)           /* line 242 */
            (*inst).state =  counted("*");;            /* line 243 */
        elif  counted("*") ==  whichOutput:            /* line 244 */
            forward ( eh, counted("*"), mev)           /* line 245 */
        else:                                          /* line 246 */
            send ( eh, counted("✗"), counted("internal_error_bad_state_in_switch1*"), mev)/* line 247 *//* line 248 */
    elif  counted("reset") ==   (*mev).port:           /* line 249 */
        (*inst).state =  counted("1");;                /* line 250 */
    else:                                              /* line 251 */
        send ( eh, counted("✗"), counted("internal_error_bad_mevent_for_switch1*"), mev)/* line 252 *//* line 253 *//* line 254 *//* line 255 */}

StringAccumulator* fresh_StringAccumulator () {
    StringAccumulator *self;
    self = (StringAccumulator*)malloc(sizeof(StringAccumulator*));
    self->s =  counted("");                            /* line 257 *//* line 258 */
    return self;
}
                                                       /* line 259 */
void strcatstar_reset_handler (Leaf* eh) {
                                                       /* line 260 */
    (*eh).instance_data =  fresh_StringAccumulator ()  /* line 261 */;;/* line 262 *//* line 263 */}

Leaf* strcatstar_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 264 */
    Str* name_with_id = gensymbol ( counted("String_Concat_*"))/* line 265 */
    Switch1star_Instance_Data* instp =  fresh_StringAccumulator ()/* line 266 */
    return (make_leaf ( name_with_id, owner, instp, counted(""), strcatstar_handler, strcatstar_reset_handler)/* line 267 */)/* line 268 *//* line 269 */}

void strcatstar_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 270 */
    Switch1star_Instance_Data*  accum =   (*eh).instance_data;/* line 271 */
    if  counted("") ==   (*mev).port:                  /* line 272 */
        (*accum).s =  str(  (*accum).s) +    (*mev).payload.v /* line 273 */;;
    elif  counted("fini") ==   (*mev).port:            /* line 274 */
        send ( eh, counted(""),  (*accum).s, mev)      /* line 275 */
    else:                                              /* line 276 */
        send ( eh, counted("✗"), counted("internal_error_bad_mevent_for_String_Concat_*"), mev)/* line 277 *//* line 278 *//* line 279 *//* line 280 */}

Leaf* stop_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 281 */
    Str* name_with_id = gensymbol ( counted("Stop"))   /* line 282 */
    Switch1star_Instance_Data* inst =  NULL            /* line 283 */
    return (make_leaf ( name_with_id, owner, inst, counted(""), stop_handler, NULL)/* line 284 */)/* line 285 *//* line 286 */}

void stop_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 287 */
    Any*  inst =   (*eh).instance_data;                /* line 288 */
    Container*  parent =   (*eh).owner;                /* line 289 */
    Str*  s =  str( counted("___!!!_stopping:_'")) +  str(  (*parent).name) +  counted("'")  /* line 290 */;
    print ( s, file=sys.stderr)                        /* line 291 */
                                                       /* line 292 */
    (*parent).reset ( parent)                          /* line 293 */
    send ( eh, counted(""),   (*mev).payload.v, mev)   /* line 294 *//* line 295 *//* line 296 */}

/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
void initialize_stock_components (Component_Registry* reg) {
                                                       /* line 300 */
    register_component ( reg,mkTemplate ( counted("1then2"), NULL, deracer_instantiate))/* line 301 */
    register_component ( reg,mkTemplate ( counted("1→2"), NULL, deracer_instantiate))/* line 302 */
    register_component ( reg,mkTemplate ( counted("trash"), NULL, trash_instantiate))/* line 303 */
    register_component ( reg,mkTemplate ( counted("🗑️"), NULL, trash_instantiate))/* line 304 */
    register_component ( reg,mkTemplate ( counted("🚫"), NULL, stop_instantiate))/* line 305 *//* line 306 *//* line 307 */
    register_component ( reg,mkTemplate ( counted("Read_Text_File"), NULL, low_level_read_text_file_instantiate))/* line 308 */
    register_component ( reg,mkTemplate ( counted("Ensure_String_Datum"), NULL, ensure_string_datum_instantiate))/* line 309 *//* line 310 */
    register_component ( reg,mkTemplate ( counted("syncfilewrite"), NULL, syncfilewrite_instantiate))/* line 311 */
    register_component ( reg,mkTemplate ( counted("String_Concat"), NULL, stringconcat_instantiate))/* line 312 */
    register_component ( reg,mkTemplate ( counted("switch1*"), NULL, switch1star_instantiate))/* line 313 */
    register_component ( reg,mkTemplate ( counted("String_Concat_*"), NULL, strcatstar_instantiate))/* line 314 */
    /*  for fakepipe */                                /* line 315 */
    register_component ( reg,mkTemplate ( counted("fakepipename"), NULL, fakepipename_instantiate))/* line 316 *//* line 317 *//* line 318 */}
