typedef struct _Mevent {
                                                       /* line 1 */
    port;
    payload;                                           /* line 4 */
} Mevent;
Mevent fresh_Mevent () {
    Mevent *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->port =  nil;                                 /* line 2 */
    self->payload =  nil;                              /* line 3 *//* line 4 */
    return self;
}
