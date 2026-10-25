/*  A mevent has a port (the name of the incoming or outgoing port) and a payload. The payload is always a string. *//* line 1 */
class Mevent {
  constructor () {                                     /* line 2 */

    this.port =  null;                                 /* line 3 */
    this.payload =  null;                              /* line 4 *//* line 5 */
  }
}
                                                       /* line 6 */
function make_mevent (port,payload) {                  /* line 7 */
    let m =  new Mevent ();                            /* line 8 */;
    m.port =  port;                                    /* line 9 */
    m.payload =  payload;                              /* line 10 */
    return  m;                                         /* line 11 *//* line 12 */
}
