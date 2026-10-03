I want a JS function that reads JS objects from stdin and creates a symbol table based on scope
We start out with scope `_global`
There might be blank lines in stdin.
The form of the JS objects is as follows
```
{"name":"jit_instantiate", "pointer":true, "type":"Part", "scope:"_global", "kind":"function"}
{"name":"reg", "pointer":true, "type":"Component_Registry", "scope":"jit_instantiate","kind":"parameter"}
{"name":"owner", "pointer":true, "type":"Container", "scope":"jit_instantiate","kind":"parameter"}
{"name":"name", "pointer":true, "type":"Str", "scope":"jit_instantiate","kind":"parameter"}
{"name":"arg", "pointer":true, "type":"Str", "scope":"jit_instantiate","kind":"parameter"}
{"name":"name_with_id", "pointer":true, "type":"Str", "scope":"jit_instantiate", "kind":"variable"}
{"name":"inst", "pointer":true, "type":"Part", "scope":"jit_instantiate", "kind":"variable"}
...
```

---

I want to include the functions in a support file called `support.mjs`. How best to do this? Shall I simply paste the functions into `support.mjs` or use some other feature of JS?
I want a function that builds the symbol table internally.
I want another function that queries the table by name and scope and returns something convenient to use in JS holding the various attributes of each item.
The query function throws if the name isn't defined in the given scope.

--- 

I already have a file `support.mjs` I want to include the read and query functions in it, in whatever way is most reasonable.
I guess that I want to specify which file to read instead of using stdin.
