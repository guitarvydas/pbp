decl("step_children",id1).
scope(id1,sc1).
type(id1, procedure).

decl("container",id2).
scope(id2,sc2).
pointerto(id2,"Container").

decl("causingMevent",id3).
scope(id3,sc2).
pointerto(id3,"Mevent").

decl("child",id4).
scope(id4,sc3).
pointerto(id4,"Part").

decl("mev",id5).
scope(id5,sc5).
pointerto(id5,"Mevent").

decl("mev",id6).
scope(id6,sc6).
pointerto(id6,"Mevent").

decl("child",id7).
scope(id7,sc7).
pointerto(id7,"Part").

decl("mev",id8).
scope(id8,sc9).
pointerto(id8,"Mevent").

scopecontains(sc1,sc2).
scopecontains(sc2,sc3).
scopecontains(sc3,sc4).
scopecontains(sc4,sc5).
scopecontains(sc5,sc6).
scopecontains(sc2,sc7).
scopecontains(sc7,sc8).
scopecontains(sc7,sc9).
