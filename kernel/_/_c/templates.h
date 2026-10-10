#ifndef Component_Registry_H
#define Component_Registry_H
typedef struct s_Component_Registry {
                                                       /* line 1 */
    Dict_of_Template* templates;                       /* line 2 *//* line 3 */
} Component_Registry;
#endif
                                                       /* line 4 */
#ifndef Template_H
#define Template_H
typedef struct s_Template {
                                                       /* line 5 */
    Str* name;                                         /* line 6 */
    Container* container;                              /* line 7 */
    Finstantiator instantiator;                        /* line 8 *//* line 9 */
} Template;
#endif
                                                       /* line 10 *//* line 19 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 20 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 21 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 22 *//* line 23 */
/*  ... by reading the little-net from an external file  *//* line 24 */
/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 31 */
