/* line 5 */
#ifndef TwoMevents_H
#define TwoMevents_H
typedef struct s_TwoMevents {
                                                       /* line 15 */
    Mevent* firstmev;                                  /* line 16 */
    Mevent* secondmev;                                 /* line 17 *//* line 18 */
} TwoMevents;
#endif
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
#ifndef Deracer_Instance_Data_H
#define Deracer_Instance_Data_H
typedef struct s_Deracer_Instance_Data {
                                                       /* line 21 */
    State state;                                       /* line 22 */
    TwoMevents* buffer;                                /* line 23 *//* line 24 */
} Deracer_Instance_Data;
#endif
                                                       /* line 25 */
#ifndef Syncfilewrite_Data_H
#define Syncfilewrite_Data_H
typedef struct s_Syncfilewrite_Data {
                                                       /* line 108 */
    Str* filename;                                     /* line 109 *//* line 110 */
} Syncfilewrite_Data;
#endif
                                                       /* line 111 */
/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
#ifndef StringConcat_Instance_Data_H
#define StringConcat_Instance_Data_H
typedef struct s_StringConcat_Instance_Data {
                                                       /* line 140 */
    Str* buffer1;                                      /* line 141 */
    Str* buffer2;                                      /* line 142 *//* line 143 */
} StringConcat_Instance_Data;
#endif
                                                       /* line 144 */
/*  */                                                 /* line 189 *//* line 190 *//* line 192 *//* line 214 *//* line 221 */
#ifndef Switch1star_Instance_Data_H
#define Switch1star_Instance_Data_H
typedef struct s_Switch1star_Instance_Data {
                                                       /* line 222 */
    Str* state;                                        /* line 223 *//* line 224 */
} Switch1star_Instance_Data;
#endif
                                                       /* line 225 */
#ifndef StringAccumulator_H
#define StringAccumulator_H
typedef struct s_StringAccumulator {
                                                       /* line 256 */
    Str* s;                                            /* line 257 *//* line 258 */
} StringAccumulator;
#endif
                                                       /* line 259 */
/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
