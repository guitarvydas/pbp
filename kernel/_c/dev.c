typedef struct _Mevent {
                                                       /* line 1 */
    char* port;                                        /* line 2 */
    char* payload;                                     /* line 3 *//* line 4 */
} Mevent;
Mevent fresh_Mevent () {
    Mevent *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->port =  nil;                                 /* line 2 */
    self->payload =  nil;                              /* line 3 *//* line 4 */
    return self;
}
