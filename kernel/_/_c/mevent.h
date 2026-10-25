/*  A mevent has a port (the name of the incoming or outgoing port) and a payload. The payload is always a string. *//* line 1 */
#ifndef Mevent_H
#define Mevent_H
typedef struct s_Mevent {
                                                       /* line 2 */
    Port port;                                         /* line 3 */
    Payload* payload;                                  /* line 4 *//* line 5 */
} Mevent;
#endif
                                                       /* line 6 */
