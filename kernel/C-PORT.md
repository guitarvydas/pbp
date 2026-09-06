# Purpose
Copy and alter one of emitters to emit C.

Goal: learn about impact of using a non-garbage-collected language. Question: will GC be needed at all, since this is a custom port with known parameters. Can we measure the exact number of mevents generated during design and then pre-allocated, obviating the need for any GC at all (with efficiency benefits)?

Goal: learn what constructs, e.g. strings (handled by Python, not handled by C), need to be pre-allocated or converted to symbols.

Goal: understand what else needs to be generalized in `emit.ohm` and the `.rt` meta language.

Goal: understand issues related to making a tiny version

# Method
Copy and alter `emitpython.rwr`

# Observations
# Conclusion
