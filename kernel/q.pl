shadows(B,A) :-
    decl(Name,A),
    scope(A,SCA),
    decl(Name,B),
    scope(B,SCB),
    scopecontains(SCA,SCB).
