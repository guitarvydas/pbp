typedef struct _Datum {
                                                       /* line 1 */
    STR v;                                             /* line 2 */
    FGEN clone;                                        /* line 3 */
    FPROC reclaim;                                     /* line 4 */
    FVOID other; /*  reserved for use on per-project basis  *//* line 5 *//* line 6 */
} Datum;
Datum fresh_Datum () {
    Datum *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->v =  NULL;                                   /* line 2 */
    self->clone =  NULL;                               /* line 3 */
    self->reclaim =  NULL;                             /* line 4 */
    self->other =  NULL; /*  reserved for use on per-project basis  *//* line 5 *//* line 6 */
    return self;
}
