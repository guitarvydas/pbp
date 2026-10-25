#include "pbp.h"

/*  A mevent has a port (the name of the incoming or outgoing port) and a payload. The payload is always a string. *//* line 1 */
Mevent* fresh_Mevent () {
    Mevent *self;
    self = (Mevent*)malloc(sizeof(Mevent*));
    self->port =  NULL;                                /* line 3 */
    self->payload =  NULL;                             /* line 4 *//* line 5 */
    return self;
}
                                                       /* line 6 */
Mevent* make_mevent (Port port,payload* payload) {
                                                       /* line 7 */
    Mevent* m =  fresh_Mevent ()                       /* line 8 */;
    (*m).port =  port;                                 /* line 9 */
    (*m).payload =  payload;                           /* line 10 */
    return ( m)                                        /* line 11 */;;/* line 12 */}
